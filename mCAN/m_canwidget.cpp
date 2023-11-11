#include "m_canwidget.h"
#include "ui_m_canwidget.h"

#include <QApplication>
#include <QPushButton>
#include <QMessageBox>

#include "m_candatabase.h"

#include <QMetaType>

M_CANWidget::M_CANWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::M_CANWidget)
{
    ui->setupUi(this);
    qRegisterMetaType<M_CanDataBase> ("M_CanDataBase");
    //链接默认的按钮和信号与槽
    connect(this,&M_CANWidget::sig_in_num,this,&M_CANWidget::ret_any_in);
    //CAN收发任务初始化
    m_can = new M_Can_Task;
    connect(&m_can->mct_Rx_timer,&QTimer::timeout,m_can,&M_Can_Task::mct_do_Rx_Task);

    //新建一个线程对象
    my_Can_Thread = new QThread(this);
    //使用movetothread 方法，m_can_task类中的方法将在新的线程中执行
    m_can->moveToThread(my_Can_Thread);

    connect(m_can,&M_Can_Task::sendElement,this,&M_CANWidget::test_receive);//,Qt::DirectConnection
    connect(this,&M_CANWidget::test_send,m_can,&M_Can_Task::receiveElement);
//    connect(this,&M_CANWidget::StartDataReceive,m_can,&M_Can_Task::mct_DataRecive);
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        qDebug()<< "You clicked the button!";
        M_CanDataBase  msg;
        msg.mcd_canID = 0x100;
        emit this->test_send(msg);
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
//        int ret = 0 ;
//        ret = sig_in_num(10);
//        qDebug()<<ret;
    });



}

M_CANWidget::~M_CANWidget()
{
    delete ui;
    delete(m_can);
}

int M_CANWidget::ret_any_in(int in)
{
    return in;
}

void M_CANWidget::test_receive(M_CanDataBase element)
{
    qDebug()<<element.mcd_canID;

}
