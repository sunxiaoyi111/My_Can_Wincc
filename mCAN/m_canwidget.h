#ifndef M_CANWIDGET_H
#define M_CANWIDGET_H

#include <QWidget>
#include <QThread>
#include "m_can_task.h"





QT_BEGIN_NAMESPACE
namespace Ui { class M_CANWidget; }
QT_END_NAMESPACE

class M_CANWidget : public QWidget
{
    Q_OBJECT

public:
    M_CANWidget(QWidget *parent = nullptr);
    ~M_CANWidget();
signals:
    int sig_in_num(int try_in);
public slots:
    int  ret_any_in(int in);
private:
    Ui::M_CANWidget *ui;
    M_Can_Task * m_can;
    QThread *my_Can_Thread;

};
#endif // M_CANWIDGET_H
