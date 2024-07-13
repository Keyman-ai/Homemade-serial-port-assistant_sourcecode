#include "widget.h"
#include "ui_widget.h"
#include <QSerialPortInfo>
#include <QMessageBox>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QStringList serialNamePort;

    serialPort = new QSerialPort(this);

    //关联发送和面板框
    connect(serialPort, SIGNAL(readyRead()),this,SLOT(serialPortReadyRead_Slot()));

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        serialNamePort<<info.portName();
    }
    ui->serialCb->addItems(serialNamePort);



}

Widget::~Widget()
{
    delete ui;
}

void Widget::serialPortReadyRead_Slot()
{
    QString buf;
    buf = QString(serialPort->readAll());
    ui->receiveEdit->appendPlainText(buf);
}


void Widget::on_openBt_clicked()
{
    //初始化串口
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity checkBits;

    //进行赋值
    if(ui->boundrateCb->currentText() == "4800"){
        baudRate = QSerialPort::Baud4800;
    }else if(ui->boundrateCb->currentText() == "9600"){
        baudRate = QSerialPort::Baud9600;
    }else if(ui->boundrateCb->currentText() == "115200"){
        baudRate = QSerialPort::Baud115200;
    }

    if(ui->dataCb->currentText() == "5"){
        dataBits = QSerialPort::Data5;
    }else if(ui->dataCb->currentText() == "6"){
        dataBits = QSerialPort::Data6;
    }else if(ui->dataCb->currentText() == "7"){
        dataBits = QSerialPort::Data7;
    }else if(ui->dataCb->currentText() == "8"){
        dataBits = QSerialPort::Data8;
    }

    if(ui->stopCb->currentText() == "1"){
        stopBits = QSerialPort::OneStop;
    }else if(ui->stopCb->currentText() == "1.5"){
        stopBits = QSerialPort::OneAndHalfStop;
    }else if(ui->stopCb->currentText() == "2"){
        stopBits = QSerialPort::TwoStop;
    }

    if(ui->checkCb->currentText() == "None"){
        checkBits = QSerialPort::NoParity;
    }

    //初始化串口，设值
    serialPort->setPortName(ui->serialCb->currentText());
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setStopBits(stopBits);
    serialPort->setParity(checkBits);


    //判断是否打开成功
    if(serialPort->open(QIODevice::ReadWrite) == true){
        QMessageBox::information(this, "提示", "成功");
    }else{
        QMessageBox::critical(this, "提示", "失败");
    }

}

void Widget::on_closeBt_clicked()
{
    serialPort->close();
    QMessageBox::information(this, "提示", "串口关闭成功");
}


void Widget::on_sendBt_clicked()
{
    serialPort->write(ui->sendEdit->text().toLocal8Bit().data());
}

void Widget::on_clearBt_clicked()
{
    ui->receiveEdit->clear();
}
