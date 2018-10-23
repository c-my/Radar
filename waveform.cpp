#include "waveform.h"

Waveform::Waveform(QWidget *parent) : QWidget(parent)
{
    top = 100;
    left = 100;
    soundSpeed = 340;

    resize(2*left + 180*3, 2*top+MAX_DIS);
    setAutoFillBackground(true);
    setPalette(QPalette(Qt::lightGray));
}

void Waveform::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black,1));

    //horizon
    painter.drawLine(left, top+MAX_DIS, left+180 * 3, top + MAX_DIS);
    //ver
    painter.drawLine(left, top, left, top+MAX_DIS);

    // draw target
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::black));
    for(int i = 0; i < 256; i++)
    {
        if(distances[i]!=0){
        double angle = i;
        double dis = distances[i];
        //        qDebug()<<dis;
        double x = left + angle*3;
        double y = top + MAX_DIS - dis;
        // qDebug() << "draw point: " << x << " " << y;
//        painter.drawPoint(int(x), int(y));
        painter.drawEllipse(int(x), int(y), 2, 2);
        }
    }
    for(int i=0;i<5;++i){
        painter.drawText(left+i*45*3 - 5,10+top+MAX_DIS, QString::number(i*45));
    }
    for(int i=0;i<4;++i){

    }
}


double Waveform::convertDis(int dis)
{
    return dis*soundSpeed/20000.0;
}

void Waveform::getPos(int ang, int dis)
{
    mst = dis;
    distances[ang] = int(convertDis(dis));
    qDebug()<<convertDis(dis);
    lastAngle = currentAngle;
    currentAngle = ang;

    update();
}

void Waveform::setSoundSpeed(double speed)
{
    soundSpeed = speed*20000/mst;

}
