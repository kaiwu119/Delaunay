#-------------------------------------------------
#
# Project created by QtCreator 2018-08-04T17:51:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Delaunay
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    convexhull.cpp \
    sort.cpp \
    kdtree.cpp

HEADERS  += widget.h \
    convexhull.h \
    sort.h \
    reforence.h \
    kdtree.h

FORMS    += widget.ui

RESOURCES += \
    img.qrc
