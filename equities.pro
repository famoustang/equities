#-------------------------------------------------
#
# Project created by QtCreator 2016-06-20T11:07:45
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = equities
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    setupdialog.cpp


HEADERS  += mainwidget.h \
    setupdialog.h \


FORMS    += mainwidget.ui \
    setupdialog.ui

RESOURCES += \
    resource.qrc

RC_ICONS = equties.ico

RC_FILE = version.rc
