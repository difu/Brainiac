// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <QtGui/QApplication>
#include "gui/mainwindow.h"
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include "core/scene.h"


int main(int argc, char *argv[])
{
    setenv("OSG_GL_EXTENSION_DISABLE","GL_EXT_timer_query GL_ARB_timer_query",0); //!< @bug see http://forum.openscenegraph.org/viewtopic.php?t=11086
    QApplication a(argc, argv);
    const QString gitVersion(__LASTCOMMIT__);
    std::cout << "Brainiac V0.01" << std::endl << std::flush;
    std::cout << "Build from Git commit " << gitVersion.toStdString() << std::endl << std::flush;
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
