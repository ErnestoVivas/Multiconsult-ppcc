//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[]) {

    //QApplication::setStyle("windowsvista");
    QApplication::setStyle("fusion");
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("PPCC");
    w.show();
    return a.exec();
}
