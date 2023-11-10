#ifndef M_CANDATABASE_H
#define M_CANDATABASE_H

#include <QObject>
#include <QDateTime>
class M_CanDataBase
{
public:
    M_CanDataBase();
    //一帧 报文数据
    unsigned char * Data;

    QDateTime Time;
    double sTime;

    //接收报文的时间
    QDateTime mcd_Time_Rx;
    //这帧报文的CAN ID
    uint mcd_canID;
    QString mcd_getStringData();

    void mcd_SetTime(QDateTime time);
    void mcd_SetTime(quint64 time);

    int Len();

protected:
    QString mcd_strData;
    int mcd_lenght;

};

#endif // M_CANDATABASE_H
