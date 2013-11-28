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

QT *= xml opengl

CONFIG += qt opengl #warn_on thread rtti console embed_manifest_exe# no_keywords

# --------------------------------------------------------------------------------------

# The remaining of this configuration tries to automatically detect the library paths.
# In your applications, you can probably simply use (see doc/compilation.html for details) :

#INCLUDEPATH *= C:/Users/debunne/Documents/libQGLViewer-2.4.0
#LIBS *= -LC:/Users/debunne/Documents/libQGLViewer-2.4.0/QGLViewer -lQGLViewer2

# Change these paths according to your configuration.

# --------------------------------------------------------------------------------------

QT_VERSION=$$[QT_VERSION]

### Unix configuration ###
unix {
  CONFIG -= debug debug_and_release
  CONFIG *= release

  macx|darwin-g++ {
    # On Mac, the lib path can be specified in the executable using install_name_tool
    contains( LIB_NAME, ".*QGLViewer.framework.*" ) {
      # If framework was not found in a standard directory
      !contains( LIB_DIR, ".*/Library/Frameworks/*$" ) {
        QMAKE_LFLAGS += -F$${LIB_DIR}
        !plugin:QMAKE_POST_LINK=install_name_tool -change QGLViewer.framework/Versions/2/QGLViewer $${LIB_DIR_ABSOLUTE_PATH}/QGLViewer.framework/Versions/2/QGLViewer $${TARGET}.app/Contents/MacOS/$${TARGET}
      }
      LIBS += -F$${LIB_DIR} -framework QGLViewer
    } else {
        !plugin:QMAKE_POST_LINK=install_name_tool -change libQGLViewer.2.dylib $${LIB_DIR_ABSOLUTE_PATH}/libQGLViewer.2.dylib $${TARGET}.app/Contents/MacOS/$${TARGET}
        LIBS *= -L$${LIB_DIR} -lQGLViewer
    }
  } else {
    isEmpty(QMAKE_LFLAGS_RPATH) {
      !plugin:QMAKE_LFLAGS += -Wl,-rpath,$${LIB_DIR_ABSOLUTE_PATH}
    } else {
      !plugin:QMAKE_RPATHDIR *= $${LIB_DIR_ABSOLUTE_PATH}
    }
    LIBS *= -L$${LIB_DIR} -lQGLViewer

        # Qt 4.8 removed the GLU dependency
    QMAKE_LIBS_OPENGL *= -lGLU
  }

  # Paths were correctly detected
  INCLUDEPATH *= $${INCLUDE_DIR}
  DEPENDPATH  *= $${INCLUDE_DIR}

  !isEmpty( QGLVIEWER_STATIC ) {
    LIBS *= $${LIB_DIR}/$${LIB_NAME}
  }

  macx|darwin-g++ {
    contains( QT_VERSION, "^3.*" ) {
      # Qt3 only
      LIBS *= -lobjc
      CONFIG -= thread
    }
  }

  # Remove debugging options in release mode makes files much smaller
  release:QMAKE_CFLAGS_RELEASE -= -g
  release:QMAKE_CXXFLAGS_RELEASE -= -g

  # Intermediate files are created in an hidden folder
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

win32 {
  #CONFIG *= debug_and_release

  # Seems to be needed for Visual Studio with Intel compiler
  #DEFINES *= WIN32

  !isEmpty( QGLVIEWER_STATIC ) {
    DEFINES *= QGLVIEWER_STATIC
    message(Qglviewer static build)
  }

  # Paths were correctly detected
  INCLUDEPATH *= $${INCLUDE_DIR}
  DEPENDPATH  *= $${INCLUDE_DIR}

  # LIB_NAME
  LIB_NAME = QGLViewer
  build_pass:CONFIG(debug, debug|release) {
    LIB_NAME = $$join(LIB_NAME,,,d)
  }
  LIB_NAME = $$join(LIB_NAME,,,2) #TODO 2

  win32-g++: LIB_FILE_NAME = lib$${LIB_NAME}.a
  !win32-g++: LIB_FILE_NAME = $${LIB_NAME}.lib


  win32-g++ {
    # The actual directory where the library/framework was found
    # LIB_DIR_ABSOLUTE_PATH = $$system(cd $${LIB_DIR} && cd)

    isEmpty( QGLVIEWER_STATIC ) {
      LIBS *= -L$${LIB_DIR} -l$${LIB_NAME}
    } else {
      LIBS *= $${LIB_DIR}/$${LIB_FILE_NAME}
    }
  }

  !win32-g++ {
    # Use the Qt DLL version. Only needed with Qt3
    contains( QT_VERSION, "^3.*" ) {
      DEFINES *= QT_DLL QT_THREAD_SUPPORT
    }

    LIBS *= -L$${LIB_DIR} -l$${LIB_NAME}
  }
}
