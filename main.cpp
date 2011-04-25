#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::cout << "Brainiac V0.01" << std::endl;
    int num = qApp->argc() ;
    for ( int i = 0; i < num; i++ ) {
        QString s = qApp->argv()[i] ;
        if ( s.startsWith( "--batch" ) ) {
            std::cout << "Running in batch mode" << std::endl;
            return a.exec();
        } else if ( s.startsWith( "-t" ) ) {
            // do wat you want with t arg...
        }
    }
    MainWindow w;
    w.show();

    return a.exec();
}
