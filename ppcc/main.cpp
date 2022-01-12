#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setStyle("windowsvista");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
