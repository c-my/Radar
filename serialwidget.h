#ifndef SERIALWIDGET_H
#define SERIALWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTextEdit>
#include <QSerialPortInfo>
#include <QTimer>
#include <QMessageBox>
#include <QThread>
#include <QLineEdit>
#include "serialcontroller.h"
#include "radar.h"
#include "waveform.h"

class SerialWidget : public QWidget
{
    Q_OBJECT
public:
    SerialWidget(QWidget *parent = 0);
    void CheckSerials();
    ~SerialWidget();
    //  void SendContent();
    //  bool eventFilter(QObject *watched, QEvent *event);

signals:
    void requestOpen(QString portName);
    void requestClose();
    void setBaudRate(QString baudrate);
    void setStopBits(QString stopbits);
    void setDataBits(QString databits);
    void setParity(QString parity);
    void sendData(QString content);
    void changeRTS(bool set);
    void changeDTR(bool set);

    void deliverPos(int ang,int dis);
    void sendSoundSpeed(double s);

public slots:
    void serialOpened();           //串口打开成功
    void serialNotOpened();        //串口打开失败
    void serialClosed();           //串口关闭
    void getRecv(int ang, int dis); //串口接受数据
    void OpenSerial();
    void CloseSerial();
    void Calibrate();
    //  void ClearRecv();
    //  void ClearSend();
    //  void detNewLine(int state); //处理发送新行
    //  void ControlSendTimer(int state);
    //  void changeSendTimer();
    //  void detHex(int state);     //处理16进制发送
    //  void detRecvHex(int state); //处理16进制接收
    //  void RTSControl(int state);
    //  void DTRControl(int state);

private:
    QComboBox *COMBox, *BaudrateBox, *StopbitsBox, *DatabitsBox, *ParityBox;
    QStringList BaudrateList, StopbitsList, DatabitsList, ParityList;
    QStringList COMList, PortNameList, DescList;
    QLabel *COMLabel, *BaudrateLabel, *StopbitsLabel, *DatabitsLabel, *ParityLabel;
    QCheckBox *RTSBox, *DTRBox;
    QVBoxLayout *cvlayout;
    QHBoxLayout *bottomLayout;
    QFormLayout *leftLlayout;
    QLineEdit *sendArea;
    //  QTextEdit *RecvArea, *SendArea;
    QPushButton *OpenButton, *SendButton, *calibrationButton;
    QTimer *CheckTimer;
    SerialController *serialController;
    QThread SerialThr;

    bool isSendNewLine = false;
    bool isSendHex = false;
    bool isRecvHex = false;
    bool isOpened = false;

    Radar *radar;
    Waveform *wave;

    //  QString HexStringToString(QString hexstr); //解码16进制字符串
    void ACtionAttachToSerial(bool set);
};

#endif // SERIALWIDGET_H
