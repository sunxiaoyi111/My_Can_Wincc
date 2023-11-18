#include "projectwindow.h"
#include "ui_projectwindow.h"


#include <QDebug>


ProjectWindow::ProjectWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProjectWindow)
{
    ui->setupUi(this);
    this->mw_ui_init();
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        uint32_t id = 0x123;
        uint8_t data[8] ={1,2,3,4,5,6,7,8};
        mw_Send_Can(id,data);
    });
}

ProjectWindow::~ProjectWindow()
{
    delete ui;
}

void ProjectWindow::mw_ui_init()
{

}

void ProjectWindow::mw_Send_Can(uint32_t can_id,uint8_t *data)
{
    MyClass_Candata dt;
    dt.id = can_id;
    memcpy(dt.data,data,8);
    static QVariant var_dt = QVariant::fromValue(dt);
    emit this->mw_Send_Can_data_signal(var_dt);
}
