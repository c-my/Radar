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

    inline double convertDis(int dis);
    int mst;

    int distances[256] = {0};
    int MAX_DIS = 100;

    double soundSpeed;

    QTimer scanTimer;
    int lefttop;
    int interval;
    int scanAngle;

    int currentAngle, lastAngle;
    int currentDistant, lastDistant;
    bool isClock;
    int baseDiameter;



signals:
    void sendPos(int ang, int dis);

public slots:
    void getPos(int ang, int dis);
    void setSoundSpeed(double speed);
};

#endif // RADAR_H
