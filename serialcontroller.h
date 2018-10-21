#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QSerialPortInfo>
#include <QTimer>

class SerialController : public QObject
{
    Q_OBJECT
public:
    explicit SerialController(QObject *parent = nullptr);
    void openSerial(QString name);

    void closeSerial();
private:
    QSerialPort *serial;
    QTimer testTimer;
    int testAng;
    int testWise;


signals:
    void sendPos(double angle, double dis);
    void openSuccess();
    void openFailed();
    void closeSuccess();


public slots:
    void handleRecv();
    void getBaudrate(QString baudrate);
    void getStopbits(QString stopbits);
    void getDatabits(QString databits);
    void getParity(QString parity);
    void contrloRTS(bool set);
    void controlDTR(bool set);
};

#endif // SERIALCONTROLLER_H
