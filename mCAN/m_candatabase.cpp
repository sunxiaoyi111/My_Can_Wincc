#include "m_candatabase.h"

M_CanDataBase::M_CanDataBase()
{
    Data = data_can;
}
void M_CanDataBase::mcd_SetTime(QDateTime time)
{
    this->Time = time;
}
void M_CanDataBase::mcd_SetTime(quint64 time)
{
    this->sTime = time / 1000000.0;
}
QString M_CanDataBase::getStrData()
{
    if(this->lenght==0)
    {
        return "00 00 00 00 00 00 00 00";
    }
    else
    {
        QString str ="";

        for(int i=0;i<this->lenght;i++)
        {
            str += QString("%1 ").arg( this->Data[i],2,16,QLatin1Char('0'));
        }
        // strData = str;
        return str;
    }
}
int M_CanDataBase::Len()
{
    return lenght;
}
void M_CanDataBase::SetData(uint id,unsigned char * data,int len,bool extend,bool remote)
{
    this->id = id;
    memcpy(this->Data,data,len);
//    this->isextendFrame = extend;
//    this->isremoteFrame = remote;
    this->lenght = len;
}
void M_CanDataBase::SetTime(QDateTime time)
{
    this->Time = time;
}
void M_CanDataBase::SetTime(quint64 time)
{
    this->sTime = time / 1000000.0;
}
