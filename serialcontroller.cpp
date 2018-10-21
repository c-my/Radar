#include "serialcontroller.h"
#include <QDebug>

SerialController::SerialController(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this,SLOT(handleRecv()));

//     QList<QSerialPortInfo> SerialList = QSerialPortInfo::availablePorts();
//    if(SerialList.size()!=0){
//        openSerial(SerialList[0].portName());
//    }
//    testAng = 0;
//    testWise=  false;

//    connect(&testTimer,&QTimer::timeout, this,[this](){
//        emit sendPos(testAng-=2,0);
//    });
//    testTimer.setInterval(1);
//    testTimer.start();


}

void SerialController::openSerial(QString name)
{
    qDebug() << name;
    serial->setPortName(name);
    if (serial->open(QIODevice::ReadWrite))
    {
        //串口打开成功
        serial->setDataTerminalReady(false);
        serial->setDataTerminalReady(false);
        emit openSuccess();

    }
    else
    {
        //串口打开失败
        emit openFailed();
        qDebug() << serial->error();
    }
}

void SerialController::getBaudrate(QString baudrate)
{
    serial->setBaudRate(baudrate.toInt());
}

void SerialController::getStopbits(QString stopbits)
{
    if (stopbits == "1")
        serial->setStopBits(QSerialPort::OneStop);
    else if (stopbits == "1.5")
        serial->setStopBits(QSerialPort::OneAndHalfStop);
    else if (stopbits == "2")
        serial->setStopBits(QSerialPort::TwoStop);
}

void SerialController::getDatabits(QString databits)
{
    serial->setDataBits(QSerialPort::DataBits(databits.toInt()));
}

void SerialController::getParity(QString parity)
{
    if (parity == tr("无"))
        serial->setParity(QSerialPort::NoParity);

    else if (parity == tr("奇校验"))
        serial->setParity(QSerialPort::OddParity);

    else if (parity == tr("偶校验"))
        serial->setParity(QSerialPort::EvenParity);
}

void SerialController::closeSerial()
{
    if (serial->isOpen())
    {
        serial->close();
        emit closeSuccess();
    }
}

void SerialController::handleRecv()
{
    QByteArray data = serial->readAll();
    QString str = data;
    double a, d;
    try {
        QStringList sl = str.split("|");
        a = sl[0].toInt();
        d = sl[0].toInt();
        emit sendPos(a, d);
    } catch(...) {
        ;
    }
}

void SerialController::contrloRTS(bool set)
{
    serial->setRequestToSend(set);
}

void SerialController::controlDTR(bool set)
{
    serial->setDataTerminalReady(set);
}
