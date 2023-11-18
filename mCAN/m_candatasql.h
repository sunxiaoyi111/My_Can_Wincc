#ifndef M_CANDATASQL_H
#define M_CANDATASQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>
#include <QDateTime>

#include <QTimer>
#include <QQueue>
#include <QMutex>


#include "m_candatabase.h"


class M_CanDataSql: public QObject
{
    Q_OBJECT // 添加Q_OBJECT宏
public:
    M_CanDataSql();
    ~M_CanDataSql();
    QSqlDatabase db;
    QDateTime current_date_time;
    QString appDir;
    QString dbDir;
//    QSqlQuery query;
QMutex m_lock;




    bool mcd_sqlconnect();
    bool mcd_sqladd();
    bool mcd_sqladd_candata_Variant(QVariant cd);
    QTimer task_timer;
    void mcd_Task();

    QQueue<MyClass_Candata> mct_Rx;
};

#endif // M_CANDATASQL_H
