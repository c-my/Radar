#include "radar.h"
#include <QtDebug>

Radar::Radar(QWidget *parent) : QWidget(parent)
{

    setAutoFillBackground(true);
    setPalette(QPalette(Qt::black));
    lefttop = 50;
    interval = 200;
    scanAngle = 40;

    soundSpeed = 340;

    lastAngle = 0;
    lastDistant = 0;

    // test
    currentAngle = 0;
    isClock = false;

    baseDiameter = 800;

    setWindowTitle(tr("Radar"));
    resize(baseDiameter+2*lefttop, baseDiameter/2+2*lefttop);


    connect(&scanTimer,&QTimer::timeout, this, [this](){
        //       currentAngle++;
        //       this->update();
    });
    scanTimer.start(1);
}

void Radar::paintEvent(QPaintEvent *)
{
    // radar background
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::darkGreen,1));

    for(int i=0;i<5;++i){
        painter.drawArc(lefttop+i*interval/2,lefttop+i*interval/2,baseDiameter-i*interval,baseDiameter-i*interval,0,180*16);
        painter.drawText(lefttop + baseDiameter - i* interval/2, lefttop+ baseDiameter/2, QString::number(MAX_DIS - i*double(interval)/baseDiameter*MAX_DIS));
    }

    //  scan area
    QConicalGradient conicalGradient(lefttop+baseDiameter/2, lefttop+baseDiameter/2, currentAngle);
    //    conicalGradient.setStops(currentAngle+scanAngle);
    conicalGradient.setAngle(currentAngle-scanAngle-1);


    if(isClock){
        conicalGradient.setAngle(currentAngle);
        conicalGradient.setColorAt(0, QColor(0,230,0,120));
        conicalGradient.setColorAt(scanAngle/360.0, QColor(0,0,0,120));
        painter.setPen(QPen(QColor(0,0,0,0),1));
        painter.setBrush(QBrush(conicalGradient));
        painter.drawPie(lefttop,lefttop,baseDiameter,baseDiameter,(currentAngle)*16,scanAngle*16);
    } else{
        conicalGradient.setColorAt(0, QColor(0,0,0,120));
        conicalGradient.setColorAt(scanAngle/360.0, QColor(0,230,0,120));
        painter.setPen(QPen(QColor(0,0,0,0),1));
        painter.setBrush(QBrush(conicalGradient));
        painter.drawPie(lefttop,lefttop,baseDiameter,baseDiameter,(currentAngle-scanAngle)*16,scanAngle*16);
    }
    //    painter.drawRect(250,250,20,20);

    // draw target
    painter.setPen(QPen(Qt::green));
    painter.setBrush(QBrush(Qt::green));
    for(int i = 0; i < 256; i++)
    {
        double angle = i;
        double dis = distances[i];
        //        qDebug()<<dis;
        double x = lefttop + baseDiameter / 2 + dis*qCos(qDegreesToRadians(angle));
        double y = lefttop + baseDiameter / 2 - dis*qSin(qDegreesToRadians(angle));
        // qDebug() << "draw point: " << x << " " << y;
        painter.drawPoint(int(x), int(y));
        painter.drawEllipse(int(x), int(y), 5, 5);
    }

    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::black));
    painter.drawPie(lefttop,lefttop,baseDiameter,baseDiameter,180*16,180*16);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::darkGreen,1));

    painter.drawLine(lefttop, lefttop + baseDiameter/2,lefttop + baseDiameter,lefttop + baseDiameter/2);
    painter.drawLine(lefttop + baseDiameter/2,lefttop, lefttop + baseDiameter/2,lefttop + baseDiameter/2);

}

double Radar::convertDis(int dis)
{
    return dis*soundSpeed/20000.0;
}

void Radar::getPos(int ang, int dis)
{
    mst=dis;
    if(ang == 255){
        memset(distances, 0,255*sizeof (int));
        distances[90] = int(convertDis(dis)/MAX_DIS*baseDiameter/2);

    }
    else{
        distances[ang] = int(convertDis(dis)/MAX_DIS*baseDiameter/2);
    }
    qDebug()<<convertDis(dis);
    lastAngle = currentAngle;
    currentAngle = ang;
    isClock = currentAngle < lastAngle;

    update();
}

void Radar::setSoundSpeed(double speed)
{
    soundSpeed = speed*20000/mst;
}
