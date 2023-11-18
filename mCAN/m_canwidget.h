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
#include "projectwindow.h"
#include "Candispatcher/can_dispatcher.h"

#include <QVariant>

QT_BEGIN_NAMESPACE
namespace Ui { class M_CANWidget; }
QT_END_NAMESPACE

class M_CANWidget : public QWidget
{
    Q_OBJECT

public:
    M_CANWidget(QWidget *parent = nullptr);
    ~M_CANWidget();
    m_canconfig  init_config;
    M_CanDataSql data;
    ProjectWindow projectwindow;

signals:
    void test_send(M_CanDataBase element);
//    void send_Can_Data_Quence(M_CanDataBase element);
    void send_Can_Data_Quence(QVariant cd);
public slots:
    void OnText_Filter_Edit(const QString& strText);
    void pb_Device_Open();
    void OnAsorDsSort();
    //数据表操作功能
    void UI_data_Save();
    void UI_data_Stop();
    void UI_data_clear();

    void UI_device_widget_show();
    void UI_Can_send_widget_show();
private:
    Ui::M_CANWidget *ui;
    M_Can_Task * m_can;
    QThread *my_Can_Thread;
    void lock_can_settings();
    void set_tablemodel_ui();
    QSqlTableModel *m_pSqlTableModel = nullptr;
    Can_dispatcher *can_dispatcher;
    QThread *my_Can_dispatcher_Thread;
    QTimer * m_sql_sectimer;
};
#endif // M_CANWIDGET_H
