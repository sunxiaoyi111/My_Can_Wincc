#ifndef CANMESSAGEBASE_H
#define CANMESSAGEBASE_H

#include <QObject>
#include "cansingalbase.h"
#include "m_candatabase.h"
class canMessageBase
{
public:
    canMessageBase();
    QList<cansingalbase> SingalList;//信号队列
    QString Name;                   //消息名字
    bool IsSend;                    //是否是发送帧
    quint32 SendCycle;              //是否是周期信号
    QString SendNodeName;           //发送节点名字
    M_CanDataBase Can;              //CAN信号
    bool isMoto;                    //是否moto编码
    void SetMoto(bool moto);        //设置为moto码
    void getValue(const unsigned char *Data,int len);   //获取数据
    int  getValue_t(const unsigned char *Data,int len);
signals:
    void updataCan_value(cansingalbase si, float value);
};

#endif // CANMESSAGEBASE_H
