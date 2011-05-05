#include <QtGui/QApplication>
#include "gui/mainwindow.h"
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include "core/scene.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::cout << "Brainiac V0.01" << std::endl << std::flush;

    // Apply stylesheet
    QFile file(":/gui/brainiacStyleSheet.style");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 1;
    QTextStream in(&file);
    QString styleSheet;
    while (!in.atEnd()) {
        styleSheet += in.readLine();
    }
    a.setStyleSheet(styleSheet);

    // Parse parameters
    int num = qApp->argc() ;
    bool batchMode=false;
    QString sceneFileName;

    Scene *theScene=new Scene();

    for ( int i = 0; i < num; i++ ) {
        QString s = qApp->argv()[i] ;
        if ( s.startsWith( "--batch" ) ) {
            qDebug() << "Running in batch mode";
            batchMode=true;
        } else if ( s.startsWith( "--scene" ) ) {
            if( i+1 < num )  { // do we have a next argument?
                sceneFileName=qApp->argv()[i+1];
                qDebug() << "loading scene file "<< sceneFileName;
                if( !theScene->openConfig(sceneFileName) ) {
                    qCritical() << "Error while opening scene file!";
                    return 1;
                }
            } else {
                qCritical() << "Error:Wrong arguments";
                return 1;
            }
        }
    }


    if(!batchMode) {
        MainWindow *w = new MainWindow(theScene);
        w->show();
    }

    return a.exec();
}
