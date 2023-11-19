#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>


#include "m_candatabase.h"
#include <QVariant>

#include "Candispatcher/can_dispatcher_table.h"
namespace Ui {
class ProjectWindow;
}

class ProjectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProjectWindow(QWidget *parent = nullptr);
    ~ProjectWindow();
    void mw_ui_init();

    QLabel * mw_labellist[Candata_singal_list_MAX];
signals:
    void mw_Send_Can_data_signal(QVariant cd);

private:
    void mw_Send_Can(uint32_t can_id,uint8_t *data);
    void mw_Send_Can_cit(uint signa_id);
    Ui::ProjectWindow *ui;
    void mw_singal_int_rx_ui_list();
    void mw_singal_init_tx();
public slots:
    void mw_slots_set_label_rx(QVariant si);
signals:

};


union float_bytes{
    float value;
    uint8_t bytes[4];
};


#endif // PROJECTWINDOW_H
