TEMPLATE = app
TARGET   = bodyprj

INCLUDE_DIR = "C:/Program Files (x86)/libQGLViewer/sources"
LIB_DIR = "C:/Program Files (x86)/libQGLViewer/lib/QGLViewer"

SOURCES += main.cpp\
        mainwindow.cpp \
    viewer.cpp

HEADERS  += mainwindow.h \
    viewer.h

FORMS    += mainwindow.ui

QT += xml opengl

CONFIG += qt opengl #warn_on thread rtti console embed_manifest_exe# no_keywords

  INCLUDEPATH += $$INCLUDE_DIR
  DEPENDPATH  += $$INCLUDE_DIR

    LIBS += -L$${LIB_DIR} -lqglviewerd2
    message($${LIBS})


