TEMPLATE = app
TARGET = SynAnt

QT = core gui
config += debug

greaterThan (QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    appmanager.cpp \

HEADERS += \
    appmanager.h \

DEPENDPATH += \
    $$top_builddir/Common \
    $$top_builddir/UIResources
INCLUDEPATH += \
    $$top_srcdir/Common \
    $$top_srcdir/UIResources
LIBS += \
    -L$$top_builddir/Common -lCommon \
    -L$$top_builddir/UIResources -lUIResources
