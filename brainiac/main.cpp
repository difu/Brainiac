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

#include <QtWidgets/QApplication>
#include "gui/mainwindow.h"
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QCommandLineParser>
#include "core/scene.h"
#include "core/simulation.h"
#include "core/simulationsettings.h"
#include "core/brainiacerror.h"


int main(int argc, char *argv[])
{
    setenv("OSG_GL_EXTENSION_DISABLE","GL_EXT_timer_query GL_ARB_timer_query",0); //!< @bug see http://forum.openscenegraph.org/viewtopic.php?t=11086
    QApplication a(argc, argv);
    QLoggingCategory::setFilterRules("*.debug=false\n");

    const QString gitVersion(__LASTCOMMIT__);
    a.setApplicationName("Brainiac");
    a.setApplicationVersion("0.01 "%gitVersion);
    a.setOrganizationDomain("com");
    a.setOrganizationName("Braniac");

    QCommandLineParser parser;
    parser.setApplicationDescription("Crowd simulator");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption noGuiOption(QStringList() << "nogui", "Do not show gui.");
    parser.addOption(noGuiOption);

    QCommandLineOption sceneFileOption(QStringList() << "scene","Load scene <file>.","file");
    parser.addOption(sceneFileOption);
    QCommandLineOption simulationOption(QStringList() << "simulation","Runs a simulation in frame range <start-end>.","start-end");
    parser.addOption(simulationOption);
    QCommandLineOption windowPosOption(QStringList() << "window","Open window at <x,y,w,h>.","x,y,w,h");
    parser.addOption(windowPosOption);
    QCommandLineOption numOfThreadsOption(QStringList() << "threads","Sets the maximum number of threads <numOfThreads>.","numOfThreads");
    parser.addOption(numOfThreadsOption);



    // Process the actual command line arguments given by the user
    parser.process(a);

    QString sceneFileName=parser.value(sceneFileOption);

    int x,y,width,height;
    bool windowPosSet=parser.isSet(windowPosOption);
    if(windowPosSet) {
        QString winPos=parser.value(windowPosOption);
        QStringList winPosList=winPos.split(",", QString::SkipEmptyParts);
        if(winPosList.count()!=4) {
            std::cerr << "Error: wrong number or format for parameter window. " << std::endl << std::flush;
            exit(1);
        }

        x=winPosList.at(0).toInt();
        y=winPosList.at(1).toInt();
        width=winPosList.at(2).toInt();
        height=winPosList.at(3).toInt();
        if(width<600) {
            width=600;
        }
        if(height<800) {
            height=800;
        }
    }

    if(parser.isSet(numOfThreadsOption)) {
        int numOfThreads=parser.value(numOfThreadsOption).toInt();
        if(numOfThreads>0) {
            QThreadPool::globalInstance()->setMaxThreadCount(numOfThreads);
        } else {
            std::cerr << "Error: number of threads must be greater or even one. " << std::endl << std::flush;
            exit(1);
        }
    }

    std::cout << "Starting Brainiac with " << QThreadPool::globalInstance()->maxThreadCount() << " threads" << std::endl << std::flush;
    Scene theScene;
//    QThread sceneThread; //!<  thread that runs the scene

//    theScene.moveToThread(&sceneThread);
//    sceneThread.start();

    if(!sceneFileName.isEmpty()) {
        if(!theScene.openConfig(sceneFileName)) {
            BrainiacError::printLastError();
            exit(1);
        }
    }

    if(!parser.isSet(noGuiOption)) {
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



        MainWindow *w = new MainWindow(&theScene);
        if(windowPosSet) {
            w->resize(width,height);
            w->move(x,y);
        }
        w->show();
    }

    if(parser.isSet(simulationOption)) {
        if(parser.values(simulationOption).count()==1) {
            QString simFrames=parser.value(simulationOption);
            QStringList simFrameList=simFrames.split("-", QString::SkipEmptyParts);
            if(simFrameList.count()==2) {
                int startFrame=simFrameList.at(0).toInt();
                int endFrame=simFrameList.at(1).toInt();
                if(startFrame>=endFrame || endFrame < 1 ) {
                    std::cerr << "Error: wrong parameters for simulation frames. " << std::endl << std::flush;
                    exit(1);
                }
                theScene.getSimulation()->getSettings()->setStartEndFrame(startFrame,endFrame);
            }
        }
        theScene.getSimulation()->setSimulationMode(Simulation::SIMULATE);
        theScene.getSimulation()->startSimulation();
        if(parser.isSet(noGuiOption)) {
            QObject::connect(theScene.getSimulation(),SIGNAL(stopped()),&a,SLOT(quit()));
        }
    } else {
        if(parser.isSet(noGuiOption)) {
            exit(0);
        }
    }

    return a.exec();

//    sceneThread.terminate();
//    sceneThread.wait();
}
