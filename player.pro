#-------------------------------------------------
#
# Project created by QtCreator 2014-04-04T14:56:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = player
TEMPLATE = app

LIBS+=-lvlc

SOURCES += main.cpp\
        player.cpp \
    FenCodeGenere.cpp

HEADERS  += player.h \
    FenCodeGenere.h

FORMS    += player.ui \
    qxsrexample.ui

QT += core gui xml
