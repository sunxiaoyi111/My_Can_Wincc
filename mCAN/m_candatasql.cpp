#include "m_candatasql.h"

M_CanDataSql::M_CanDataSql()
{
    connect(&task_timer,&QTimer::timeout,this,[=](){
        mcd_Task();
    });
    task_timer.start(10);
}
M_CanDataSql::~M_CanDataSql()
{

}
bool M_CanDataSql::mcd_sqlconnect()
{
    qDebug() << "Q:可利用的数据库的驱动:";
    QStringList drivers = QSqlDatabase::drivers();
    qDebug() << drivers;
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
    query.exec("CREATE table tbl_staff(id INTEGER  primary key, rttime text,canid text,dir text,candata text)");
//    query.exec("INSERT INTO tbl_staff VALUES(1,'0' ,'123', '01 02 03 04 05 06 07 08' )");


    query.exec(R"(
    CREATE TRIGGER limit_records
    AFTER INSERT ON tbl_staff
    BEGIN
        DELETE FROM tbl_staff WHERE id IN (
            SELECT id FROM tbl_staff ORDER BY id DESC LIMIT -1 OFFSET 10000
        );
    END;
)");



    return 0;
}
bool M_CanDataSql::mcd_sqladd()
{
//    qDebug()<<"md";
//    M_CanDataBase md;
//    mct_Rx.append(md);
    return 0;
}
bool M_CanDataSql::mcd_sqladd_candata_Variant(QVariant cd)
{
    MyClass_Candata data= cd.value<MyClass_Candata>();
    mct_Rx.append(data);
    return 0;
}
void  M_CanDataSql::mcd_Task()
{
    QSqlQuery query;
    m_lock.lock();
    if(!mct_Rx.isEmpty())
    {
        db.transaction(); // 开启事务
        while(!mct_Rx.isEmpty()){
            MyClass_Candata dt=mct_Rx.dequeue();
            // 执行查询获取总行数
//            query.exec("SELECT COUNT(*) FROM tbl_staff");
//            if (query.next()) {
//                rowCount = query.value(0).toInt();
//            }
//            if(rowCount>=10)rowCount=9;
            query.exec("SELECT last_insert_rowid()");
            query.next();
            bool ok;
            int id  = 1;
            id =  query.value(0).toInt(&ok);
            if(ok)
            {
                id=id+1;
            }
            QString str ="";
            for(int i=0;i<8;i++)
            {
                str += QString("%1 ").arg( dt.data[i],2,16,QLatin1Char('0'));
            }
            QString str_id    = QString::number(id);
            QString str_time  = QString::number(id);
            QString str_CanId = QString::number(dt.id,16);
            QString dir[2] = {"Tx","Rx"};
            QString str_dir   = dir[dt.dir];
            QString str_data  = str;
            QString cmd = QString("INSERT INTO tbl_staff VALUES( '%1','%2' , '%3', '%4' ,'%5' )")
                              .arg(str_id,
                                   str_time,
                                   str_CanId,
                                   str_dir,
                                   str_data);
            query.exec(cmd);
        }
        db.commit();
    }
    m_lock.unlock();
    query.finish(); // 资源释放
}
