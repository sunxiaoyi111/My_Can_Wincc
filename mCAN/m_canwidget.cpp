#include "m_canwidget.h"
#include "ui_m_canwidget.h"

#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QMetaType>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include "m_canconfig.h"
#include <QVariant>
M_CANWidget::M_CANWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::M_CANWidget)
{
    ui->setupUi(this);
    lock_can_settings();
    m_canconfig cfg;
    cfg.m_canconfig_init();


    // 创建一个正则表达式对象，匹配xx xx xx xx xx xx xx xx这样的格式
    QRegExp regExp ("^([0-9A-Fa-f]{2} ){0,7}[0-9A-Fa-f]{2}$");
    // 创建一个验证器对象，使用正则表达式对象作为参数
    QRegExpValidator *validator = new QRegExpValidator (regExp, this);
    // 设置QLineEdit的验证器为验证器对象
    ui->lineEdit_candata->setValidator (validator);


    #pragma region "注册信号与槽 自定义传递变量(QT通用定义未包含需要在这里注册)" {
    qRegisterMetaType<M_CanDataBase> ("M_CanDataBase");
//    qRegisterMetaType<M_CanDataBase> ("M_CanDataBase");
#pragma endregion}

    //CAN收发任务初始化
    m_can = new M_Can_Task;
    //新建一个线程对象
    my_Can_Thread = new QThread(this);
    //使用movetothread 方法，m_can_task类中的方法将在新的线程中执行
    m_can->moveToThread(my_Can_Thread);

    can_dispatcher = new Can_dispatcher;
    my_Can_dispatcher_Thread = new QThread(this);
    can_dispatcher->moveToThread(my_Can_dispatcher_Thread);
    connect(m_can,&M_Can_Task::mctask_Rx_Variant,can_dispatcher,&Can_dispatcher::mc_candispatcher_Variant);
    my_Can_dispatcher_Thread->start();


    #pragma region "信号与槽函数初始化" {
    connect(&m_can->mct_Rx_timer,&QTimer::timeout,m_can,&M_Can_Task::mct_Recurring_Task);
    //异步CAN报文发送
//    connect(this,&M_CANWidget::send_Can_Data_Quence,m_can,&M_Can_Task::mctask_sendDataSequence);//,Qt::DirectConnection
    connect(this,&M_CANWidget::send_Can_Data_Quence,m_can,&M_Can_Task::mytask_sendSequence);//,Qt::DirectConnection


    //打开和关闭设备按钮
    connect(ui->toolButton_DevConnect,&QPushButton::clicked,this,&M_CANWidget::pb_Device_Open);

    QObject::connect(ui->m_pAsorDsSortBtn, SIGNAL(clicked()), this, SLOT(OnAsorDsSort()));

    //测试用信号连接
    connect(ui->lineEdit_ID,SIGNAL(textEdited(const QString)),this,SLOT(OnText_Filter_Edit(const QString)));
    connect(ui->lineEdit_Time,SIGNAL(textEdited(const QString)),this,SLOT(OnText_Filter_Edit(const QString)));
    connect(ui->lineEdit_Canid,SIGNAL(textEdited(const QString)),this,SLOT(OnText_Filter_Edit(const QString)));
    connect(ui->lineEdit_Candata,SIGNAL(textEdited(const QString)),this,SLOT(OnText_Filter_Edit(const QString)));
    connect(ui->comboBox_Dir,SIGNAL(currentIndexChanged(const QString)),this,SLOT(OnText_Filter_Edit(const QString)));

    connect(m_can,&M_Can_Task::mctask_0nSqlShow,&data,&M_CanDataSql::mcd_sqladd);
    connect(m_can,&M_Can_Task::mctask_Rx_Variant,&data,&M_CanDataSql::mcd_sqladd_candata_Variant);

    //connect(this,&M_CANWidget::test_send,m_can,&M_Can_Task::mctask_receiveElement);


    connect(ui->toolButton_save,&QToolButton::clicked,this,&M_CANWidget::UI_data_Save);
    connect(ui->toolButton_stop,&QToolButton::clicked,this,&M_CANWidget::UI_data_Stop);
    connect(ui->toolButton_clear,&QToolButton::clicked,this,&M_CANWidget::UI_data_clear);

    connect(ui->toolButton_device,&QToolButton::clicked,this,&M_CANWidget::UI_device_widget_show);
    connect(ui->toolButton_Can_send,&QToolButton::clicked,this,&M_CANWidget::UI_Can_send_widget_show);

    connect(&projectwindow,&ProjectWindow::mw_Send_Can_data_signal,m_can,&M_Can_Task::mytask_sendSequence);

#pragma endregion}
    //异步发送测试函数
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        qDebug()<< "You clicked the pushButton:QPushButton Quence Send!";

        //QVariant cd

        MyClass_Candata dt;
        bool ok;
        int int_id = 0;
        uint8_t data_arr [8] = {0}; // 声明一个uint8_t类型的数组，用于存储转换后的数值
        QString str_data = ui->lineEdit_candata->text();
        int_id = ui->lineEdit_canid->text().toInt(&ok,16);
        if(ok)
        {
            dt.id = int_id;
        }
        else
        {
            dt.id = 0;
        }
        dt.time = 0;
        dt.dir  = 0;

        QStringList str_data_list = str_data.split(" "); // 使用空格作为分隔符，将字符串分割成8个子字符串

        if(str_data_list.count()>=1)
        {
            bool data_ok; // 声明一个bool变量，用于检查转换是否成功
            for (int i = 0; i < str_data_list.count(); i ++) { // 遍历8个子字符串
                data_arr [i] = str_data_list [i].toInt(&data_ok, 16); // 使用16进制作为基数，将子字符串转换为uint8_t类型的数值，并赋值给数组
                if (!data_ok) { // 如果转换失败，打印错误信息，并退出循环
                    qDebug() << "Invalid input: " << str_data_list [i];
                    break;
                }
            }
        }
        memcpy(dt.data,data_arr,8);
        QVariant var = QVariant::fromValue(dt);
        emit send_Can_Data_Quence(var);
    });
    //阻塞发送测试函数
    connect(ui->pushButton_2,&QPushButton::clicked,this,[=](){
        qDebug()<< "You clicked the button——Block Send!";
//        uint8_t data[8] = {00, 00, 00, 00, 00, 00, 00, 00};
//        QByteArray byteArray(reinterpret_cast<char*>(data), sizeof(data));
//        can_dispatcher->can_rx_show_hash.insert(0x65c,byteArray);
        qDebug()<< "You clicked the pushButton:QPushButton Quence Send!";

        //QVariant cd

        MyClass_Candata dt;
        bool ok;
        int int_id = 0;
        uint8_t data_arr [8] = {0}; // 声明一个uint8_t类型的数组，用于存储转换后的数值
        QString str_data = ui->lineEdit_candata->text();
        int_id = ui->lineEdit_canid->text().toInt(&ok,16);
        if(ok)
        {
            dt.id = int_id;
        }
        else
        {
            dt.id = 0;
        }
        dt.time = 0;
        dt.dir  = 0;

        QStringList str_data_list = str_data.split(" "); // 使用空格作为分隔符，将字符串分割成8个子字符串

        if(str_data_list.count()>=1)
        {
            bool data_ok; // 声明一个bool变量，用于检查转换是否成功
            for (int i = 0; i < str_data_list.count(); i ++) { // 遍历8个子字符串
                data_arr [i] = str_data_list [i].toInt(&data_ok, 16); // 使用16进制作为基数，将子字符串转换为uint8_t类型的数值，并赋值给数组
                if (!data_ok) { // 如果转换失败，打印错误信息，并退出循环
                    qDebug() << "Invalid input: " << str_data_list [i];
                    break;
                }
            }
        }
        memcpy(dt.data,data_arr,8);
        m_can->mct_DataTransmitBlock(dt.id,dt.data,8);

    });
    #pragma region "将数据库在新的线程 脱离主UI线程" {
