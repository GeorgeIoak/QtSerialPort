#include "mainwindow.h"
#include <QApplication>

#include <wiringPi.h>

MainWindow *ptWindow;

void setInterrupt(MainWindow *pt);
void interrupt();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    // hide the window title bar, but let the window be closed using Alt + F4
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowCloseButtonHint);
    //w.show();
    w.showFullScreen();

    setInterrupt(&w);

    return a.exec();
}

void setInterrupt(MainWindow *pt)
{
    ptWindow = pt;

    wiringPiISR(5,INT_EDGE_FALLING, &interrupt);
}

void interrupt()
{
    ptWindow->interrupt();
}
