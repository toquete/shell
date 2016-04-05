TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    mysh.c

include(deployment.pri)
qtcAddDeployment()

