#-------------------------------------------------
#
# Project created by QtCreator 2011-04-25T12:41:25
#
#-------------------------------------------------

QT       += core gui

TARGET = BrainiacNG
TEMPLATE = app


SOURCES += main.cpp\
        gui/mainwindow.cpp \
    core/scene.cpp \
    gui/mainwindowlayout.cpp \
    gui/editorlabel.cpp \
    core/generator/generator.cpp \
    core/generator/pointgenerator.cpp

HEADERS  += gui/mainwindow.h \
    core/scene.h \
    gui/mainwindowlayout.h \
    gui/editorlabel.h \
    core/generator/generator.h \
    core/generator/pointgenerator.h

FORMS    += gui/mainwindow.ui

RESOURCES += \
    brainiac.qrc
