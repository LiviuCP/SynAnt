TEMPLATE = subdirs

SUBDIRS += \
    Application \
    SystemFunctionality \
    Tests


Application.depends = SystemFunctionality
Tests.depends = SystemFunctionality
