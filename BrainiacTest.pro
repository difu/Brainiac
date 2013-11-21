QT       += widgets testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += tst_brainiacscenetest.cpp
SOURCES += gui/mainwindow.cpp \
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
    gui/bodyeditor/bodyeditor.cpp \
    gui/bodyeditor/bodyeditoritem.cpp \
    gui/bodyeditor/segmenteditor.cpp \
    core/agent/body/animation/animation.cpp \
    core/agent/body/animation/animationcurve.cpp \
    core/agent/body/animation/animationplayer.cpp \
    gui/Animation/actioneditor.cpp \
    core/agent/body/animation/modifiableanimation.cpp \
    gui/Animation/loopeditorscene.cpp \
    gui/Animation/editorlineitem.cpp \
    core/agent/body/animation/latchcurve.cpp \
    gui/braineditor/andeditor.cpp \
    gui/bodyeditor/bodydisplay_.cpp \
    core/agent/body/bodymanager.cpp \
    core/agent/body/segment.cpp \
    core/agent/body/segmentshape.cpp \
    core/agent/body/bodysegment.cpp \
    core/agent/body/bodysegmentsignalhandler.cpp \
    gui/scenedisplay_.cpp \
    gui/Animation/actiondisplay_.cpp \
    core/agent/body/animation/motiontreemanager.cpp \
    core/brainiacerror.cpp

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
    gui/bodyeditor/bodyeditor.h \
    gui/bodyeditor/bodyeditoritem.h \
    gui/bodyeditor/segmenteditor.h \
    core/agent/body/animation/animation.h \
    core/agent/body/animation/animationcurve.h \
    core/agent/body/animation/animationplayer.h \
    gui/Animation/actioneditor.h \
    core/agent/body/animation/modifiableanimation.h \
    gui/Animation/loopeditorscene.h \
    gui/Animation/editorlineitem.h \
    cml/cml.h \
    core/agent/body/animation/latchcurve.h \
    gui/braineditor/andeditor.h \
    gui/osgmultithreadedqtwidget.h \
    gui/bodyeditor/bodydisplay_.h \
    core/agent/body/bodymanager.h \
    core/agent/body/segment.h \
    core/agent/body/segmentshape.h \
    core/agent/body/bodysegment.h \
    core/agent/body/bodysegmentsignalhandler.h \
    gui/scenedisplay_.h \
    gui/Animation/actiondisplay_.h \
    core/agent/body/animation/motiontreemanager.h \
    core/brainiacerror.h

FORMS    += gui/mainwindow.ui \
    gui/sceneeditor/groupeditor.ui \
    gui/braineditor/outputeditor.ui \
    gui/braineditor/inputeditor.ui \
    gui/braineditor/fuzzyeditor.ui \
    gui/braineditor/defuzzeditor.ui \
    gui/braineditor/noiseeditor.ui \
    gui/placeeditor.ui \
    gui/bodyeditor/segmenteditor.ui \
    gui/Animation/actioneditor.ui \
    gui/braineditor/andeditor.ui

RESOURCES +=  \
    testData.qrc\
    brainiac.qrc


macx: LIBS += -L/usr/local/lib/ -losg -losgQt -losgViewer -losgGA -losgDB -lOpenThreads -losgFX -losgUtil
macx: LIBS += -framework glut
macx: LIBS += -framework QtConcurrent
macx: INCLUDEPATH += /usr/local/include
macx: DEPENDPATH += /usr/local/include


unix: LIBS += -L/usr/lib -losg -losgQt -losgViewer -losgGA -losgDB -lOpenThreads -losgFX -losgUtil
unix: INCLUDEPATH += /usr/include
unix: DEPENDPATH += /usr/include
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

mac: QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.9
QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64

LASTCOMMIT= $$system(git log | head -1 | awk \'{print $2;}\')
DEFINES += "__LASTCOMMIT__=\'\"($$LASTCOMMIT)\"\'"
message( Last Commit ($$LASTCOMMIT). )


