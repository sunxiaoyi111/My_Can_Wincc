#ifndef M_CAN_TASK_H
#define M_CAN_TASK_H
#include <QObject>

#include <QDebug>
#include <QThread>
#include <QMessageBox>

#include "canlib/zlgcan/zlgcan_x64/zlgcan.h"
#include "canlib/zlgcan/zlgcan_x64/canframe.h"
#include "canlib/zlgcan/zlgcan_x64/config.h"
#include "canlib/zlgcan/zlgcan_x64/typedef.h"



class M_Can_Task : public QObject
{
public:
    M_Can_Task();
    ~M_Can_Task();
    int open_device_test();
    int open_device();
    int mct_close_device();
    void mct_msgBox_Show(QString msg);

    bool isStart;

    DEVICE_HANDLE mct_dhandle;
    CHANNEL_HANDLE mct_chHandle;
    ZCAN_Transmit_Data mct_frame;
    ZCAN_CHANNEL_INIT_CONFIG mct_cfg;
    BYTE test_data[8] = {1, 2, 3, 4, 5, 6, 7, 8};



    void mct_DataRecive();

};

#endif // M_CAN_TASK_H
