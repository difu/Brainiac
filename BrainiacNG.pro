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
    core/agent/brain/fuzzybase.cpp

HEADERS  += gui/mainwindow.h \
    core/scene.h \
    core/agent/brain/fuzzybase.h

FORMS    += gui/mainwindow.ui