//    //新建一个线程对象
//    QThread * my_SQL_Thread = new QThread(this);
//    //使用movetothread 方法，m_can_task类中的方法将在新的线程中执行
//    data.moveToThread(my_SQL_Thread);
//    my_SQL_Thread->start();
#pragma endregion}

    data.mcd_sqlconnect();
    m_sql_sectimer  = new QTimer;
    connect(m_sql_sectimer,&QTimer::timeout,this,[=](){
        if(m_pSqlTableModel!= NULL){
            if(ui->toolButton_DevConnect->text()=="断开连接"){
            m_pSqlTableModel->select();
            if(ui->checkBox->isChecked())
            {
                //lj QT 这里有bug 会卡在256行
                //ui->tableView->scrollToBottom(); //让滚动条滚动到最顶部
                // 获取模型
                while(m_pSqlTableModel->canFetchMore())
                {
                    m_pSqlTableModel->fetchMore();
                }
                ui->tableView->scrollToBottom();
            }
            }
        }
    });
    m_sql_sectimer->start(200);
    set_tablemodel_ui();
    projectwindow.show();
}

M_CANWidget::~M_CANWidget()
{

    delete(m_can);
    my_Can_Thread->quit();
    my_Can_Thread->wait();

    delete(my_Can_Thread);
    delete(m_pSqlTableModel);

    can_dispatcher->mc_dispatcher_Timer->stop();
    delete can_dispatcher;
    my_Can_dispatcher_Thread->quit();
    my_Can_dispatcher_Thread->wait();
    delete my_Can_dispatcher_Thread;
    qDebug()<<"quit M_CANWidget";

    delete m_sql_sectimer;
    delete ui;
}

