#include "m_can_task.h"

M_Can_Task::M_Can_Task()
{
    qDebug()<<"init";
}

M_Can_Task::~M_Can_Task()
{
    qDebug()<<"quit";
}

int M_Can_Task::open_device_test()
{
    DEVICE_HANDLE dhandle;
    CHANNEL_HANDLE chHandle;
    ZCAN_Transmit_Data frame;
    ZCAN_CHANNEL_INIT_CONFIG cfg;
    BYTE data[] = {1, 2, 3, 4, 5, 6, 7, 8};


    //ZCAN_USBCAN_2E_U 为设备类型, 请根据实际修改
    dhandle = ZCAN_OpenDevice(ZCAN_USBCAN_2E_U, 0, 0);
    if (INVALID_DEVICE_HANDLE == dhandle)
    {
        qDebug() << "打开设备失败" ;
        return 0;
    }
    //CAN 设备设置波特率的 key 为 baud_rate，值 1000000 为 1000kbps, 800000 为 800kbps, 其它请查看
    //属性表
    //若为 CANFD 设备, 设置冲裁域波特率的 key 为 canfd_abit_baud_rate，数据域波特率为
      //canfd_dbit_baud_rate，请注意区分 CAN 和 CANFD 设备设置波特率的区别。
        if (ZCAN_SetValue (dhandle , "0/baud_rate", "1000000") != STATUS_OK)
    {
        qDebug() << "设置波特率失败" ;
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
        qDebug() << "初始化通道失败" ;
        goto end;
    }
    if (ZCAN_StartCAN(chHandle) != STATUS_OK)
    {
        qDebug()<< "启动通道失败" ;
        goto end;
    }

    memset(&frame, 0, sizeof(frame));
    frame.frame.can_id = MAKE_CAN_ID(0x100, 1, 0, 0);
    frame.frame.can_dlc = 8;

    memcpy(frame.frame.data, data, sizeof(data));
    if (ZCAN_Transmit(chHandle, &frame, 1) != 1)
    {
        qDebug() << "发送数据失败" ;
            goto end;
    }
end:
    ZCAN_CloseDevice(dhandle);
    qDebug()<<"end";
    return 0;
}
