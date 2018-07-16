TEMPLATE = app
TARGET = SynAnt

QT = core gui
config += debug
DESTDIR = ../SynAntApp

greaterThan (QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    introductionwindow.cpp \
    hintswindow.cpp \
    maingamewindow.cpp \
    wordmixer.cpp \
    fatalerrors.cpp \
    scoreitem.cpp \
    appmanager.cpp


HEADERS += \
    introductionwindow.h \
    hintswindow.h \
    maingamewindow.h \
    wordmixer.h \
    fatalerrors.h \
    scoreitem.h \
    game.h \
    gamestrings.h \
    appmanager.h
