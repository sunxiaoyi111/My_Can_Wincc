#ifndef M_CANDATABASE_H
#define M_CANDATABASE_H

#include <QObject>
#include <QDateTime>
class M_CanDataBase
{
public:
    M_CanDataBase();
    //一帧 报文数据
    unsigned char * Data ;
    uint8_t data_can[8];
    QDateTime Time;
    double sTime;

    //接收报文的时间
    QDateTime mcd_Time_Rx;
    //这帧报文的CAN ID
    uint id;
    QString getStrData();

    void mcd_SetTime(QDateTime time);
    void mcd_SetTime(quint64 time);

    int Len();
    void SetData(uint id,unsigned char * data,int len,bool extend = false,bool remote = false);
    void SetTime(QDateTime time);
    void SetTime(quint64 time);

protected:
    QString strData;
    int lenght = 8;

};
struct MyClass_Candata {
    uint32_t time;
    uint8_t dir  = 0; //1: rx  0: tx
    int id = 0;
    uint8_t data[8] = {0};
};
struct M_struct_CanRx {
    uint32_t time = 0;
    uint32_t id   = 0;
    uint8_t  data[8] = {0};
};

Q_DECLARE_METATYPE(MyClass_Candata)
Q_DECLARE_METATYPE(M_struct_CanRx)
struct P_struct_Rx_Show {
    uint id     = 0;
    QString  str   ;
};
Q_DECLARE_METATYPE(P_struct_Rx_Show)
#endif // M_CANDATABASE_H
