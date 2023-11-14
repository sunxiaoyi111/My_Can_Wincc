#include "m_canwidget.h"
#include "ui_m_canwidget.h"

#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QMetaType>

#include "m_canconfig.h"


M_CANWidget::M_CANWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::M_CANWidget)
{
    ui->setupUi(this);
    lock_can_settings();
    m_canconfig cfg;
    cfg.m_canconfig_init();

    #pragma region "注册信号与槽 自定义传递变量(QT通用定义未包含需要在这里注册)" {
    qRegisterMetaType<M_CanDataBase> ("M_CanDataBase");
#pragma endregion}
    //CAN收发任务初始化
    m_can = new M_Can_Task;
    //新建一个线程对象
    my_Can_Thread = new QThread(this);
    //使用movetothread 方法，m_can_task类中的方法将在新的线程中执行
    m_can->moveToThread(my_Can_Thread);
    #pragma region "信号与槽函数初始化" {
    connect(&m_can->mct_Rx_timer,&QTimer::timeout,m_can,&M_Can_Task::mct_Recurring_Task);
    //异步CAN报文发送
    connect(this,&M_CANWidget::send_Can_Data_Quence,m_can,&M_Can_Task::mctask_sendDataSequence);//,Qt::DirectConnection
    //打开和关闭设备按钮
    connect(ui->toolButton_DevConnect,&QPushButton::clicked,this,&M_CANWidget::pb_Device_Open);

    QObject::connect(ui->m_pAsorDsSortBtn, SIGNAL(clicked()), this, SLOT(OnAsorDsSort()));

    //测试用信号连接
    connect(ui->lineEdit_ID,SIGNAL(textEdited(const QString)),this,SLOT(OnText_Filter_Edit(const QString)));
    connect(ui->lineEdit_Time,SIGNAL(textEdited(const QString)),this,SLOT(OnText_Filter_Edit(const QString)));
    connect(ui->lineEdit_Canid,SIGNAL(textEdited(const QString)),this,SLOT(OnText_Filter_Edit(const QString)));
    connect(ui->lineEdit_Candata,SIGNAL(textEdited(const QString)),this,SLOT(OnText_Filter_Edit(const QString)));


    connect(m_can,&M_Can_Task::mctask_0nSqlShow,&data,&M_CanDataSql::mcd_sqladd);
    connect(m_can,&M_Can_Task::mctask_Rx,&data,&M_CanDataSql::mcd_sqladd_candata);
    connect(m_can,&M_Can_Task::mctask_Rx_Variant,&data,&M_CanDataSql::mcd_sqladd_candata_Variant);

    //connect(this,&M_CANWidget::test_send,m_can,&M_Can_Task::mctask_receiveElement);
    #pragma endregion}
    //异步发送测试函数
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        qDebug()<< "You clicked the pushButton!";
        static M_CanDataBase dt;
        dt.id = 0x01;
        uint8_t data[8]= {0,1,2,3,4,5,6,7};
        dt.SetData(1,data,8,0,0);
        emit send_Can_Data_Quence(dt);
    });
    //阻塞发送测试函数
    connect(ui->pushButton_2,&QPushButton::clicked,this,[=](){
        qDebug()<< "You clicked the button2!";
    });
    #pragma region "将数据库在新的线程 脱离主UI线程" {
//    //新建一个线程对象
//    QThread * my_SQL_Thread = new QThread(this);
//    //使用movetothread 方法，m_can_task类中的方法将在新的线程中执行
//    data.moveToThread(my_SQL_Thread);
//    my_SQL_Thread->start();
#pragma endregion}

    data.mcd_sqlconnect();
    QTimer * m_sql_sectimer  = new QTimer;
    connect(m_sql_sectimer,&QTimer::timeout,this,[=](){
        //        qDebug()<< "timeout!";
        if(m_pSqlTableModel!= NULL){
            if(ui->toolButton_DevConnect->text()=="断开连接"){
            m_pSqlTableModel->select();
            if(ui->checkBox->isChecked())
            ui->tableView->scrollToTop(); //让滚动条滚动到最顶部
            }
        }
    });
    m_sql_sectimer->start(200);
    set_tablemodel_ui();
}

M_CANWidget::~M_CANWidget()
{
    delete ui;
    delete(m_can);
    delete(my_Can_Thread);
    delete(m_pSqlTableModel);
}

void M_CANWidget::test_receive(M_CanDataBase element)
{
    data.mcd_sqladd();
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
    if(!str_filltercmd.isEmpty())
    {
        m_pSqlTableModel->setFilter(str_filltercmd);
    }
    m_pSqlTableModel->select();
    return ;
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
    if(ui->m_pAsorDsSortBtn->text()=="按id升序排列")
    {
        m_pSqlTableModel->setSort(0, Qt::AscendingOrder); //id属性即第0列，升序排列
        m_pSqlTableModel->select();
        ui->m_pAsorDsSortBtn->setText("按降序排列");
    }
    else
    {
        m_pSqlTableModel->setSort(0, Qt::DescendingOrder); //id属性即第0列，降序排列
        m_pSqlTableModel->select();
        ui->m_pAsorDsSortBtn->setText("按id升序排列");
    }

}

void M_CANWidget::set_tablemodel_ui()
{
    // 设置垂直头不可见
    ui->tableView->verticalHeader()->setVisible(false);
    // 设置序列状态表不可编辑
    //ui->m_pSqlQueryTblView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 设置表格宽度为等宽格式(表头可拖动)
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    // 设置表格宽度为等宽格式(表头不可拖动)
    //ui->m_pSqlQueryTblView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 设置整行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 显示水平和垂直滚动条
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    // 设置默认宽度
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
    m_pSqlTableModel->setSort(0, Qt::DescendingOrder); //id属性即第0列，升序排列
    // 查询整张表
    m_pSqlTableModel->select();
    // 表头
//    InitHeaderTbl(m_pSqlTableModel);
    m_pSqlTableModel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("序号"));
    m_pSqlTableModel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("时间标识"));
    m_pSqlTableModel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("帧ID"));
    m_pSqlTableModel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("数据"));
     // 显示
    ui->tableView->setModel(m_pSqlTableModel);
}
