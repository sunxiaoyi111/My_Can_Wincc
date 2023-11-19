#include "can_dispatcher.h"
#include "cansingalbase.h"
#include "can_dispatcher_table.h"

#include <QDebug>

Can_dispatcher::Can_dispatcher()
{
    mc_dispatcher_Timer = new QTimer;
    connect(mc_dispatcher_Timer,&QTimer::timeout,this,&Can_dispatcher::mc_candispatcher_Task);
    mc_dispatcher_Timer->start(100);

    for(uint n=0;n<2;n++)
    {
        can_rx_show_hash.insert(CAN_ID_RECEIVE_TABLE_SUM[n],"");
    }


}
Can_dispatcher::~Can_dispatcher()
{
    delete mc_dispatcher_Timer;
    qDebug()<<"quit2";
}
bool Can_dispatcher::mc_candispatcher_Variant(QVariant cd)
{
    MyClass_Candata data= cd.value<MyClass_Candata>();
    mcdis_Rx.append(data);
    return 0;
}

void Can_dispatcher::mc_candispatcher_Task()
{
    while (!mcdis_Rx.isEmpty())
    {
        MyClass_Candata data = mcdis_Rx.dequeue();
        if(can_rx_show_hash.contains(data.id))
        {
            //can _id 的数据会更新存储到这里
            can_rx_show_hash[data.id] = QByteArray(reinterpret_cast<char*>(data.data), 8);
            mc_can_rx_custom_callback(data.id,data.data);
        }
    }
}
void Can_dispatcher::mc_set_label_str(uint id,QString str)
{
    P_struct_Rx_Show p_show_str;
    p_show_str.id = id;
    p_show_str.str = str;
    QVariant var = QVariant::fromValue(p_show_str);
    emit mc_singal_set_label_str(var);
}


void Can_dispatcher::mc_can_rx_custom_callback(uint id,uint8_t * data)
{
    uint8_t can_data[8] ={0};
    QString str_show ;
    switch(id)
    {
        case 0x6EA:
        for(int i=0;i<8;i++)
        {
            str_show += QString("%1 ").arg(data[i],2,16,QLatin1Char('0'));
        }
        mc_set_label_str(label_test,str_show);
        break;
    }
    qDebug()<<"更新数据ID=:"<<Qt::hex<<id;
}