/** 界面初始化_CAN设置
 * @brief lock_can_settings
 * @brief 暂时先写死了CANII的盒子，CANFD200U等待支持
 * @param Na
 * @param
 * @return 设备打开结果  0 成功  Others :失败
 */
void M_CANWidget::lock_can_settings()
{
    ui->comboBox_devicetype->addItem("ZLG_USBCANII");
    ui->comboBox_devicebaud->addItem("500K");
    ui->comboBox_deviceindex->addItem("I_0");
    ui->comboBox_devicechannel->addItem("通道0");

    ui->comboBox_devicebaud->setEnabled(false);
    ui->comboBox_devicebaud->setEnabled(false);
    ui->comboBox_deviceindex->setEnabled(false);
    ui->comboBox_devicechannel->setEnabled(false);

    QStringList list_comboBox_Dir;
    list_comboBox_Dir<<"全部方向"<<"Rx"<<"TX";
    ui->comboBox_Dir->addItems(list_comboBox_Dir);

}

/** 过滤 报文条件修改
 * @brief OnTextEdit
 * @param  设置报文显示 升序还是降序排列
 * @param
 * @return 设备打开结果  0 成功  Others :失败
 */
void M_CANWidget::OnText_Filter_Edit(const QString &/*strText*/)
{

    QString str_candata = ui->lineEdit_Candata->text();
    QString str_canid = ui->lineEdit_Canid->text();
    QString str_ID = ui->lineEdit_ID->text();
    QString str_cantime=ui->lineEdit_Time->text();
    QString str_dir = ui->comboBox_Dir->currentText();
    QString str_filltercmd;
    m_pSqlTableModel->setTable("tbl_staff");
    if(!str_candata.isEmpty())
    {
        str_filltercmd.append(QString("candata like '%%1%' ").arg(str_candata));
    }
    if(!str_canid.isEmpty()){
        if(str_filltercmd.isEmpty()) str_filltercmd.append(QString(" canid like '%%1%'").arg(str_canid));
        else str_filltercmd.append(QString("AND canid like '%%1%'").arg(str_canid));
    }
    if(!str_ID.isEmpty()){
        if(str_filltercmd.isEmpty()) str_filltercmd.append(QString(" id like '%%1%'").arg(str_ID));
        else str_filltercmd.append(QString("AND id like '%%1%'").arg(str_ID));
    }
    if(!str_cantime.isEmpty()){
        if(str_filltercmd.isEmpty()) str_filltercmd.append(QString(" rttime like '%%1%'").arg(str_cantime));
        else str_filltercmd.append(QString("AND rttime like '%%1%'").arg(str_cantime));
    }
    if(str_dir!="全部方向"){
        if(str_filltercmd.isEmpty()) str_filltercmd.append(QString(" dir like '%%1%'").arg(str_dir));
        else str_filltercmd.append(QString("AND dir like '%%1%'").arg(str_dir));
    }
    if(!str_filltercmd.isEmpty())
    {
        m_pSqlTableModel->setFilter(str_filltercmd);
    }
    m_pSqlTableModel->select();
//    if (strText.toInt() > 1 && strText.toInt() < 109999)
//    {
//        // 按数字查询
//        m_pSqlTableModel->setFilter(QString("id = %1").arg(strText.toInt()));
//        m_pSqlTableModel->select();
//        // 表头
////        InitHeaderTbl(m_pSqlTableModel);
//    }
//    else if((strText.at(0) >= 'a' && strText.at(0) <= 'z') ||
//             (strText.at(0) >= 'A' && strText.at(0) <= 'Z'))
//    {
//        // 按字母查询
//        m_pSqlTableModel->setFilter(QString("name like '%1%' ").arg(strText));
//        m_pSqlTableModel->select();
//        // 表头
////        InitHeaderTbl(m_pSqlTableModel);
//    }
//    else
//    {
//        // 删除表中原有内容
////        DelContentTbl(m_pSqlTableModel);
//    }
    m_pSqlTableModel->select();
    // 设置默认宽度 data
    //    ui->tableView->setColumnWidth(1, 0);
    ui->tableView->setColumnWidth(0, 0);
    ui->tableView->setColumnWidth(1, 90);
    ui->tableView->setColumnWidth(2, 120);
    ui->tableView->setColumnWidth(3, 60);
    ui->tableView->setColumnWidth(4, 300);
    qDebug()<<"筛选条件修改"<<str_filltercmd;
}

