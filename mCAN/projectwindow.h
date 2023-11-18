#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <QMainWindow>




#include "m_candatabase.h"
#include <QVariant>
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

private:
    void mw_Send_Can(uint32_t can_id,uint8_t *data);
signals:
    void mw_Send_Can_data_signal(QVariant cd);

private:
    Ui::ProjectWindow *ui;
};

#endif // PROJECTWINDOW_H
