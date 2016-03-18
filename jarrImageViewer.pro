#-------------------------------------------------
#
# Project created by QtCreator 2014-06-14T16:27:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jarrImageViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    JarrSettings/settings.cpp \
    JarrSettings/jarrattribute.cpp \
    imgadmin.cpp \
    settings_dialog_2.cpp \
    settings_dialog.cpp \
    clickablelabel.cpp

HEADERS  += mainwindow.h \
    JarrSettings/settings.h \
    JarrSettings/jarrattribute.h \
    JarrSettings/attribute.h \
    imgadmin.h \
    settings_dialog.h \
    settings_dialog_2.h \
    program_settings.h \
    clickablelabel.h

FORMS    += mainwindow.ui \
    settings_dialog_2.ui \
    settings_dialog.ui

OTHER_FILES += \
    IV.ico \
    jarrImageViewer.ico \
    exiftool.exe
