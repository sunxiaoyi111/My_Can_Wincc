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
    connect(this,&M_CANWidget::test_send,m_can,&M_Can_Task::mctask_sendDataSequence);//,Qt::DirectConnection


    //测试用信号连接
    connect(m_can,&M_Can_Task::mctask_sendElement,this,&M_CANWidget::test_receive);//,Qt::DirectConnection
    connect(ui->lineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(OnTextEdit(const QString&)));//,Qt::DirectConnection

    //lineEdit
//connect(this,&M_CANWidget::test_send,m_can,&M_Can_Task::mctask_receiveElement);
#pragma endregion}
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        qDebug()<< "You clicked the button!";
        M_CanDataBase  msg;
        msg.mcd_canID = 0x100;
        for(int i =0;i<100;i++){
            emit this->test_send(msg);
        }

    });
    connect(ui->pushButton_2,&QPushButton::clicked,this,[=](){
        qDebug()<< "You clicked the button2!";


//        M_CanDataBase  msg;
//        msg.mcd_canID = 0x100;
//        uint8_t data[8] = {1,2,3,4,5,6,7,8};
//        for(int i =0;i<100;i++){
//            if(m_can->mct_DataTransmitBlock(msg.mcd_canID,data,8)==STATUS_ERR);
//            {
//                m_can->mct_msgBox_Show("报文发送失败");
//                break;
//            }
//        }

    });
    connect(ui->toolButton_DevConnect,&QPushButton::clicked,this,[=](){
        qDebug()<<"connect";
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
            if(my_Can_Thread->isRunning())
            {
                 my_Can_Thread->quit();
                 my_Can_Thread->wait();
            }
        }
    });

    m_canconfig cfg;
    cfg.m_canconfig_init();

    //新建一个线程对象
//    QThread * my_SQL_Thread = new QThread(this);
//    //使用movetothread 方法，m_can_task类中的方法将在新的线程中执行
//    data.moveToThread(my_SQL_Thread);
//    my_SQL_Thread->start();

    data.mcd_sqlconnect();

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

//    QSqlQueryModel *model = new QSqlQueryModel;
//    model->setQuery("select * from table where id in (select max(id) from table group by name)");

    m_pSqlTableModel = new QSqlTableModel(this);
    // 指定表名
    m_pSqlTableModel->setTable("tbl_staff");
    // 编辑保存策略
    m_pSqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    // 查询整张表
    m_pSqlTableModel->select();
    // 表头
//    InitHeaderTbl(m_pSqlTableModel);
    m_pSqlTableModel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("工号"));
    m_pSqlTableModel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("员工"));
    m_pSqlTableModel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("工龄"));
    m_pSqlTableModel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("部门"));
     // 显示
    ui->tableView->setModel(m_pSqlTableModel);
}

M_CANWidget::~M_CANWidget()
{
    delete ui;
    delete(m_can);
    delete(my_Can_Thread);
}

void M_CANWidget::test_receive(M_CanDataBase element)
{
    qDebug()<<element.mcd_canID;
    data.mcd_sqladd();

//    m_pSqlTableModel = new QSqlTableModel(this);
//    // 指定表名
//    m_pSqlTableModel->setTable("tbl_staff");
//    // 编辑保存策略
//    m_pSqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    // 查询整张表
    m_pSqlTableModel->select();
//    // 表头
//    //    InitHeaderTbl(m_pSqlTableModel);
//    m_pSqlTableModel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("工号"));
//    m_pSqlTableModel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("员工"));
//    m_pSqlTableModel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("工龄"));
//    m_pSqlTableModel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("部门"));
//    // 显示
//    ui->tableView->setModel(m_pSqlTableModel);
}
/**
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
}
void M_CANWidget::OnTextEdit(const QString &/*strText*/)
{
    QString strText = ui->lineEdit->text();

    if (strText.isEmpty())
    {
        // 查找表中所有信息
        m_pSqlTableModel->setTable("tbl_staff");
        m_pSqlTableModel->select();
        // 表头
//        InitHeaderTbl(m_pSqlTableModel);
    }
    else if (strText.toInt() > 1 && strText.toInt() < 1099)
    {
        // 按数字查询
        m_pSqlTableModel->setFilter(QString("id = %1").arg(strText.toInt()));
        m_pSqlTableModel->select();
        // 表头
//        InitHeaderTbl(m_pSqlTableModel);
    }
    else if((strText.at(0) >= 'a' && strText.at(0) <= 'z') ||
             (strText.at(0) >= 'A' && strText.at(0) <= 'Z'))
    {
        // 按字母查询
        m_pSqlTableModel->setFilter(QString("name like '%1%' ").arg(strText));
        m_pSqlTableModel->select();
        // 表头
//        InitHeaderTbl(m_pSqlTableModel);
    }
    else
    {
        // 删除表中原有内容
//        DelContentTbl(m_pSqlTableModel);
    }
}
