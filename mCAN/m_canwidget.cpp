#include "m_canwidget.h"
#include "ui_m_canwidget.h"

#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QMetaType>

#include "m_candatabase.h"



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
