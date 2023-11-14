#ifndef M_CAN_TASK_H
#define M_CAN_TASK_H
#include <QObject>

#include <QDebug>
#include <QThread>
#include <QMessageBox>

#include <QTimer>
#include <QQueue>

//Q_DECLARE_METATYPE(M_CanDataBase)

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

    DEVICE_HANDLE mct_dhandle = nullptr;
    CHANNEL_HANDLE mct_chHandle = nullptr;
    ZCAN_Transmit_Data mct_frame = {0};
    ZCAN_CHANNEL_INIT_CONFIG mct_cfg = {0};
    BYTE test_data[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    QQueue<M_CanDataBase> mct_Tx;
    QQueue<M_CanDataBase> mct_Rx;

    QTimer mct_CanRx_timer;


    int mct_DataTransmitBlock(uint32_t can_id,uint8_t * data,uint8_t length);

    // 此函数仅用于构造示例 CAN 报文
    void get_can_frame(ZCAN_Transmit_Data& can_data, canid_t id);
    // 此函数仅用于构造示例 CANFD 报文
    void get_canfd_frame(ZCAN_TransmitFD_Data& canfd_data, canid_t id);

    void mct_Recurring_Task();
    void mct_do_Tx_Task();
    void mct_AddData(const ZCAN_Receive_Data *data, UINT len);
    void mct_AddData(M_CanDataBase *data);
private:


signals:
    void mctask_0nSqlShow();
    void mctask_Rx(M_CanDataBase cd);
    void mctask_Rx_Variant(QVariant cd);
public slots:
    void mctask_receiveElement(M_CanDataBase element);
    void mctask_sendDataSequence(M_CanDataBase element);
};

#endif // M_CAN_TASK_H
