#include "canmessagebase.h"
#include "qmath.h"
#include <QDebug>
canMessageBase::canMessageBase()
{
    this->IsSend = false;
    SendCycle = 0;
}
void canMessageBase::SetMoto(bool moto)
{
    this->isMoto = moto;
}

void canMessageBase::getValue(const unsigned char *data, int len)
{
    QString bits;
    qDebug()<<"get_value_start";
    for(int i=0;i<len;i++)
    {
        QString str = QString("%1").arg(data[i],8,2,QLatin1Char('0'));
        QString str1;
        str1.fill('0',str.size());
        for(int j = 0;j<str1.length();j++)
            str1[j] = str[str1.length()-j-1];
        bits += str1;
    }

    for(int j=0;j<SingalList.size();j++)
    {
        cansingalbase item = SingalList.at(j);

        qint32 value = 0;
        bool isGetValue = true;

        for (int i = 0; i < item.Number; i++)
        {
            int p = i + item.Start;
            if (p >= 0 && p < len * 8)
            {
                if (bits[i + item.Start] == QLatin1Char('1'))
                    value += (qint32)qPow(2, i);
            }
            else
            {
                isGetValue = false;
                break;
            }
        }


        if (isGetValue)
        {
            qDebug()<<"get_value"<< value << &SingalList[j].Value;
            SingalList[j].Value = value;
            //            emit this->updataCan_value(SingalList[j], value);
        }
    }
}


int canMessageBase::getValue_t(const unsigned char *data, int len)
{
    QString bits;
    int is_get_Value_numb=0;
    qDebug()<<"get_value_start";
    for(int i=0;i<len;i++)
    {
        QString str = QString("%1").arg(data[i],8,2,QLatin1Char('0'));
        QString str1;
        str1.fill('0',str.size());
        for(int j = 0;j<str1.length();j++)
            str1[j] = str[str1.length()-j-1];
        bits += str1;
    }

    for(int j=0;j<SingalList.size();j++)
    {
        cansingalbase item = SingalList.at(j);

        qint32 value = 0;
        bool isGetValue = true;

        for (int i = 0; i < item.Number; i++)
        {
            int p = i + item.Start;
            if (p >= 0 && p < len * 8)
            {
                if (bits[i + item.Start] == QLatin1Char('1'))
                    value += (qint32)qPow(2, i);
            }
            else
            {
                isGetValue = false;
                break;
            }
        }


        if (isGetValue)
        {
            qDebug()<<"get_value"<< value << &SingalList[j].Value;
            SingalList[j].Value = value;
            is_get_Value_numb++;
        }

    }
    return is_get_Value_numb;
}
