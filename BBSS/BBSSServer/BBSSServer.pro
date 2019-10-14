#-------------------------------------------------
#
# Project created by QtCreator 2019-07-19T13:40:02
#
#-------------------------------------------------

QT       += core gui network

#windows API
LIBS += -lGdi32
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BBSSServer
TEMPLATE = app

SOURCES += main.cpp\
    ../mybitmap.cpp \
    ../mycolorutils.cpp \
    ../myjpegcompress.cpp \
    ../myjpegdecompress.cpp \
    ../myqbitmapserialize.cpp \
    ../myscreen.cpp \
    ../socketrecv.cpp \
    ../socketsend.cpp \
    screencontrol.cpp \
    bitmapcontrol.cpp \
    servercontrol.cpp \
    serversetwidget.cpp \
    ../debugutils.cpp

HEADERS  += \
    ../jconfig.h \
    ../jmorecfg.h \
    ../jpeglib.h \
    ../mybitmap.h \
    ../mycolorutils.h \
    ../myjpegcompress.h \
    ../myjpegdecompress.h \
    ../myqbitmapserialize.h \
    ../myscreen.h \
    ../socketrecv.h \
    ../socketsend.h \
    screencontrol.h \
    bitmapcontrol.h \
    servercontrol.h \
    serversetwidget.h \
    ../config.h \
    ../debugutils.h

#jpeg lib
win32: LIBS += -L$$PWD/../ -ljpeg
INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../jpeg.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../libjpeg.a

FORMS += \
    serversetwidget.ui
