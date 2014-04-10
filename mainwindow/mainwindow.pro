#-------------------------------------------------
#
# Project created by QtCreator 2014-03-25T23:19:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = mainwindow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphicsview/mapgraphicsview.cpp \
    datamanager.cpp \
    staticdata.cpp \
    dynamicdata.cpp \
    graphics/gbusstop.cpp \
    popup/busstoppopup.cpp \
    communicator.cpp \
    graphics/groute.cpp \
    graphics/gbus.cpp

HEADERS  += mainwindow.h \
    graphicsview/mapgraphicsview.h \
    datamanager.h \
    staticdata.h \
    dynamicdata.h \
    graphics/gbusstop.h \
    popup/busstoppopup.h \
    communicator.h \
    graphics/groute.h \
    datastruct.h \
    graphics/gbus.h

FORMS    += mainwindow.ui \
    popup/busstoppopup.ui

RESOURCES += \
    icons.qrc
