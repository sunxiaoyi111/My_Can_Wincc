#ifndef CAN_DISPATCHER_H
#define CAN_DISPATCHER_H

#include "m_candatabase.h"
#include <QObject>
#include <QMap>
#include <QQueue>
#include <QVariant>
#include <QTimer>
#include "m_candatabase.h"
#include <QHash>
struct can_rx_data_type{
//    observer_callback_type cb;
//    enum can_observer_type type;
    uint32_t sb;
    uint32_t eb;
//    uint32_t mask;
    uint32_t mask;
    QByteArray data;
};

//Q_DECLARE_METATYPE(can_rx_data_type)

class Can_dispatcher : public QObject
{
    Q_OBJECT
public:
    Can_dispatcher();
    ~Can_dispatcher();
    QTimer *mc_dispatcher_Timer;
    QQueue<MyClass_Candata> mcdis_Rx;
    QHash<int,QByteArray> can_rx_show_hash;
public slots:
    bool mc_candispatcher_Variant(QVariant cd);
    void mc_candispatcher_Task();
};

#endif // CAN_DISPATCHER_H
