TEMPLATE = subdirs
SUBDIRS += \
    buttons \
    gifs \
    fonts \
    headerview \
    qquickdialog \
    screenshots \
    styles \
    testbench

qtConfig(systemtrayicon): SUBDIRS += systemtrayicon

qtHaveModule(widgets): SUBDIRS += viewinqwidget
