#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
QSerialPort *Serial;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug("Some test Output");
    ui->setupUi(this);
    ui->label->clear();


     *  Testing code, prints the description, vendor id, and product id of all ports.
     *  
    qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Description: " << serialPortInfo.description() << "\n";
        qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
        qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
        qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
        qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";
    }


    Serial = new QSerialPort(this);
    Serial-> setPortName("/dev/ttyACM0");
    Serial-> setBaudRate(QSerialPort :: Baud115200);
    Serial-> setDataBits(QSerialPort :: Data8);
    Serial-> setParity(QSerialPort :: NoParity);
    Serial-> setStopBits(QSerialPort :: OneStop);
    Serial-> setFlowControl(QSerialPort :: NoFlowControl);
    Serial-> open(QIODevice :: ReadWrite);
    //Serial-> write("config-get sd acceleration\n");
    connect(Serial, SIGNAL(readyRead()), this, SLOT(serialReceived()));
}

MainWindow::~MainWindow()
{
    delete ui;
    Serial->close();
}

void MainWindow::serialReceived()
{
    QByteArray ba;
    ba = Serial->readAll();
    ui->label->setText(ba);
    qDebug() << ba;
}

void MainWindow::on_pushButton_clicked()
{
    //Serial-> write("config-get sd alpha_steps_per_mm\n");
    QByteArray cmdText;
    cmdText.append(ui->lineEdit->text());
    cmdText.append("\r");
    ui->label->clear();
    qDebug() << cmdText;
    Serial->write(cmdText);
    Serial->flush();
    cmdText.clear();
}
