TEMPLATE = subdirs

SUBDIRS += \
    Application \
    Tests \
    Common \
    UIResources

Application.depends = Common UIResources
Tests.depends = Common
UIResources.depends = Common
