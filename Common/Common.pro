#-------------------------------------------------
#
# Project created by QtCreator 2018-08-27T12:31:39
#
#-------------------------------------------------

QT       -= gui

TARGET = Common
TEMPLATE = lib

DEFINES += COMMON_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        common.cpp \
        wordmixer.cpp \
        scoreitem.cpp \
        exceptions.cpp \
        gamefacade.cpp \
        wordpairowner.cpp \
        inputbuilder.cpp \
        datasource.cpp \
        datasourceaccesshelper.cpp \
        gamemanager.cpp \
        gameinitproxy.cpp \
        wordmixerproxy.cpp \
        gamefunctionalityproxy.cpp \
        gameproxy.cpp

HEADERS += \
        common.h \
        common_global.h \
        wordmixer.h \
        scoreitem.h \
        game.h \
        gamestrings.h \
        exceptions.h \
        gamefacade.h \
        wordpairowner.h \
        inputbuilder.h \
        datasource.h \
        datasourceaccesshelper.h \
        gamemanager.h \
        gameinitproxy.h \
        wordmixerproxy.h \
        gamefunctionalityproxy.h \
        gameproxy.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