/** 打开、关闭设备
 * @brief pb_Device_Open
 * @param  设置报文显示 升序还是降序排列
 * @param
 * @return 设备打开结果  0 成功  Others :失败
 */
void M_CANWidget::pb_Device_Open()
{
    int ret ;
    if(ui->toolButton_DevConnect->text()=="连接并启动")
        {
        ret = m_can->open_device();
        if(ret == 0)
        {
            ui->toolButton_DevConnect->setText("断开连接");
            m_can->mct_Rx_timer.start(100);
            my_Can_Thread->start();
        }
    }
    else
    {
        m_can->mct_close_device();
        ui->toolButton_DevConnect->setText("连接并启动");
        m_can->mct_Rx_timer.stop();
        if(my_Can_Thread->isRunning())
        {
            my_Can_Thread->quit();
            my_Can_Thread->wait();
        }
    }
}

/** 设置报文显示 升序还是降序排列
 * @brief OnAsorDsSort
 * @param  设置报文显示 升序还是降序排列
 * @param
 * @return 设备打开结果  0 成功  Others :失败
 */
void M_CANWidget::OnAsorDsSort()
{
    if(ui->m_pAsorDsSortBtn->text()=="升序")
    {
        m_pSqlTableModel->setSort(0, Qt::AscendingOrder); //id属性即第0列，升序排列
        m_pSqlTableModel->select();
        ui->m_pAsorDsSortBtn->setText("降序");
    }
    else
    {
        m_pSqlTableModel->setSort(0, Qt::DescendingOrder); //id属性即第0列，降序排列
        m_pSqlTableModel->select();
        ui->m_pAsorDsSortBtn->setText("升序");
    }

}

/** 设置ui默认显示
 * @brief set_tablemodel_ui
 * @param  设置报文显示 升序还是降序排列
 * @param
 * @return 设备打开结果  0 成功  Others :失败
 */
void M_CANWidget::set_tablemodel_ui()
{
    // 设置垂直头不可见
    ui->tableView->verticalHeader()->setVisible(false);
    // 设置序列状态表不可编辑
    //ui->m_pSqlQueryTblView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 设置表格宽度为等宽格式(表头可拖动)
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    // 设置表格宽度为等宽格式(表头不可拖动)
    //ui->m_pSqlQueryTblView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 设置整行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 显示水平和垂直滚动条
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


//    ui->tableView->verticalHeader()->setSortIndicatorShown(true);
//    ui->tableView->verticalHeader()->setSortIndicator(0,Qt::DescendingOrder);
    ui->tableView->verticalHeader()->show();



    // 设置默认宽度
    //需要在设置模型后设置
    //ui->tableView->setColumnWidth(3,300);


    // 设置为不可编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    QSqlQueryModel *model = new QSqlQueryModel;
//    model->setQuery("select * from table where id in (select max(id) from table group by name)");

    m_pSqlTableModel = new QSqlTableModel(this);
    // 指定表名
    m_pSqlTableModel->setTable("tbl_staff");
    // 编辑保存策略
    m_pSqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    // 按降序排列
    m_pSqlTableModel->setSort(0, Qt::AscendingOrder); //id属性即第0列，排列
    // 查询整张表
    m_pSqlTableModel->select();
    // 表头
//    InitHeaderTbl(m_pSqlTableModel);
    m_pSqlTableModel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("序号"));
    m_pSqlTableModel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("时间标识"));
    m_pSqlTableModel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("帧ID 0x"));
    m_pSqlTableModel->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("数据"));
    m_pSqlTableModel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("方向"));

     // 显示
    ui->tableView->setModel(m_pSqlTableModel);

    // 设置默认宽度 data
