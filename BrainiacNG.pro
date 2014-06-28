#-------------------------------------------------
#
# Project created by QtCreator 2011-04-25T12:41:25
#
#-------------------------------------------------


QT += core gui
QT += concurrent
QT += opengl
QT += widgets
QT += xmlpatterns

TARGET = BrainiacNG
TEMPLATE = app


include ( BrainiacCommonSources.pri )
include ( BrainiacCommonForms.pri )

SOURCES += brainiac/main.cpp

RESOURCES += \
    brainiac.qrc

OSG_INC_DIR = $$(BRAINIAC_OSG_INC_PATH)
OSG_LIB_DIR = $$(BRAINIAC_OSG_LIB_PATH)

isEmpty(OSG_INC_DIR){
  OSG_INC_DIR = /usr/local/include
}

isEmpty(OSG_LIB_DIR){
  OSG_LIB_DIR = /usr/local/lib
}
message(OSG Include dir $$OSG_INC_DIR)
message(OSG Lib dir $$OSG_LIB_DIR)

macx: LIBS += -L $$OSG_LIB_DIR -losg -losgQt -losgViewer -losgGA -losgDB -lOpenThreads -losgFX -losgUtil
macx: LIBS += -framework glut
macx: LIBS += -framework QtConcurrent
macx: INCLUDEPATH += $$OSG_INC_DIR
#macx: DEPENDPATH += /usr/local/include

CONFIG(debug, debug|release){
    message(Build mode containes debug: ($$CONFIG))
    unix: LIBS += -L $$OSG_LIB_DIR -losgd -losgQtd -losgViewerd -losgGAd -losgDBd -lOpenThreadsd -losgFXd -losgUtild
}else{
    unix: LIBS += -L $$OSG_LIB_DIR -losg -losgQt -losgViewer -losgGA -losgDB -lOpenThreads -losgFX -losgUtil
}
unix: INCLUDEPATH += $$OSG_INC_DIR
# unix: DEPENDPATH += /usr/include
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

CONFIG(debug, debug|release){
    TARGET=$$join(TARGET,,,d)
}

macx: QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.9
macx: QMAKE_CFLAGS_X86_64 += -Wall
QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64

DEFINES += "BRAINIAC_SUPPRESS_THIRD_PARTY_WARNINGS"

LASTCOMMIT= $$system(git log | head -1 | awk \'{print $2;}\')
DEFINES += "__LASTCOMMIT__=\'\"($$LASTCOMMIT)\"\'"
message( Last Commit ($$LASTCOMMIT). )


