#-------------------------------------------------
#
# Project created by QtCreator 2018-06-28T14:33:57
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Practice_library_app
TEMPLATE = app

QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    bookmodel.cpp \
    readermodel.cpp \
    readerstablemodel.cpp \
    bookstablemodel.cpp \
    database.cpp \
    readereditdialog.cpp \
    bookeditdialog.cpp \
    namemodel.cpp \
    statisticdialog.cpp \
    bookproxymodel.cpp \
    readerproxymodel.cpp

HEADERS += \
        mainwindow.h \
    bookmodel.h \
    readermodel.h \
    readerstablemodel.h \
    bookstablemodel.h \
    database.h \
    readereditdialog.h \
    bookeditdialog.h \
    noreaderexception.h \
    namemodel.h \
    statisticdialog.h \
    statisticexception.h \
    bookproxymodel.h \
    readerproxymodel.h

FORMS += \
        mainwindow.ui \
    readereditdialog.ui \
    bookeditdialog.ui \
    statisticdialog.ui


