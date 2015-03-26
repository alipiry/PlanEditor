#-------------------------------------------------
#
# Project created by QtCreator 2015-02-25T06:31:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Origin_GUI
TEMPLATE = app


SOURCES += main.cpp\
    TabWidget.cpp \
    SetParameters/SetParameters.cpp \
    SetParameters/ParametersTreeModel.cpp \
    SetParameters/ParametersTreeItem.cpp \
    Formation/mainwindow.cpp


HEADERS  += TabWidget.h \
    SetParameters/SetParameters.h \
    SetParameters/ParametersTreeModel.h \
    SetParameters/ParametersTreeItem.h \
    Formation/mainwindow.h

FORMS += \
    Formation/mainwindow.ui
