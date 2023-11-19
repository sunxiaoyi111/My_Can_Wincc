#ifndef CANSINGALBASE_H
#define CANSINGALBASE_H

#include <QObject>
#include <QMap>
class cansingalbase
{
//    Q_OBJECT
public:
    cansingalbase();
    //暂用做上位机
    uint lable_id;
    int Start;       //启始位
    int Number;      //位长度
    int End;         //结束位
    double Rate;     //分辩率
    double Offset;   //偏移
    QString Unit;    //单位
    double Value;    //实际值
    QString StrValue;//显示值
    bool isSigned;   //有符号
    bool isEnum = false;
    quint8 multiplexer_type;    //see 'multiplexer type' above
    quint8 valType;            //0:integer, 1:float, 2:double
    QMap<int,QString> Status; //枚举类型存储

    void GetMaxMinValue();
    void GetEnd();
    double GetValue();
    QString GetStrValue();
};

#endif // CANSINGALBASE_H
