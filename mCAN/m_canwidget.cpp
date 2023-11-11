#include "m_canwidget.h"
#include "ui_m_canwidget.h"

#include <QApplication>
#include <QPushButton>
#include <QMessageBox>

M_CANWidget::M_CANWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::M_CANWidget)
{
    ui->setupUi(this);
    //链接默认的按钮和信号与槽
    connect(this,&M_CANWidget::sig_in_num,this,&M_CANWidget::ret_any_in);
    //CAN收发任务初始化
    m_can = new M_Can_Task;


    //新建一个线程对象
    my_Can_Thread = new QThread(this);
    //使用movetothread 方法，m_can_task类中的方法将在新的线程中执行
    m_can->moveToThread(my_Can_Thread);



    connect(this,&M_CANWidget::StartDataReceive,m_can,&M_Can_Task::mct_DataRecive);




    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        qDebug()<< "You clicked the button!";
//        can->open_device_test();
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
                my_Can_Thread->start();
                m_can->isStart = true;
                 emit this->StartDataReceive();
            }

        }
        else
        {
            m_can->mct_close_device();
            ui->toolButton_DevConnect->setText("连接并启动");
            m_can->isStart = false;
            if(my_Can_Thread->isRunning())
            {
//                 CanSet->CanClose();
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
