#-------------------------------------------------
#
# Project created by QtCreator 2011-04-25T12:41:25
#
#-------------------------------------------------

QT       += core gui opengl qt warn_on qt3d
CONFIG += qt3d
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
    core/brainiaccolor.cpp \
    core/agent/channel.cpp \
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
    core/agent/brain/fuzzyor.cpp \
    core/agent/brain/fuzzyfuzz.cpp \
    core/agent/brain/fuzzydefuzz.cpp \
    gui/braineditor/fuzzyeditor.cpp \
    gui/braineditor/defuzzeditor.cpp \
    core/agent/brain/timer.cpp \
    core/idgenerator.cpp \
    gui/braineditor/noiseeditor.cpp \
    gui/braineditor/fuzzyeditorcurveeditor.cpp \
    gui/braineditor/fuzzyeditorcurvepoint.cpp \
    core/generator/locator.cpp \
    core/agent/body/skeletonnode.cpp \
    core/agent/body/skeletonnodesphere.cpp \
    core/agent/body/skeletonnodebox.cpp \
    gui/bodyeditor/bodyeditor.cpp \
    gui/bodydisplay.cpp

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
    core/brainiaccolor.h \
    core/agent/channel.h \
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
    core/agent/brain/fuzzyor.h \
    core/agent/brain/fuzzyfuzz.h \
    core/agent/brain/fuzzydefuzz.h \
    gui/braineditor/fuzzyeditor.h \
    gui/braineditor/defuzzeditor.h \
    core/agent/brain/timer.h \
    core/idgenerator.h \
    gui/braineditor/noiseeditor.h \
    gui/braineditor/fuzzyeditorcurveeditor.h \
    gui/braineditor/fuzzyeditorcurvepoint.h \
    core/generator/locator.h \
    core/agent/body/skeletonnode.h \
    core/agent/body/skeletonnodesphere.h \
    core/agent/body/skeletonnodebox.h \
    gui/bodyeditor/bodyeditor.h \
    gui/bodydisplay.h

FORMS    += gui/mainwindow.ui \
    gui/sceneeditor/groupeditor.ui \
    gui/braineditor/outputeditor.ui \
    gui/braineditor/inputeditor.ui \
    gui/braineditor/fuzzyeditor.ui \
    gui/braineditor/defuzzeditor.ui \
    gui/braineditor/noiseeditor.ui \
    gui/placeeditor.ui

LIBS += -framework \
    glut

RESOURCES += \
    brainiac.qrc

LASTCOMMIT= $$system(git log | head -1 | awk \'{print $2;}\')
message( Last Commit ($$LASTCOMMIT). )


