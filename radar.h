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

    int distances[256] = {0};

    QTimer scanTimer;
    int lefttop;
    int interval;
    int scanAngle;

    int currentAngle, lastAngle;
    int currentDistant, lastDistant;
    bool isClock;
    int baseRadius;



signals:
    void sendPos(int ang, int dis);

public slots:
    void getPos(int ang, int dis);
};

#endif // RADAR_H
