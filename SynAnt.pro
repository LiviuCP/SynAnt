TEMPLATE = subdirs

SUBDIRS += \
    Application \
    Tests \
    Common

Application.depends = Common
Tests.depends = Common
