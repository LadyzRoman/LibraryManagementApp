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

SUBDIRS += \
    Practice_library_app.pro

FORMS += \
    controller/dialog/bookeditdialog.ui \
    controller/dialog/readereditdialog.ui \
    controller/dialog/statisticdialog.ui \
    controller/page/bookpage.ui \
    controller/page/readerinfopage.ui \
    controller/page/readerpage.ui \
    mainwindow.ui \
    controller/dialog/settingsdialog.ui

HEADERS += \
    controller/dialog/bookeditdialog.h \
    controller/dialog/readereditdialog.h \
    controller/dialog/statisticdialog.h \
    controller/page/bookpage.h \
    controller/page/readerinfopage.h \
    controller/page/readerpage.h \
    exception/databaseexception.h \
    exception/noreaderexception.h \
    exception/statisticexception.h \
    model/abstract/bookmodel.h \
    model/abstract/readermodel.h \
    model/proxy/bookproxymodel.h \
    model/proxy/readerproxymodel.h \
    model/booktablemodel.h \
    model/readertablemodel.h \
    model/sqlrecordbookmodel.h \
    model/sqlrecordreadermodel.h \
    database.h \
    mainwindow.h \
    model/proxy/uniqueproxymodel.h \
    controller/dialog/settingsdialog.h \
    settings.h

SOURCES += \
    controller/dialog/bookeditdialog.cpp \
    controller/dialog/readereditdialog.cpp \
    controller/dialog/statisticdialog.cpp \
    controller/page/bookpage.cpp \
    controller/page/readerinfopage.cpp \
    controller/page/readerpage.cpp \
    model/proxy/bookproxymodel.cpp \
    model/proxy/readerproxymodel.cpp \
    model/booktablemodel.cpp \
    model/readertablemodel.cpp \
    model/sqlrecordbookmodel.cpp \
    model/sqlrecordreadermodel.cpp \
    database.cpp \
    main.cpp \
    mainwindow.cpp \
    model/proxy/uniqueproxymodel.cpp \
    controller/dialog/settingsdialog.cpp \
    settings.cpp





