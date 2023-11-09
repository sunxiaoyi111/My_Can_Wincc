#include "m_canwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    M_CANWidget w;
    w.show();
    return a.exec();
}
