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

QMAKE_LFLAGS += "-Wl,-rpath,\'$$top_builddir/Common\'"
QMAKE_LFLAGS += "-Wl,-rpath,\'$$top_builddir/UIResources\'"

LIBS += \
    -L$$top_builddir/Common -lCommon \
    -L$$top_builddir/UIResources -lUIResources
