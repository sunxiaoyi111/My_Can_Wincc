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
    //CAN收发任务初始化
    can = new M_Can_Task;
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        qDebug()<< "You clicked the button!";
        can->open_device_test();
    });

    //链接默认的按钮和信号与槽





}

M_CANWidget::~M_CANWidget()
{
    delete ui;
    delete(can);
}

