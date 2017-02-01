# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = BrainTumourSegmentation
DESTDIR = ../Win32/Debug
QT += core widgets gui
CONFIG += debug
DEFINES += WIN64 QT_DLL QT_WIDGETS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
win32:RC_FILE = BrainTumourSegmentation.rc
HEADERS += ./braintumoursegmentation.h \
    thresholdtoolwindow.h \
    resultwindow.h \
    imagearithmwindow.h \
    complexsegmentation.h \
    superpixelisationwindow.h
SOURCES += ./main.cpp \
    ./braintumoursegmentation.cpp \
    thresholdtoolwindow.cpp \
    resultwindow.cpp \
    imagearithmwindow.cpp \
    complexsegmentation.cpp \
    superpixelisationwindow.cpp
FORMS += ./braintumoursegmentation.ui \
    thresholdtoolwindow.ui \
    resultwindow.ui \
    imagearithmwindow.ui \
    complexsegmentation.ui \
    superpixelisationwindow.ui
RESOURCES += braintumoursegmentation.qrc
