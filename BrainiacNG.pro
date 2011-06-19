#-------------------------------------------------
#
# Project created by QtCreator 2011-04-25T12:41:25
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = BrainiacNG
TEMPLATE = app


SOURCES += main.cpp\
        gui/mainwindow.cpp \
    core/scene.cpp \
    core/agent/brain/fuzzybase.cpp \
    core/generator/generator.cpp \
    core/generator/pointgenerator.cpp \
    gui/mainwindowlayout.cpp \
    gui/editorlabel.cpp \
    gui/editorgraphicsview.cpp \
    gui/sceneeditor/sceneeditor.cpp \
    core/group/group.cpp \
    core/agent/agentmanager.cpp \
    gui/sceneeditor/sceneeditoritem.cpp \
    gui/sceneeditor/groupeditor.cpp \
    core/brainiacglobals.cpp \
    gui/itemeditorwidgetsbase.cpp \
    core/agent/agent.cpp \
    core/agent/body/body.cpp \
    core/agent/body/segment.cpp \
    core/brainiaccolor.cpp \
    core/agent/channel.cpp \
    core/agent/body/sphere.cpp \
    gui/scenedisplay.cpp \
    core/camera.cpp \
    gui/editorbase.cpp \
    gui/editoritem.cpp \
    gui/braineditor/braineditor.cpp \
    core/agent/brain/output.cpp \
    core/agent/brain/brain.cpp \
    gui/braineditor/outputeditor.cpp \
    gui/brainiacsliderframe.cpp \
    gui/brainiacslider.cpp \
    gui/braineditor/braineditoritem.cpp \
    core/simulation.cpp \
    core/agent/brain/input.cpp \
    gui/braineditor/inputeditor.cpp \
    core/agent/brain/noise.cpp \
    gui/editoritemconnector.cpp \
    core/agent/brain/fuzzyand.cpp \
    core/agent/brain/fuzzyor.cpp

HEADERS  += gui/mainwindow.h \
    core/scene.h \
    core/agent/brain/fuzzybase.h \
    core/generator/generator.h \
    core/generator/pointgenerator.h \
    gui/mainwindowlayout.h \
    gui/editorlabel.h \
    gui/editorgraphicsview.h \
    gui/sceneeditor/sceneeditor.h \
    core/group/group.h \
    core/agent/agentmanager.h \
    gui/sceneeditor/sceneeditoritem.h \
    gui/sceneeditor/groupeditor.h \
    core/brainiacglobals.h \
    gui/itemeditorwidgetsbase.h \
    core/agent/agent.h \
    core/agent/body/body.h \
    core/agent/body/segment.h \
    core/brainiaccolor.h \
    core/agent/channel.h \
    core/agent/body/sphere.h \
    gui/scenedisplay.h \
    core/camera.h \
    gui/editorbase.h \
    gui/editoritem.h \
    gui/braineditor/braineditor.h \
    core/agent/brain/output.h \
    core/agent/brain/brain.h \
    gui/braineditor/outputeditor.h \
    gui/brainiacsliderframe.h \
    gui/brainiacslider.h \
    gui/braineditor/braineditoritem.h \
    core/simulation.h \
    core/agent/brain/input.h \
    gui/braineditor/inputeditor.h \
    core/agent/brain/noise.h \
    gui/editoritemconnector.h \
    core/agent/brain/fuzzyand.h \
    core/agent/brain/fuzzyor.h

FORMS    += gui/mainwindow.ui \
    gui/sceneeditor/groupeditor.ui \
    gui/braineditor/outputeditor.ui \
    gui/braineditor/inputeditor.ui

LIBS += -framework \
    glut

RESOURCES += \
    brainiac.qrc
