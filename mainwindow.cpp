#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "flurosens.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QtWidgets>
#include <QDateTime>
#include <QThread>
#include <string>
#include <QModbus/QAsciiModbus>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000); //Update clock every second

    takeareading = new QTimer();
    connect(takeareading, SIGNAL(timeout()), SLOT(justDoIt()));

    ui->dataReceived->clear();
    mySerial = new QSerialPort(this);
    serialBuffer = "";
    parsed_data = "";

    // QModbus Test Code
   // bool myPort = QAsciiModbus::open("/dev/ttyUSB0", BR57600, BPC8, OneStopBit, NoParity, NoFlowControl);

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
                    if((serialPortInfo.productIdentifier() == fluosens_product_id)
                            && (serialPortInfo.vendorIdentifier() == fluosens_vendor_id)){
                        port_is_available = true; //    fluosens is available on this port
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
    qDebug() << "Received Response is: " << ba;
    qDebug() << "Function Code Returned is: " << ba.mid(3, 2);
    if (ba.mid(3, 2) == "03")
    {
        qDebug() << "Number of Bytes is: " << ba.mid(5, 2);
    }

    // Get the hex decoded form.
    QByteArray pdu;
    pdu = QByteArray::fromHex( ba.mid( 1 , ba.size() -5 ) );

    // Read device address and command ID and control them.
    quint8 rxDeviceAddress , rxFunctionCode , byteCount;
    quint16 rxRegAddress;
    float    rxRegValue;
    QDataStream rxStream( pdu );
    rxStream.setByteOrder( QDataStream::BigEndian );

    if (ba.mid(3, 2) == "03") {
        rxStream >> rxDeviceAddress >> rxFunctionCode >> byteCount;
        // It is ok, read and convert the data.
        QList<quint32> list; //was quint16
        quint32 tmp; //was quint16
        for ( int i = 0 ; i < byteCount/4 ; i++ )
        {
            rxStream >> tmp;
            list.append( tmp );
            //qDebug() << "list is: " << list[i] << ", " << QByteArray::number(list[i]).toHex();
            QString hexvalue = QString("0x%1").arg(list[i], 2, 16, QLatin1Char( '0' ));
            rxRegValue = list[i] * 0.000298; //convert to mV
            qDebug() << "list is: " << list[i] << ", " << rxRegValue << ", " << hexvalue;
            qDebug() << "The current time is " << QTime::currentTime().toString("hh:mm:ss");
        }
    } else {
        rxStream >> rxDeviceAddress >> rxFunctionCode >> rxRegAddress;
    }

}

void MainWindow::on_btnSendCmd_clicked()
{
    QByteArray cmdText;
    cmdText.append(ui->cmdEntered->text());
    // Convert to binary
    QByteArray theCMD = QByteArray::fromHex(cmdText);
    // Step 1, sum bytes (relies on char losing any overflow)
    char LRC = 0;
    foreach (char c, theCMD)
        LRC += c;
        // steps 2 and 3: negate result
        LRC = -LRC;

    qDebug() << "Input    =" << theCMD.toHex().toUpper();
    theCMD.append(LRC);
    theCMD = theCMD.toHex().toUpper();
    theCMD.prepend(":"); //Start Frame
    theCMD.append("\r\n"); // Fluo Sens needs CRLF

    ui->dataReceived->clear();
    qDebug() << "theCMD is " << theCMD;
    mySerial->write(theCMD);
    //mySerial->flush();
    cmdText.clear();
    theCMD.clear();
}

void MainWindow::on_btnTake1_clicked()
{
    int timeBetween = ui->spinBoxTime->value() * 1000;
    takeareading->start(timeBetween);
    qDebug() << "Time Between Readings is in ms " << timeBetween;
    int readings = ui->spinBoxReadings->value();
    //for ( int i = 0 ; i < readings ; i++ )
    //{
    //    QTimer::singleShot(timeBetween, this, SLOT(justDoIt()));
    //    qDebug() << "i is " << i;
    //}
}

void MainWindow::justDoIt()
{
        mySerial->write(":000602000001F7\r\n"); //Start Measurement
        QThread::msleep(100);
        mySerial->write(":000301000010EC\r\n"); //Read On/Off Value Registers
}

void MainWindow::on_btnInit_clicked()
{
    mySerial->write(":000600000001F9\r\n"); //Set to 1 Cycle
    QThread::msleep(500);
    qDebug() << "Sent Cycle";
    mySerial->write(":000600010001F8\r\n"); //Set cycle time to 1 sec
    QThread::msleep(500);
    qDebug() << "Sent Time";
    mySerial->write(":000600020000F8\r\n"); //Set Start Mode to On Command
    QThread::msleep(500);
    qDebug() << "Sent Mode";
    mySerial->write(":000600030700F0\r\n"); //Set Method Type to E1+E1D2+E2D2
    qDebug() << "Sent Type";
}

void MainWindow::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    if ((time.second() % 2) == 0)
        text[5] = ' ';
    ui->label_DateTime->setText(text);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == takeareading->timerId()) {
        ++step;
        qDebug() << "I'm in the timer event";
        //update();
        if (step > readings)
            takeareading->stop();
    } else {
        QWidget::timerEvent(event);
    }
}
