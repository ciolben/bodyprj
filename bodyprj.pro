#-------------------------------------------------
#
# Project created by QtCreator 2013-11-23T13:12:18
#
#-------------------------------------------------

QT       += core gui #standard for using qt
QT       += opengl xml #qgl extensions

TARGET = bodyprj
TEMPLATE = app
CONFIG += qt opengl

#path to where is the libqglviewer root
win32:LIBQGL_PATH = "C:/Program Files (x86)/libQGLViewer"
macx: {
    LIBS += -framework QGLViewer
    LIBS += -framework OpenGL -framework GLUT
}

SOURCES += main.cpp\
        mainwindow.cpp \
    viewer.cpp \
    Mass_spring_viewer.cpp \
    Mass_spring_system.cpp \
    Cloth.cpp \
    sphere.cpp

HEADERS  += mainwindow.h \
    viewer.h \
    Mass_spring_viewer.h \
    Mass_spring_system.h \
    utils/vec2.h \
    utils/vec3.h \
    Cloth.h \
    sphere.h \
    object3d.h

FORMS    += mainwindow.ui

# ----LibQGL stuff----
win32:INCLUDEPATH += $$LIBQGL_PATH/sources
win32:DEPENDPATH += $$LIBQGL_PATH/sources
macx:INCLUDEPATH += $$LIBQGL_PATH/Versions/2/Headers
macx:DEPENDPATH += $$LIBQGL_PATH/Versions/2/Headers

# LIB_NAME
win32 {
  LIB_NAME = QGLViewer
  build_pass:CONFIG(debug, debug|release) {
    LIB_NAME = $$join(LIB_NAME,,,d)
  }
  LIB_NAME = $$join(LIB_NAME,,,2)
}

win32:LIBS += -L$$LIBQGL_PATH/lib/QGLViewer -l$$LIB_NAME

#--------------------
