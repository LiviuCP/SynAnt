#-------------------------------------------------
#
# Project created by QtCreator 2018-08-27T12:31:39
#
#-------------------------------------------------

QT       -= gui
QT       += sql

TARGET = SystemFunctionality
TEMPLATE = lib

DEFINES += SYSTEMFUNCTIONALITY_LIBRARY

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
        CoreFunctionality/wordmixer.cpp \
        CoreFunctionality/wordpairowner.cpp \
        CoreFunctionality/inputbuilder.cpp \
        DataAccess/datasource.cpp \
        DataAccess/datasourceloader.cpp \
        DataAccess/dataentryvalidator.cpp \
        DataAccess/dataentrycache.cpp \
        DataAccess/dataentrystatistics.cpp \
        DataAccess/datasourceaccesshelper.cpp \
        ManagementInterfaces/gameinit.cpp \
        ManagementInterfaces/game.cpp \
        ManagementInterfaces/gamefunctionality.cpp \
        ManagementInterfaces/dataentry.cpp \
        ManagementInterfaces/data.cpp \
        Management/gamemanager.cpp \
        Management/gamefacade.cpp \
        Management/dataentryfacade.cpp \
        ManagementProxies/gameinitproxy.cpp \
        ManagementProxies/gamefunctionalityproxy.cpp \
        ManagementProxies/gameproxy.cpp \
        ManagementProxies/dataproxy.cpp \
        ManagementProxies/dataentryproxy.cpp \
        Utilities/statisticsitem.cpp \
        Utilities/chronometer.cpp \
        Utilities/exceptions.cpp \
        systemfunctionality.cpp

HEADERS += \
        CoreFunctionality/wordmixer.h \
        CoreFunctionality/wordpairowner.h \
        CoreFunctionality/inputbuilder.h \
        DataAccess/datasource.h \
        DataAccess/datasourceloader.h \
        DataAccess/dataentryvalidator.h \
        DataAccess/dataentrycache.h \
        DataAccess/dataentrystatistics.h \
        DataAccess/datasourceaccesshelper.h \
        ManagementInterfaces/gameinit.h \
        ManagementInterfaces/game.h \
        ManagementInterfaces/gamefunctionality.h \
        ManagementInterfaces/dataentry.h \
        ManagementInterfaces/data.h \
        Management/gamemanager.h \
        Management/gamefacade.h \
        Management/dataentryfacade.h \
        ManagementProxies/gameinitproxy.h \
        ManagementProxies/gamefunctionalityproxy.h \
        ManagementProxies/gameproxy.h \
        ManagementProxies/dataproxy.h \
        ManagementProxies/dataentryproxy.h \
        Utilities/statisticsitem.h \
        Utilities/chronometer.h \
        Utilities/exceptions.h \
        Utilities/gameutils.h \
        Utilities/dataentryutils.h \
        systemfunctionality.h \
        systemfunctionality_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
