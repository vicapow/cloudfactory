# Project information
message( Name : CloudFactory)
message( Author : Victor Powell Timothy Luciani)
message( Email : vicapow at gmail dot com tbl8 at pitt dot edu)
message( Qt Version : $$QT_VERSION )

TEMPLATE = app
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
UI_DIR = tmp/ui
OBJECTS_DIR = tmp/obj

WM5_PATH = ~/GeometricTools/WildMagic5/

message( WM5_PATH : $$WM5_PATH )

WMLIB = $(WM5_PATH)/SDK/Library/Release

message( WMLIB : $$WMLIB )

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
INCLUDEPATH += $(WM5_PATH)/SDK/Include .
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
    GLUT \
    $$WMLIB/libWm5AglApplication.a \
    $$WMLIB/libWm5AglGraphics.a \
    $$WMLIB/libWm5Core.a \
    $$WMLIB/libWm5Mathematics.a \
    $$WMLIB/libWm5Physics.a


CONFIG(debug, debug|release)
:message( build type: debug )

# include(src/src.pri)
SOURCES += src/main.cpp \
    src/mygldrawer.cpp
SOURCES += src/marchingcubes.cpp
SOURCES += src/marchingcubes.hpp
DESTDIR = ./build
HEADERS += src/mygldrawer.h
