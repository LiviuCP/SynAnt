TEMPLATE = app
TARGET = SynAnt

QT = core gui
config += debug

greaterThan (QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    introductionwindow.cpp \
    hintswindow.cpp \
    maingamewindow.cpp \
    fatalerrors.cpp \
    appmanager.cpp


HEADERS += \
    introductionwindow.h \
    hintswindow.h \
    maingamewindow.h \
    fatalerrors.h \
    appmanager.h

DEPENDPATH += $$top_builddir/Common
INCLUDEPATH += $$top_srcdir/Common
LIBS += -L$$top_builddir/Common -lCommon
