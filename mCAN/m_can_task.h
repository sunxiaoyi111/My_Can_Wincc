#ifndef M_CAN_TASK_H
#define M_CAN_TASK_H

#include <QDebug>
#include <QThread>


#include "canlib/zlgcan/zlgcan_x64/zlgcan.h"
#include "canlib/zlgcan/zlgcan_x64/canframe.h"
#include "canlib/zlgcan/zlgcan_x64/config.h"
#include "canlib/zlgcan/zlgcan_x64/typedef.h"



class M_Can_Task
{
public:
    M_Can_Task();
    ~M_Can_Task();
    int open_device_test();
};

#endif // M_CAN_TASK_H
