#ifndef M_CANCONFIG_H
#define M_CANCONFIG_H

#include <QObject>
#include <QSettings> // 引入QSettings头文件
#include <QCoreApplication> // 引入QCoreApplication头文件
#include <QDir> // 引入QDir头文件
class m_canconfig
{
public:
    m_canconfig();
    ~m_canconfig();

    void m_canconfig_init();



private:
    QString mcfg_bb;

};

#endif // M_CANCONFIG_H