//    ui->tableView->setColumnWidth(1, 0);
    ui->tableView->setColumnWidth(0, 0);
    ui->tableView->setColumnWidth(1, 90);
    ui->tableView->setColumnWidth(2, 120);
    ui->tableView->setColumnWidth(3, 60);
    ui->tableView->setColumnWidth(4, 300);



      QStringList str_sqldata_limit = {"10000","50000","100000","不做限制"};
    ui->comboBox_sqldatalimit->addItems(str_sqldata_limit);

}
void M_CANWidget::UI_data_Save()
{
    //以下是保存csv
    int row,col,i,j;
    QFile file;
    QString fileName;
    QModelIndex index;
    QDateTime time =QDateTime::currentDateTime();
    QString current_date =time.toString("yyyyMMddhhmmsszzzddd");

    row = ui->tableView->model()->rowCount();          //获取当前tableview行列数
    col = ui->tableView->model()->columnCount();
    fileName = QFileDialog::getSaveFileName(this,
                                            tr("保存表格"),
                                            QDir::currentPath() + "/" +\
                                            current_date + ".csv",
                                            tr("csv File(*.csv)"));
    if(fileName.isNull())
    {
        return;
    }

    file.setFileName(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"open file fail";
        return;
    }
    QTextStream out(&file);             //创建一个文本流，向保存文件中写入文本

    //写表头
    for(i = 0;i < col; i++)
    {
        out<<ui->tableView->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()<<",";
        if(i == (col - 1))
        {
            out<<"\n";
        }
    }
    //写数据
    for(i = 0;i < row; i++)
    {
        for(j = 0;j < col; j++)
        {
            index = ui->tableView->model()->index(i,j);
            out<<ui->tableView->model()->data(index).toString()<<",";
            if(j == (col - 1))
            {
                out<<"\n";
            }
        }
    }
    file.close();
}
void M_CANWidget::UI_data_Stop()
{
    static bool stop = false;
    if(stop)
    {
        ui->toolButton_stop->setDown(0);
        stop = false;
        m_sql_sectimer->start(200);
    }
    else
    {
        stop = true;
        ui->toolButton_stop->setDown(1);
        m_sql_sectimer->stop();
    }
}
void M_CANWidget::UI_data_clear()
{
     #pragma region "直接暴力clear会导致ui卡顿，舍弃" {
    //        while (m_pSqlTableModel->canFetchMore ())
    //        {
    //            m_pSqlTableModel->fetchMore ();
    //        }
    //        int iRowCount = m_pSqlTableModel->rowCount();

    //        m_pSqlTableModel->removeRows(0,iRowCount);
    //        m_pSqlTableModel->submitAll();
     #pragma endregion}
    //据说这样会破坏数据库的性能，暂时未发现明显bug
    //:TODO 这里的清空数据表需要优化
    m_pSqlTableModel->query().exec("DELETE FROM tbl_staff");
    m_pSqlTableModel->select();
}
void M_CANWidget::UI_device_widget_show()
{
    static uint8_t show = 1;
    if(show)
    {
        ui->groupBox_device->hide();
        show = 0;
    }
    else
    {
        ui->groupBox_device->show();
        show = 1;
    }

}
void M_CANWidget::UI_Can_send_widget_show()
{
    static uint8_t show = 1;
    if(show)
    {
        ui->groupBox_Can_send->hide();
        show = 0;
    }
    else
    {
        ui->groupBox_Can_send->show();
        show = 1;
    }
}
