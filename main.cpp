#include "serialwidget.h"
#include "radar.h"
#include <QGraphicsView>
#include <QObject>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Radar r;

    SerialWidget sw;

//    QObject::connect(&sw, &SerialWidget::deliverPos, &r, &Radar::getPos);
//    r.show();
    sw.show();

    return a.exec();
}
