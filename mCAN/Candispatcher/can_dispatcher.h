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
#include <QList>
#include "canmessagebase.h"
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
//    QList <cansingalbase> mc_can_singal_List;
    QTimer *mc_dispatcher_Timer;
    QQueue<MyClass_Candata> mcdis_Rx;
    QHash<int,QByteArray> can_rx_show_hash;
    void mc_can_rx_custom_callback(uint id,uint8_t * data);
signals:
    void mc_singal_set_label_str(QVariant si);
public slots:
    bool mc_candispatcher_Variant(QVariant cd);
    void mc_candispatcher_Task();
private:
    void mc_set_label_str(uint id,QString str);
};

#endif // CAN_DISPATCHER_H
