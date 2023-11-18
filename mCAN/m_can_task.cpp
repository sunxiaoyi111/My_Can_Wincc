#include "m_can_task.h"
M_Can_Task::M_Can_Task()
{
    qDebug()<<"init";
    connect(&mct_CanRx_timer,&QTimer::timeout,this,[=](){
//        qDebug()<< "timeout!";
        static uint32_t num = 0;
        while(!mct_Rx.isEmpty()){
            num++;
            mct_Rx.dequeue();
             qDebug()<< num;

        }

    });
    mct_CanRx_timer.start(200);
}
M_Can_Task::~M_Can_Task()
{
    mct_close_device();
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
    if (ZCAN_SetValue (mct_dhandle , "0/baud_rate", "500000") != STATUS_OK)
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


#pragma region "发送数据测试" {
//    memset(&mct_frame, 0, sizeof(mct_frame));
//    mct_frame.frame.can_id = MAKE_CAN_ID(0x100, 1, 0, 0);
//    mct_frame.frame.can_dlc = 8;

//    memcpy(mct_frame.frame.data, test_data, sizeof(test_data));
//    if (ZCAN_Transmit(mct_chHandle, &mct_frame, 1) != 1)
//    {
//        mct_msgBox_Show("发送数据失败\n已断开CAN盒连接");
//        goto end;
//    }
#pragma endregion}

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
/**
 * @brief mct_msgBox_Show
 * @brief can task 弹窗提示
 * @param Na
 * @param
 * @return 设备打开结果  0 成功  Others :失败
 */
void M_Can_Task::mct_msgBox_Show(QString msg)
{
    QMessageBox msg_Box;
    qDebug()<<msg;
    msg_Box.setText(msg);
    msg_Box.exec();
}
/**
 * @brief 关闭CAN盒
 * @brief
 * @param Na
 * @param
 * @return 设备打开结果  0 成功  Others :失败
 */
int M_Can_Task::mct_close_device()
{
    if(mct_dhandle==nullptr) return STATUS_OK;
    int ret = ZCAN_CloseDevice(mct_dhandle);
    if(ret == STATUS_ERR)
    {
        mct_msgBox_Show("关闭设备操作失败");
    }
    mct_dhandle = nullptr;
    return ret;
}
/**
 * @brief mct_DataTransmitBlock
 * @brief 阻塞发送
 * @param Na
 * @param
 * @return 设备打开结果  0 成功  Others :失败
 */
int M_Can_Task::mct_DataTransmitBlock(uint32_t can_id,uint8_t * data,uint8_t length)
{
    ZCAN_Transmit_Data frame;
    memset(&frame, 0, sizeof(frame));
    //标准帧 数据帧 CAN帧
    frame.frame.can_id = MAKE_CAN_ID(can_id, 0, 0, 0);
    frame.frame.can_dlc = 8;
    if(length<=8)
    {
        memcpy(frame.frame.data, data, length);
    }
    else
    {
        memcpy(frame.frame.data, data, 8);
    }
    if (ZCAN_Transmit(mct_chHandle, &frame, 1) != 1)
    {
        qDebug() << "发送数据失败" ;
        return STATUS_ERR;
    }

    MyClass_Candata dt;
    memcpy(&(dt.data[0]),&(data[0]),8);
    dt.id = can_id;
    dt.dir = 0;
    QVariant var = QVariant::fromValue(dt);
    emit mctask_Rx_Variant(var);

    return STATUS_OK;
}
/**
 * @brief mctask_receiveElement
 * @brief 接收到CAN报文发送该节点到其他线程
 * @param Na
 * @param
 * @return 设备打开结果  0 成功  Others :失败
 */
void M_Can_Task::mctask_receiveElement(M_CanDataBase element)
{
    qDebug()<<"M_Can_Task::receiveElement";
    qDebug()<<element.id;
}
void M_Can_Task::mctask_sendDataSequence(M_CanDataBase element)
{
    qDebug()<<"M_Can_Task::mctask_sendDataSequence";
    qDebug()<<element.id << ":append";
    mct_Tx.append(element);
}
void M_Can_Task::mytask_sendSequence(QVariant cd)
{
    M_CanDataBase element;
    MyClass_Candata data= cd.value<MyClass_Candata>();
    element.id = data.id;
    memcpy(element.data_can,data.data,8);
    mct_Tx.append(element);
}
void M_Can_Task::mct_Recurring_Task()
{
    UINT len = 0;
    //从CAN盒缓存队列中获取当前通道的CAN报文数量
    len = ZCAN_GetReceiveNum(mct_chHandle, TYPE_CAN);
    if (len>0)
    {
        ZCAN_Receive_Data can_data[len] ;
        ZCAN_Receive(mct_chHandle, can_data, len, 50);
        mct_AddData(can_data, len);
    }
    if(!mct_Tx.isEmpty())
    {
        int send_num = mct_Tx.size();
        send_num = (send_num <= 10) ? send_num:10;
        for(int loop_num = 0;loop_num<send_num;loop_num++)
        {
            M_CanDataBase can_d_tx = mct_Tx.head();
               if(mct_DataTransmitBlock(can_d_tx.id, can_d_tx.data_can, can_d_tx.Len())==STATUS_OK)
            {
                mct_Tx.dequeue();
//                mct_AddData(&can_d_tx);
            }
        }
    }
}
void M_Can_Task::mct_AddData(const ZCAN_Receive_Data *data, UINT len)
{
    for (UINT i = 0; i < len; ++i)
    {
        const ZCAN_Receive_Data& can = data[i];
        const canid_t& id = can.frame.can_id;
        M_CanDataBase *cdb = new M_CanDataBase();
        cdb->SetTime(data->timestamp);
        cdb->SetData(GET_ID(id),(unsigned char*)can.frame.data,can.frame.can_dlc,IS_EFF(id),IS_RTR(id));
//        mct_AddData(cdb);
        mct_AddData(cdb,1);
        delete cdb;
    }
}
void M_Can_Task::mct_AddData(M_CanDataBase *cdb)
{
    cdb->SetTime(QDateTime::currentDateTime());
    MyClass_Candata dt;
    memcpy(&(dt.data[0]),&(cdb->data_can[0]),8);
    dt.id = cdb->id;
    dt.time = cdb->sTime;
    QVariant var = QVariant::fromValue(dt);
    emit mctask_Rx_Variant(var);
}
void M_Can_Task::mct_AddData(M_CanDataBase *cdb,uint8_t dir)
{
    cdb->SetTime(QDateTime::currentDateTime());
    MyClass_Candata dt;
    memcpy(&(dt.data[0]),&(cdb->data_can[0]),8);
    dt.id = cdb->id;
    dt.time = cdb->sTime;
    dt.dir = dir;
    QVariant var = QVariant::fromValue(dt);
    emit mctask_Rx_Variant(var);
}
