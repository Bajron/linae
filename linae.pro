#-------------------------------------------------
#
# Project created by QtCreator 2016-11-28T21:10:53
#
#-------------------------------------------------

QT       += core gui
CONFIG   += C++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = linae
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    CommandLineInput.cpp \
    Highlighter.cpp \
    SimpleStyleMarker.cpp \
    FilteredContent.cpp

HEADERS  += MainWindow.h \
    CommandLineInput.h \
    Highlighter.h \
    SimpleStyleMarker.h \
    FilteredContent.h
