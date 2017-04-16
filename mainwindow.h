#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QByteArray>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void serialReceived();

    void on_btnSendCmd_clicked();

private:
    Ui::MainWindow *ui;

    QSerialPort *mySerial;
    static const quint16 smoothie_vendor_id = 7504;
    static const quint16 smoothie_product_id = 24597;
    static const quint16 fluosens_vendor_id = 1027;
    static const quint16 fluosens_product_id = 24577;
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
};

#endif // MAINWINDOW_H
