#include "cansingalbase.h"
#include <QDebug>
cansingalbase::cansingalbase()
{
    this->isSigned = false;
    this->isEnum = false;
    this->Value = 0;
}

void cansingalbase::GetMaxMinValue()
{

}

void cansingalbase::GetEnd()
{
    this->End = Start + Number - 1;
}

double cansingalbase::GetValue()
{
    double value = (Value * Rate + Offset);
    return value;
}

QString cansingalbase::GetStrValue()
{
    if (this->isEnum)
    {

        QString str1 = "";
//         str += QString("%1 ").arg( this->Data[i],2,16,QLatin1Char('0'));
        qDebug()<< QString("这是枚举类型'%1'")
                        .arg(QString::number(Value));
        qDebug() << "地址是:"
                 << QString("%1")
                        .arg((quintptr)&Value,
                             QT_POINTER_SIZE * 2,
                             16,
                             QChar('0'));
        if(Status.contains(Value))
        {
            str1 = Status[Value];
        }
        else
        {
            str1 = "无效值";
        }
        return str1;
    }
    else
    {
        double value = (Value * Rate + Offset);
        QString str = QString::number(value,'f',5);
        return str + Unit;
    }
}
