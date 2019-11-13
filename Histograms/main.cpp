#include "handlerwindow.h"
#include "histdata.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");
    HandlerWindow w1;
    HandlerWindow w2(&w1);
    HandlerWindow w3(&w2);
    w1.show();


    return a.exec();
}
