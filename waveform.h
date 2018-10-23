#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QPalette>
class Waveform : public QWidget
{
    Q_OBJECT
private:
    inline double convertDis(int dis);
    int distances[256] = {0};
    int top;
    int left;
    int mst;

    int MAX_DIS = 400;
public:
    explicit Waveform(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

    int currentAngle, lastAngle;
    double soundSpeed;



signals:

public slots:
    void getPos(int ang, int dis);
    void setSoundSpeed(double speed);
};

#endif // WAVEFORM_H
