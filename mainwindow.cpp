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
    Serial = new QSerialPort(this);
    Serial-> setPortName("/dev/ttyACM0");
    Serial-> setBaudRate(QSerialPort :: Baud115200);
    Serial-> setDataBits(QSerialPort :: Data8);
    Serial-> setParity(QSerialPort :: NoParity);
    Serial-> setStopBits(QSerialPort :: OneStop);
    Serial-> setFlowControl(QSerialPort :: NoFlowControl);
    Serial-> open(QIODevice :: ReadWrite);
    Serial-> write("help\n");
    connect(Serial, SIGNAL(readyRead()), this, SLOT(serialReceived()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::serialReceived()
{
    QByteArray ba;
    ba = Serial->readAll();
    ui->label->setText(ba);
    qDebug() << ba;
}
