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
    can = new M_Can_Task;
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        qDebug()<< "You clicked the button!";
        can->open_device_test();
    });
}

M_CANWidget::~M_CANWidget()
{
    delete ui;
    delete(can);
}

