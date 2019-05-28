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
        CoreFunctionality/wordmixer.cpp \
        CoreFunctionality/wordpairowner.cpp \
        CoreFunctionality/inputbuilder.cpp \
        DataAccess/datasource.cpp \
        DataAccess/datasourceaccesshelper.cpp \
        Management/gamemanager.cpp \
        Management/gamefacade.cpp \
        Proxies/gameinitproxy.cpp \
        Proxies/gamefunctionalityproxy.cpp \
        Proxies/gameproxy.cpp \
        Proxies/datasourceproxy.cpp \
        Proxies/wordmixerproxy.cpp \
        Utilities/scoreitem.cpp \
        Utilities/exceptions.cpp

HEADERS += \
        common.h \
        common_global.h \
        CoreFunctionality/wordmixer.h \
        CoreFunctionality/wordpairowner.h \
        CoreFunctionality/inputbuilder.h \
        DataAccess/datasource.h \
        DataAccess/datasourceaccesshelper.h \
        Management/gamemanager.h \
        Management/gamefacade.h \
        Proxies/gameinitproxy.h \
        Proxies/gamefunctionalityproxy.h \
        Proxies/gameproxy.h \
        Proxies/datasourceproxy.h \
        Proxies/wordmixerproxy.h \
        Utilities/scoreitem.h \
        Utilities/exceptions.h \
        Utilities/game.h \
        Utilities/gamestrings.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
