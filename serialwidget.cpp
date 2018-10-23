#include "serialwidget.h"

SerialWidget::SerialWidget(QWidget *parent) : QWidget(parent)
{

    setWindowTitle(tr("Control panel"));

    radar = new Radar();
    radar->show();
    connect(this, &SerialWidget::deliverPos, radar, &Radar::getPos);

    wave = new Waveform();
    wave->show();
    connect(this, &SerialWidget::deliverPos, wave, &Waveform::getPos);

    serialController = new SerialController;
    serialController->moveToThread(&SerialThr);
    SerialThr.start();
    //      BaudrateList << "115200";
    BaudrateList << "256000"
                 << "230400"
                 << "128000"
                 << "115200"
                 << "76800"
                 << "57600"
                 << "43000"
                 << "38400"
                 << "19200"
                 << "14400"
                 << "9600"
                 << "4800"
                 << "2400"
                 << "1200";
    StopbitsList << "1"
                 << "1.5"
                 << "2";
    DatabitsList << "8"
                 << "7"
                 << "6"
                 << "5";
    ParityList << tr("无") << tr("奇校验") << tr("偶校验");

    //初始化ComboBox
    COMBox = new QComboBox();
    BaudrateBox = new QComboBox();
    BaudrateBox->addItems(BaudrateList);
    BaudrateBox->setCurrentIndex(3);
    StopbitsBox = new QComboBox();
    StopbitsBox->addItems(StopbitsList);
    //    StopbitsBox->setCurrentIndex(settings.value("Stop bits", 0).toInt());
    DatabitsBox = new QComboBox();
    DatabitsBox->addItems(DatabitsList);
    //    DatabitsBox->setCurrentIndex(settings.value("Data bits", 0).toInt());
    ParityBox = new QComboBox();
    ParityBox->addItems(ParityList);

    //初始化label
    COMLabel = new QLabel(tr("串口号"));
    BaudrateLabel = new QLabel(tr("波特率"));
    StopbitsLabel = new QLabel(tr("停止位"));
    DatabitsLabel = new QLabel(tr("数据位"));
    ParityLabel = new QLabel(tr("校验位"));

    //LineEdit
    QDoubleValidator *validator = new QDoubleValidator(0, 100, 2, this);

    sendArea = new QLineEdit(this);
    sendArea->setValidator(validator);
    sendArea->setText(tr("0"));

    //按钮
    OpenButton = new QPushButton(tr("打开串口"));
    calibrationButton = new QPushButton(tr("Calibrate Mode"));
    calibrationButton->setDisabled(true);
    //校准
    connect(calibrationButton, &QPushButton::clicked, this, &SerialWidget::Calibrate);

    //checkbox
    RTSBox = new QCheckBox(tr("RTS"));
    DTRBox = new QCheckBox(tr("DTR"));

    connect(RTSBox, &QCheckBox::stateChanged, this, [this](int state){
        if(isOpened)
        {
            if(state == 2)
                emit changeRTS(true);
            else if(state == 0)
                emit changeRTS(false);
        }
    });
    connect(DTRBox, &QCheckBox::stateChanged, this, [this](int state){
        if(isOpened)
        {
            if(state == 2)
                emit changeDTR(true);
            else if(state == 0)
                emit changeDTR(false);
        }
    });


    connect(OpenButton, &QPushButton::clicked, this, &SerialWidget::OpenSerial);
    connect(this, &SerialWidget::requestClose, serialController, &SerialController::closeSerial);
    connect(this, &SerialWidget::sendData, serialController,&SerialController::writeData);


    //初始化布局

    bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(RTSBox);
    bottomLayout->addWidget(DTRBox);

    leftLlayout = new QFormLayout();
    leftLlayout->addRow(BaudrateLabel, BaudrateBox);
    leftLlayout->addRow(StopbitsLabel, StopbitsBox);
    leftLlayout->addRow(DatabitsLabel, DatabitsBox);
    leftLlayout->addRow(ParityLabel, ParityBox);
    leftLlayout->addRow(COMLabel, COMBox);
    leftLlayout->addRow(sendArea);
    leftLlayout->addRow(calibrationButton);
    leftLlayout->addRow(OpenButton);
    leftLlayout->setAlignment(OpenButton, Qt::AlignVCenter);
    leftLlayout->setMargin(30);
    leftLlayout->setHorizontalSpacing(20);
    leftLlayout->setVerticalSpacing(60);

    cvlayout = new QVBoxLayout();
    cvlayout->addLayout(bottomLayout);
    cvlayout->addLayout(leftLlayout);



    setLayout(cvlayout);

    CheckTimer = new QTimer(this);
    CheckTimer->start(1000);
    connect(CheckTimer, &QTimer::timeout, this, &SerialWidget::CheckSerials);

    connect(this, &SerialWidget::requestOpen, serialController, &SerialController::openSerial);
    connect(this, &SerialWidget::setBaudRate, serialController, &SerialController::getBaudrate);
    connect(this, &SerialWidget::setStopBits, serialController, &SerialController::getStopbits);
    connect(this, &SerialWidget::setDataBits, serialController, &SerialController::getDatabits);
    connect(this, &SerialWidget::setParity, serialController, &SerialController::getParity);

    connect(serialController, &SerialController::openSuccess, this, &SerialWidget::serialOpened);
    connect(serialController, &SerialController::openFailed, this, &SerialWidget::serialNotOpened);
    connect(serialController, &SerialController::closeSuccess, this, &SerialWidget::serialClosed);
    connect(serialController, &SerialController::sendPos, this, &SerialWidget::getRecv);



    connect(RTSBox, &QCheckBox::stateChanged, this, [this](int state){
        if(isOpened)
        {
            if(state == 2)
                emit changeRTS(true);
            else if(state == 0)
                emit changeRTS(false);
        }});
    connect(DTRBox, &QCheckBox::stateChanged, this, [this](int state){
        if(isOpened)
        {
            if(state == 2)
                emit changeDTR(true);
            else if(state == 0)
                emit changeDTR(false);
        }
    });

    connect(this, &SerialWidget::changeRTS, serialController, &SerialController::contrloRTS);
    connect(this, &SerialWidget::changeDTR, serialController, &SerialController::controlDTR);

    connect(this, &SerialWidget::sendSoundSpeed, radar, &Radar::setSoundSpeed);
    connect(this, &SerialWidget::sendSoundSpeed, wave, &Waveform::setSoundSpeed);


}

