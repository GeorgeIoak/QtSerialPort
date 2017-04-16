#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug("Some test Output");
    ui->setupUi(this);
    ui->dataReceived->clear();
    mySerial = new QSerialPort(this);
    serialBuffer = "";
    parsed_data = "";


   //*  Testing code, prints the description, vendor id, and product id of all ports.
   //*
    qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Port Name: " << serialPortInfo.portName() << "\n";
        qDebug() << "Description: " << serialPortInfo.description() << "\n";
        qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
        qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
        qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
        qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";
    }

    bool port_is_available = false;
    QString port_name;

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        //  check if the serialport has both a product identifier and a vendor identifier
                if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
                    //  check if the product ID and the vendor ID match those of the smoothie
                    if((serialPortInfo.productIdentifier() == fluosense_product_id)
                            && (serialPortInfo.vendorIdentifier() == fluosense_vendor_id)){
                        port_is_available = true; //    fluosense is available on this port
                        port_name = serialPortInfo.portName();
                    }
                }
    }

    /*
     * Open and Configure the Serial Port
     */

    if(port_is_available){
        qDebug() << "found the Detector...\n";
        mySerial-> setPortName(port_name);
        mySerial-> open(QSerialPort::ReadWrite);
        mySerial-> setBaudRate(QSerialPort::Baud57600);
        mySerial-> setDataBits(QSerialPort::Data8);
        mySerial-> setParity(QSerialPort::NoParity);
        mySerial-> setStopBits(QSerialPort::OneStop);
        mySerial-> setFlowControl(QSerialPort::NoFlowControl);
        //Serial-> write("config-get sd acceleration\n");
        QObject::connect(mySerial, SIGNAL(readyRead()), this, SLOT(serialReceived()));
    }else{
        qDebug() << "Couldn't find the correct port for the Detector.\n";
        QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to Detector.");
    }
}

MainWindow::~MainWindow()
{
    if(mySerial->isOpen()){
            mySerial->close();
    }
    delete ui;
}

void MainWindow::serialReceived()
{
    QByteArray ba;
    while (mySerial->canReadLine()){
        ba = mySerial->readLine();
    }
    //ba = mySerial->readAll();
    ui->dataReceived->setText(ba);
    qDebug() << ba;
}

void MainWindow::on_btnSendCmd_clicked()
{
    QByteArray cmdText;
    cmdText.append(ui->cmdEntered->text());
    cmdText.append("\r\n");
    ui->dataReceived->clear();
    qDebug() << cmdText;
    mySerial->write(cmdText);
    //mySerial->flush();
    cmdText.clear();
}
