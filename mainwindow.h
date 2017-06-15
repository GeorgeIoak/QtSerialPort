#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QByteArray>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QGraphicsPixmapItem>
#include <QStringList>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupPins();
    void interrupt();

public slots:
    void replyFinished(QNetworkReply *);

private slots:
    void serialReceived();
    void on_btnSendCmd_clicked();
    void on_btnTake1_clicked();
    void on_btnInit_clicked();
    void showTime();
    void justDoIt();
    void logData();
    void postThingSpeak(QList<quint32> thelist);
    void on_btnQuit_clicked();
    void getTemp();

    void on_tempBar_valueChanged(int value);

    void on_btnHeater_clicked();

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
    QString filename;
    quint16 rxRegAddress;
    QTimer *timer;
    QTimer *takeareading;
    QTimer *getTempTimer;
    int step;
    int readings;
    float    rxRegValue;
    int state;
    int num;
    QUrl url;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

    // needed for displaying image in QGraphicsView of main window
    QGraphicsScene* scene;
    QGraphicsPixmapItem* item;
};
#endif // MAINWINDOW_H
