#-------------------------------------------------
#
# Project created by QtCreator 2013-11-23T13:12:18
#
#-------------------------------------------------

QT       += core gui #standard
QT       += opengl xml #qgl extensions

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bodyprj
TEMPLATE = app
CONFIG += qt opengl

#path to where is the libqglviewer root
LIBQGL_PATH = "C:/Program Files (x86)/libQGLViewer"

SOURCES += main.cpp\
        mainwindow.cpp \
    viewer.cpp

HEADERS  += mainwindow.h \
    viewer.h

FORMS    += mainwindow.ui

# LibQGL stuff
win32:INCLUDEPATH += $$LIBQGL_PATH/sources
win32:DEPENDPATH += $$LIBQGL_PATH/sources
macx:INCLUDEPATH += $$LIBQGL_PATH
DEFINES *= QT_DLL QT_THREAD_SUPPORT
#fix for WFlags deprecated in qt 5.1
QT_VERSION=$$[QT_VERSION]
contains( QT_VERSION, "^5\.1.*" ) {
    win32:QMAKE_CXXFLAGS += -DWFlags=WindowFlags
    macx:QMAKE_CXXFLAGS += -DWFlags=WindowFlags#?? or build with qt 4.8 or maybe qt 5.0
}

release {
    win32:LIBS += -L$$LIBQGL_PATH/lib/QGLViewer -lQGLViewer2
    macx:LIBS += -F$$LIBQGL_PATH/../Library/Frameworks -framework QGLViewer
}

debug {
    win32:LIBS += -L$$LIBQGL_PATH/lib/QGLViewer -lQGLViewerd2
    #not sure what to put here for debug on mac
    macx:LIBS += -F$$LIBQGL_PATH/../Library/Frameworks -framework QGLViewer
}
