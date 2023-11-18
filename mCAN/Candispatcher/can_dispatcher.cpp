#include "can_dispatcher.h"
#include "can_dispatcher_table.h"

#include <QDebug>

Can_dispatcher::Can_dispatcher()
{
    mc_dispatcher_Timer = new QTimer;
    connect(mc_dispatcher_Timer,&QTimer::timeout,this,&Can_dispatcher::mc_candispatcher_Task);
    mc_dispatcher_Timer->start(10);
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
            can_rx_show_hash[data.id] = QByteArray(reinterpret_cast<char*>(data.data), 8);
        }
    }
}
