#ifndef M_CAN_TASK_H
#define M_CAN_TASK_H
#include <QObject>

#include <QDebug>
#include <QThread>
#include <QMessageBox>

#include <QTimer>
#include <QQueue>

#include "canlib/zlgcan/zlgcan_x64/zlgcan.h"
#include "canlib/zlgcan/zlgcan_x64/canframe.h"
#include "canlib/zlgcan/zlgcan_x64/config.h"
#include "canlib/zlgcan/zlgcan_x64/typedef.h"

#include "m_candatabase.h"


class M_Can_Task : public QObject
{
     Q_OBJECT // 添加Q_OBJECT宏
public:
    M_Can_Task();
    ~M_Can_Task();
    QTimer mct_Rx_timer;


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


    QQueue<M_CanDataBase> mct_Tx;
    QQueue<M_CanDataBase> mct_Rx;

    int mct_DataTransmitBlock(uint32_t can_id,uint8_t * data,uint8_t length);
    void mct_DataRecive();

    // 此函数仅用于构造示例 CAN 报文
    void get_can_frame(ZCAN_Transmit_Data& can_data, canid_t id);
    // 此函数仅用于构造示例 CANFD 报文
    void get_canfd_frame(ZCAN_TransmitFD_Data& canfd_data, canid_t id);

    void mct_do_Rx_Task();
    void mct_do_Tx_Task();

private:

//用来测试两个线程的队列交互
signals:
    void sendElement(M_CanDataBase element);
public slots:
    void receiveElement(M_CanDataBase element);
};

#endif // M_CAN_TASK_H
