#include "radar.h"
#include <QtDebug>

Radar::Radar(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(tr("Radar"));

    setAutoFillBackground(true);
    setPalette(QPalette(Qt::black));
    lefttop = 20;
    interval = 150;
    scanAngle = 358;

    lastAngle = 0;
    lastDistant = 0;

    // test
    currentAngle = 180;
    isClock = true;
    Point p;
    p.angle = 45;
    p.dis = 20;

    baseRadius = 500;

//    serial = new SerialController;
//    serial->moveToThread(&SerialThr);
//    SerialThr.start();
    connect(&scanTimer,&QTimer::timeout, this, [this](){
       currentAngle++;
       this->update();
    });
    scanTimer.start(1);
}

void Radar::paintEvent(QPaintEvent *)
{
    // radar background
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::green,1));

    painter.drawLine(lefttop, lefttop + baseRadius/2,lefttop + baseRadius,lefttop + baseRadius/2);
    painter.drawLine(lefttop + baseRadius/2,lefttop, lefttop + baseRadius/2,lefttop + baseRadius);

    for(int i=0;i<5;++i){
        painter.drawArc(lefttop+i*interval/2,lefttop+i*interval/2,baseRadius-i*interval,baseRadius-i*interval,0,360*16);
    }

    //  scan area
    QConicalGradient conicalGradient(lefttop+baseRadius/2, lefttop+baseRadius/2, currentAngle);

    if(isClock){
        conicalGradient.setAngle(currentAngle-scanAngle-1);
        conicalGradient.setColorAt(0, QColor(0,0,0,120));
        conicalGradient.setColorAt((scanAngle)/360.0, QColor(0,255,0,120));
        painter.setPen(QPen(QColor(0,0,0,0),1));
        painter.setBrush(QBrush(conicalGradient));
        painter.drawPie(lefttop,lefttop,baseRadius,baseRadius,(currentAngle-scanAngle)*16,scanAngle*16);
    } else{
        conicalGradient.setColorAt(0, QColor(0,255,0,120));
        conicalGradient.setColorAt(scanAngle/360.0, QColor(0,0,0,120));
        painter.setPen(QPen(QColor(0,0,0,0),1));
        painter.setBrush(QBrush(conicalGradient));
        painter.drawPie(lefttop,lefttop,baseRadius,baseRadius,currentAngle*16,scanAngle*16);
    }
//    painter.drawRect(250,250,20,20);

    // draw target
    painter.setPen(QPen(Qt::red));
    painter.setBrush(QBrush(Qt::red));
    for(int i = 0; i < 256; i++)
    {
        double angle = i;
        double dis = distances[i];
        double x = lefttop + baseRadius / 2 + dis*qCos(qDegreesToRadians(angle));
        double y = lefttop + baseRadius / 2 - dis*qSin(qDegreesToRadians(angle));
        // qDebug() << "draw point: " << x << " " << y;
        painter.drawPoint(int(x), int(y));
        painter.drawEllipse(int(x), int(y), 5, 5);
    }
}

void Radar::getPos(int ang, int dis)
{
    distances[ang] = dis;
    update();
}
