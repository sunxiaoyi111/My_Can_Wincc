#include "m_candatabase.h"

M_CanDataBase::M_CanDataBase()
{

}
void M_CanDataBase::mcd_SetTime(QDateTime time)
{
    this->Time = time;
}

void M_CanDataBase::mcd_SetTime(quint64 time)
{
    this->sTime = time / 1000000.0;
}

QString M_CanDataBase::mcd_getStringData()
{
    if(this->mcd_lenght==0)
    {
        return "00 00 00 00 00 00 00 00";
    }
    else
    {
        QString str ="";

        for(int i=0;i<this->mcd_lenght;i++)
        {
            str += QString("%1 ").arg( this->Data[i],2,16,QLatin1Char('0'));
        }
        // strData = str;
        return str;
    }
}
int M_CanDataBase::Len()
{
    return mcd_lenght;
}