void SerialWidget::CheckSerials()
{
    QList<QSerialPortInfo> SerialList = QSerialPortInfo::availablePorts();
    if (!SerialList.isEmpty())
    {
        QStringList TmpComList, TmpPortNameList, TmpDesList;
        for (QSerialPortInfo serial : SerialList)
        {
            TmpComList << serial.portName() + " " + serial.description();
            TmpPortNameList << serial.portName();
            TmpDesList << serial.description();
        }
        if (COMList != TmpComList)
        {
            //TODO: 可用串口发生改变却不为空时，保持串口打开状态，及其他细节的处理
            //只在串口发生变化时刷新ComboBox
            COMList = TmpComList;
            PortNameList = TmpPortNameList;
            DescList = TmpDesList;
            COMBox->setDisabled(false);
            COMBox->clear();
            COMBox->addItems(PortNameList);
            OpenButton->setDisabled(false);
            for (int i = 0; i < COMList.count(); i++)   //为串口列表增加ToolTip
            {
                COMBox->setItemData(i, DescList[i], Qt::ToolTipRole);
            }
        }
    }
    else    //可用串口为空时发送关闭串口信号
    {
        COMBox->clear();
        COMList.clear();
        COMBox->addItem(tr("(空)"));
        COMBox->setDisabled(true);
        OpenButton->setDisabled(true);
        //        emit CloseSerial();
        isOpened = false;
    }

}

SerialWidget::~SerialWidget()
{
    SerialThr.terminate();
    radar->close();
}


void SerialWidget::serialOpened()
{
    isOpened = true;
    //串口关闭时无法设置RTS,DTR信号（但此时checkbox仍然是可操作的），串口打开时发送stateChanged信号来应用改变
    emit RTSBox->stateChanged(RTSBox->checkState());
    emit DTRBox->stateChanged(DTRBox->checkState());
    //相应控件可用性做出改变(setDisabled)
    ACtionAttachToSerial(true);
}
void SerialWidget::serialNotOpened()
{
    QMessageBox::warning(this,tr("Warning"),tr("Failed to open serial port!"));
}

void SerialWidget::serialClosed()
{
    isOpened = false;
    //相应控件可用性做出改变(setDisabled)
    ACtionAttachToSerial(false);
}

void SerialWidget::getRecv(int ang, int dis)
{
    emit deliverPos(ang, dis);
}

void SerialWidget::OpenSerial()
{
    QString portName = COMBox->currentText();
    emit requestOpen(portName);
    emit setBaudRate(BaudrateBox->currentText());
    emit setStopBits(StopbitsBox->currentText());
    emit setDataBits(DatabitsBox->currentText());
    emit setParity(ParityBox->currentText());
}

void SerialWidget::CloseSerial()
{
    emit requestClose();

}

void SerialWidget::Calibrate()
{
    if(calibrationButton->text()==tr("Calibrate Mode")){
        emit sendData(tr("S"));
        calibrationButton->setText(tr("Confirm"));
    } else{
        emit sendData(tr("R"));
        calibrationButton->setText(tr("Calibrate Mode"));
        emit sendSoundSpeed(sendArea->text().toDouble());
    }
}

void SerialWidget::ACtionAttachToSerial(bool set)
{
    //根据串口开关状态决定一些控件的可用性
    if(set)
    {
        OpenButton->setText(tr("关闭串口"));
        COMBox->setDisabled(true);
        //SendButton->setDisabled(false);
        disconnect(OpenButton, &QPushButton::clicked, this, &SerialWidget::OpenSerial);
        connect(OpenButton, &QPushButton::clicked, this, &SerialWidget::CloseSerial);
        calibrationButton->setDisabled(false);
    }
    else
    {
        OpenButton->setText(tr("打开串口"));
        COMBox->setDisabled(false);
        //SendButton->setDisabled(true);
        disconnect(OpenButton, &QPushButton::clicked, this, &SerialWidget::CloseSerial);
        connect(OpenButton, &QPushButton::clicked, this, &SerialWidget::OpenSerial);
        calibrationButton->setDisabled(true);
    }
}
