#-------------------------------------------------
#
# Project created by QtCreator 2014-04-04T14:56:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = player
TEMPLATE = app

<<<<<<< HEAD
LIBS+=-lvlc

=======
>>>>>>> 677473f2809a324704009825acea0efc253a9eb9
SOURCES += main.cpp\
        player.cpp \
    FenCodeGenere.cpp

HEADERS  += player.h \
    FenCodeGenere.h

FORMS    += player.ui \
    qxsrexample.ui

QT += core gui xml
<<<<<<< HEAD
=======

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/lib/release/ -lvlc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/lib/debug/ -lvlc
else:unix: LIBS += -L$$PWD/../../../../../../../../usr/lib/ -lvlc

INCLUDEPATH += $$PWD/../../../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../../../usr/include
<<<<<<< HEAD

linux-* {
         target.path = /opt/
         INSTALLS += target
}
=======
>>>>>>> 677473f2809a324704009825acea0efc253a9eb9
>>>>>>> 2b14ea0d6311ef42992e0772516291f7717fc530
