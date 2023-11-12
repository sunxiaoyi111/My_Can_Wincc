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





    bool mcd_sqlconnect();
    bool mcd_sqladd();
};

#endif // M_CANDATASQL_H
