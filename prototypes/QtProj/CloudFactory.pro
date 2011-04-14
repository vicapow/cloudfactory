# Project information
message( Name : JointViewer)
message( Author : Victor Powell)
message( Email : vicapow at gmail dot com)
message( Qt Version : $$QT_VERSION )
TEMPLATE = app
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
UI_DIR = tmp/ui
OBJECTS_DIR = tmp/obj

QT += opengl

# Architecture needs to be i386 for now
# arch i386
QMAKE_CXXFLAGS += -arch \
    i386
QMAKE_CXXFLAGS_RELEASE += -arch \
    i386
QMAKE_LFLAGS_DEBUG += -arch \
    i386
QMAKE_LFLAGS_RELEASE += -arch \
    i386
CONFIG += debug

# CONFIG += i386
message( Architecture: i386 )
DEPENDPATH += .
INCLUDEPATH += .
LIBS += -framework \
    QtCore \
    -framework \
    QtGui \
    -framework \
    QtXml \
    -framework \
    QtSvg \
    -framework \
    QtOpenGL \
    -framework \
    OpenGL \
    -framework \
    GLUT
CONFIG(debug, debug|release)
:message( build type: debug )

# include(src/src.pri)
SOURCES += src/main.cpp \
    src/mygldrawer.cpp
SOURCES += src/marchingcubes.cpp
SOURCES += src/marchingcubes.hpp
DESTDIR = ./build
HEADERS += src/mygldrawer.h
