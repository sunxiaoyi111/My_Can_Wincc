#ifndef M_CANWIDGET_H
#define M_CANWIDGET_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include "m_can_task.h"

#include "m_candatabase.h"
#include "m_canconfig.h"

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>

#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

#include "m_candatabase.h"
#include "m_candatasql.h"



QT_BEGIN_NAMESPACE
namespace Ui { class M_CANWidget; }
QT_END_NAMESPACE

class M_CANWidget : public QWidget
{
    Q_OBJECT

public:
    M_CANWidget(QWidget *parent = nullptr);
    ~M_CANWidget();
    m_canconfig init_config;
    M_CanDataSql data;
signals:
    void test_send(M_CanDataBase element);
public slots:
    void test_receive(M_CanDataBase element);
    void OnTextEdit(const QString& strText);


private:
    Ui::M_CANWidget *ui;
    M_Can_Task * m_can;
    QThread *my_Can_Thread;
    void lock_can_settings();
    QSqlTableModel *m_pSqlTableModel;

};
#endif // M_CANWIDGET_H
