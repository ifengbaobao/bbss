#-------------------------------------------------
#
# Project created by QtCreator 2019-07-19T13:42:43
#
#-------------------------------------------------

QT       += core gui network

#windows API
LIBS += -lGdi32
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BBSSClient
TEMPLATE = app


SOURCES += main.cpp\
    ../mybitmap.cpp \
    ../mycolorutils.cpp \
    ../myjpegcompress.cpp \
    ../myjpegdecompress.cpp \
    ../myscreen.cpp \
    ../myqbitmapserialize.cpp \
    ../socketrecv.cpp \
    ../socketsend.cpp \
    showwidget.cpp

HEADERS  += \
    ../jconfig.h \
    ../jmorecfg.h \
    ../jpeglib.h \
    ../mybitmap.h \
    ../mycolorutils.h \
    ../myjpegcompress.h \
    ../myjpegdecompress.h \
    ../myscreen.h \
    ../myqbitmapserialize.h \
    ../socketrecv.h \
    ../socketsend.h \
    showwidget.h \
    ../config.h

#jpeg lib
win32: LIBS += -L$$PWD/../ -ljpeg
INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../jpeg.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../libjpeg.a
