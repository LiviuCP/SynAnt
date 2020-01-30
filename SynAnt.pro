TEMPLATE = subdirs

SUBDIRS += \
    Application \
    SystemFunctionality

Application.depends = SystemFunctionality

CONFIG(debug, debug|release) {
    SUBDIRS += Tests
    Tests.depends = SystemFunctionality
}
