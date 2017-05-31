#-------------------------------------------------
#
# Project created by QtCreator 2017-05-08T22:49:36
#
#-------------------------------------------------

QT       += core gui webenginewidgets xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ParserUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    eventbase.cpp \
    eventgoal.cpp \
    team.cpp \
    player.cpp \
    championship.cpp \
    game.cpp

HEADERS  += mainwindow.h \
    eventbase.h \
    eventgoal.h \
    team.h \
    player.h \
    championship.h \
    game.h

FORMS    += mainwindow.ui
