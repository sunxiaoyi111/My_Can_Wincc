#include "m_candatasql.h"

M_CanDataSql::M_CanDataSql()
{

}

M_CanDataSql::~M_CanDataSql()
{

}

bool M_CanDataSql::mcd_sqlconnect()
{


    qDebug() << "Q:可利用的数据库的驱动:";
    QStringList drivers = QSqlDatabase::drivers();
    qDebug() << drivers;
//    foreach(QString driver, drivers)
//    {
////      //qDebug() << driver;
////      qDebug("%s", qPrintable(driver));
//    }

    // 创建数据库的连接(数据库的驱动)
    db = QSqlDatabase::addDatabase("QSQLITE");
    // 获取程序的路径
    appDir = QCoreApplication::applicationDirPath();
    dbDir = QDir(appDir).filePath("candatatemp");
    if(!QDir(dbDir).exists())
    {
        QDir().mkdir(dbDir);
    }
    current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd.hh.mm.ss.zzz.ddd");
    QString db_name = QDir(appDir).filePath("candatatemp/cd"+current_date+".db3");
    db.setDatabaseName(db_name);
    if(!db.open())
    {
        QMessageBox::critical(0, ("can data sql init fail"),
                              ("Unable to establish a database connection."),
                              QMessageBox::Cancel);
        return false;
    }
    // 执行sql语句
    QSqlQuery query;
    // 新建表       id  时间标识rttime   canid  candata
    query.exec("CREATE table tbl_staff(id int primary key, rttime int,canid int,candata varchar)");
    query.exec("INSERT INTO tbl_staff VALUES(1001,0 ,123, '1 2 3 4 5 6 7 8 9' )");


}

bool M_CanDataSql::mcd_sqladd()
{
    QSqlQuery query;
    // 新建表       id  时间标识rttime   canid  candata
    static int num = 1001;
    num++;
//    QString cmd = "INSERT INTO tbl_staff VALUES("+QString::number(num)+ ",0 ,123, '1 2 3 4 5 6 7 8 9' )";
    QString cmd = QString("INSERT INTO tbl_staff VALUES( '%1',0 ,123, '1 2 3 4 5 6 7 8 9' )").arg(QString::number(num));

    query.exec(cmd);
    qDebug()<<"mcd_sqladd";
}
