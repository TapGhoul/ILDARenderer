#-------------------------------------------------
#
# Project created by QtCreator 2014-08-25T22:13:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ILDARenderer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    laserscene.cpp \
    laserfunctions.cpp \
    ildaserializer.cpp \
    laserfunctionsilda.cpp \
    ildalaserscene.cpp \
    modeldata.cpp

HEADERS  += mainwindow.h \
    laserscene.h \
    laserfunctions.h \
    ildaserializer.h \
    laserfunctionsilda.h \
    ildalaserscene.h \
    modeldata.h

FORMS    += mainwindow.ui

CONFIG   += C++11

OTHER_FILES += \
    README.md \
    LICENSE \
    Notes.txt
