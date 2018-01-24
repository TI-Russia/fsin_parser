#-------------------------------------------------
#
# Project created by QtCreator 2015-02-05T15:49:54
#
#-------------------------------------------------

QT       += core gui axcontainer widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = FSIN
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    excelhandler.cpp \
    excelcell.cpp \
    person.cpp \
    realty.cpp \
    transport.cpp

HEADERS  += mainwindow.h \
    excelhandler.h \
    excelcell.h \
    person.h \
    realty.h \
    transport.h

FORMS    += mainwindow.ui
