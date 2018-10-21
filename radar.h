#ifndef RADAR_H
#define RADAR_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QtMath>
#include <QTimer>
#include <QVector>

class Radar : public QWidget
{
    Q_OBJECT
public:
    explicit Radar(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
private:
    struct Point{
        double angle;
        double dis;
    };

    QVector<Point> pointList;

    QTimer scanTimer;
    int lefttop;
    int interval;
    int scanAngle;

    int currentAngle, lastAngle;
    int currentDistant, lastDistant;
    bool isClock;
    int baseRadius;



signals:
    void sendPos(double ang, double dis);

public slots:
    void getPos(double ang, double dis);
};

#endif // RADAR_H
