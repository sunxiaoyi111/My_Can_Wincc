#include "projectwindow.h"
#include "ui_projectwindow.h"
#include "Candispatcher/can_dispatcher_table.h"

#include <QDebug>
#include <QTabWidget>
#include <QRegExpValidator>
ProjectWindow::ProjectWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProjectWindow)
{
    ui->setupUi(this);
    this->mw_ui_init();
//    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
//        uint32_t id = 0x123;
//        uint8_t data[8] ={1,2,3,4,5,6,7,8};
//        mw_Send_Can(id,data);
//    });
    this->mw_singal_init_tx();

}

ProjectWindow::~ProjectWindow()
{
    delete ui;
}

void ProjectWindow::mw_ui_init()
{
    mw_labellist[0]=ui->label_test;
}

void ProjectWindow::mw_Send_Can(uint32_t can_id,uint8_t *data)
{
    MyClass_Candata dt;
    dt.id = can_id;
    memcpy(dt.data,data,8);
    static QVariant var_dt = QVariant::fromValue(dt);
    emit this->mw_Send_Can_data_signal(var_dt);
}

void ProjectWindow::mw_Send_Can_cit(uint signa_id)
{
    MyClass_Candata dt;

    dt.id = can_cit_cmd_t[signa_id].id;
    memcpy(dt.data,can_cit_cmd_t[signa_id].data,8);
    QVariant var_dt = QVariant::fromValue(dt);
    emit this->mw_Send_Can_data_signal(var_dt);
}

void ProjectWindow::mw_singal_init_tx()
{
  #pragma region "功能列表切换的功能" {
    connect(ui->tabWidget,&QTabWidget::currentChanged,this,[=](){
        QString mode [2] = {"模拟主机","测试模式"};
        int current_id =  ui->tabWidget->currentIndex();
        QString current_mode = mode[current_id];
        qDebug()<<"功能切换:"<<current_mode;
            if(current_id==0)        mw_Send_Can_cit(0);
            if(current_id==1)        mw_Send_Can_cit(1);
    });
    #pragma endregion}
  #pragma region "预设报文发送的功能" {
    connect(ui->pushButton_F_vol_ctrl,&QPushButton::clicked,this,[=]()
    {
        qDebug()<<"前舱音量控制";
        mw_Send_Can_cit(0);
    });
  #pragma endregion}
  #pragma region "直接输入byte报文的功能" {
    // 创建一个正则表达式对象，匹配xx xx xx xx 这样4字节十六进制的格式
    QRegExp regExp ("^([0-9A-Fa-f]{2} ){3}[0-9A-Fa-f]{2}$");
    // 创建一个验证器对象，使用正则表达式对象作为参数
    QRegExpValidator *validator = new QRegExpValidator (regExp, this);
    // 设置QLineEdit的验证器为验证器对象
    ui->lineEdit_gain_byte4->setValidator (validator);


    connect(ui->pushButton_set_gain,&QPushButton::clicked,this,[=](){
        uint32_t id = 0x6EA;
        uint8_t  dt[8] = {0x10,0x01};
        QString str_data = ui->lineEdit_gain_byte4->text();
        QStringList str_data_list = str_data.split(" "); // 使用空格作为分隔符，将字符串分割成8个子字符串
        uint8_t data_arr[8] = {0};
        if(str_data_list.count()>=1)
        {
            bool data_ok; // 声明一个bool变量，用于检查转换是否成功
            for (int i = 0; i < str_data_list.count(); i ++) { // 遍历8个子字符串
                data_arr [i] = str_data_list [i].toInt(&data_ok, 16); // 使用16进制作为基数，将子字符串转换为uint8_t类型的数值，并赋值给数组
                if (!data_ok) { // 如果转换失败，打印错误信息，并退出循环
                    qDebug() << "Invalid input: " << str_data_list [i];
                    return;
                }
            }
        }
        memcpy(&dt[3],(uint8_t *)data_arr,4);
        mw_Send_Can(id,dt);
    });
    #pragma endregion}
  #pragma region "直接输入数值的功能" {
    // 创建一个QRegExpValidator，限制输入的格式为带小数点的浮点数
    QRegExp regExp_fouble("^\\d+(\\.\\d+)?$");
    QRegExpValidator *validator_double = new QRegExpValidator(regExp_fouble, this);

    // 设置lineEdit的validator
    ui->lineEdit_gain_double->setValidator(validator_double);

    //    另一种方法: 建一个QDoubleValidator，限制输入的范围为0到100之间的浮点数，小数点后最多两位
    //    QDoubleValidator *validator_fouble = new QDoubleValidator(0, 100, 4, this);

    // 设置lineEdit的validator
    ui->lineEdit_gain_double->setValidator(validator_double);


    connect(ui->pushButton_set_gain_double,&QPushButton::clicked,this,[=](){
        uint32_t id = 0x6EA;
        uint8_t  dt[8] = {0x10,0x01,0,0,0,0,0,0};
        float gain = 0;
        union float_bytes db;

        QString str_gain_double = ui->lineEdit_gain_double->text();
        bool is_ok;
        gain=str_gain_double.toFloat(&is_ok);
        if(is_ok){
            db.value = gain;
            memcpy(&dt[3],&db.bytes[0],4);
            mw_Send_Can(id,dt);
        }else{
            qDebug()<<"Invalid input:"<<str_gain_double;
        }
    });
    #pragma endregion}
}

void ProjectWindow::mw_singal_int_rx_ui_list()
{

}

void ProjectWindow::mw_slots_set_label_rx(QVariant si)
{
    P_struct_Rx_Show p_show_str;
    p_show_str=si.value<P_struct_Rx_Show>();
    mw_labellist[p_show_str.id]->setText(p_show_str.str);
}
