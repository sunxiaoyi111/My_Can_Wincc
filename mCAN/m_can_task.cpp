#include "m_can_task.h"
M_Can_Task::M_Can_Task()
{
    qDebug()<<"init";
}

M_Can_Task::~M_Can_Task()
{
    qDebug()<<"quit";
}
/**
 * @brief 打开CAN盒 锁定为CAN II 波特率500K 通道0
 * @param Na
 * @param
 * @return 设备打开结果  0 成功  Others :失败
 */
int M_Can_Task::open_device()
{
    //ZCAN_USBCAN_2E_U 为设备类型, 请根据实际修改
    mct_dhandle = ZCAN_OpenDevice(ZCAN_USBCAN2, 0, 0);
    if (INVALID_DEVICE_HANDLE == mct_dhandle)
    {
        mct_msgBox_Show("打开设备失败\n请检查CAN盒子是否连接");
        return 1;
    }
    //CAN 设备设置波特率的 key 为 baud_rate，值 1000000 为 1000kbps, 800000 为 800kbps, 其它请查看
    //属性表
    //若为 CANFD 设备, 设置冲裁域波特率的 key 为 canfd_abit_baud_rate，数据域波特率为
    //canfd_dbit_baud_rate，请注意区分 CAN 和 CANFD 设备设置波特率的区别。
    if (ZCAN_SetValue (mct_dhandle , "0/baud_rate", "1000000") != STATUS_OK)
    {
        mct_msgBox_Show("设置波特率失败\n已断开CAN盒连接");
        goto end;
    }

    memset(&mct_cfg, 0, sizeof(mct_cfg));
    mct_cfg.can_type = TYPE_CAN;//CANFD 设备为 TYPE_CANFD
    mct_cfg.can.filter = 0;
    mct_cfg.can.mode = 0; //正常模式, 1 为只听模式
    mct_cfg.can.acc_code = 0;
    mct_cfg.can.acc_mask = 0xffffffff;
    mct_chHandle = ZCAN_InitCAN(mct_dhandle, 0, &mct_cfg);
    if (INVALID_CHANNEL_HANDLE == mct_chHandle)
    {
        mct_msgBox_Show("启动通道失败\n已断开CAN盒连接");
        goto end;
    }
    if (ZCAN_StartCAN(mct_chHandle) != STATUS_OK)
    {
        mct_msgBox_Show("启动通道失败\n已断开CAN盒连接");
        goto end;
    }

//    memset(&mct_frame, 0, sizeof(mct_frame));
//    mct_frame.frame.can_id = MAKE_CAN_ID(0x100, 1, 0, 0);
//    mct_frame.frame.can_dlc = 8;

//    memcpy(mct_frame.frame.data, test_data, sizeof(test_data));
//    if (ZCAN_Transmit(mct_chHandle, &mct_frame, 1) != 1)
//    {
//        mct_msgBox_Show("发送数据失败\n已断开CAN盒连接");
//        goto end;
//    }
    return 0 ;
end:
    ZCAN_CloseDevice(mct_dhandle);
    qDebug()<<"end";
    return 1;
}

/**
 * @brief 打开CAN盒 锁定为CAN II 波特率500K 通道0
 * @brief 仅测试用
 * @param Na
 * @param
 * @return 设备打开结果  0 成功  Others :失败
 */
int M_Can_Task::open_device_test()
{
    //将定义都拿到了开头 为了能够正常的使用goto语句
    DEVICE_HANDLE dhandle;
    CHANNEL_HANDLE chHandle;
    ZCAN_Transmit_Data frame;
    ZCAN_CHANNEL_INIT_CONFIG cfg;
    BYTE data[] = {1, 2, 3, 4, 5, 6, 7, 8};

    //ZCAN_USBCAN_2E_U 为设备类型, 请根据实际修改
    dhandle = ZCAN_OpenDevice(ZCAN_USBCAN_2E_U, 0, 0);
    if (INVALID_DEVICE_HANDLE == dhandle)
    {
        mct_msgBox_Show("打开设备失败\n请检查CAN盒子是否连接");
        return 1;
    }
    //CAN 设备设置波特率的 key 为 baud_rate，值 1000000 为 1000kbps, 800000 为 800kbps, 其它请查看
    //属性表
    //若为 CANFD 设备, 设置冲裁域波特率的 key 为 canfd_abit_baud_rate，数据域波特率为
      //canfd_dbit_baud_rate，请注意区分 CAN 和 CANFD 设备设置波特率的区别。
        if (ZCAN_SetValue (dhandle , "0/baud_rate", "1000000") != STATUS_OK)
    {
        mct_msgBox_Show("设置波特率失败\n已断开CAN盒连接");
        goto end;
    }

    memset(&cfg, 0, sizeof(cfg));
    cfg.can_type = TYPE_CAN;//CANFD 设备为 TYPE_CANFD
    cfg.can.filter = 0;
    cfg.can.mode = 0; //正常模式, 1 为只听模式
    cfg.can.acc_code = 0;
    cfg.can.acc_mask = 0xffffffff;
    chHandle = ZCAN_InitCAN(dhandle, 0, &cfg);
    if (INVALID_CHANNEL_HANDLE == chHandle)
    {
        mct_msgBox_Show("启动通道失败\n已断开CAN盒连接");
        goto end;
    }
    if (ZCAN_StartCAN(chHandle) != STATUS_OK)
    {
        mct_msgBox_Show("启动通道失败\n已断开CAN盒连接");
        goto end;
    }

    memset(&frame, 0, sizeof(frame));
    frame.frame.can_id = MAKE_CAN_ID(0x100, 1, 0, 0);
    frame.frame.can_dlc = 8;

    memcpy(frame.frame.data, data, sizeof(data));
    if (ZCAN_Transmit(chHandle, &frame, 1) != 1)
    {
        mct_msgBox_Show("发送数据失败\n已断开CAN盒连接");
        goto end;
    }
end:
    ZCAN_CloseDevice(dhandle);
    qDebug()<<"end";
    return 1;
}

void M_Can_Task::mct_msgBox_Show(QString msg)
{
    QMessageBox msg_Box;
    qDebug()<<msg;
    msg_Box.setText(msg);
    msg_Box.exec();
}

int M_Can_Task::mct_close_device()
{
    int ret = ZCAN_CloseDevice(mct_dhandle);
    if(ret == STATUS_ERR)
    {
        mct_msgBox_Show("关闭设备操作失败");
    }
    return ret;
}

void M_Can_Task::mct_DataRecive()
{
    ZCAN_Receive_Data can_data[100];
    ZCAN_ReceiveFD_Data canfd_data[100];
    UINT len;
    while(isStart)
    {
        if (len = ZCAN_GetReceiveNum(mct_chHandle, TYPE_CAN))
        {
            len = ZCAN_Receive(mct_chHandle, can_data, 100, 50);
//            AddData(can_data, len);
            qDebug()<<can_data;
        }

    }

}
