##
 # This file is part of VLE, a framework for multi-modeling,
 # simulation and analysis of complex dynamical systems.
 # http://www.vle-project.org
 #
 # Copyright (c) 2016 Agilack

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rotation
TEMPLATE = app

INCLUDEPATH += ../

SOURCES += main.cpp\
        mainwindow.cpp \
        widgetRotation.cpp \
        ../data-model/exploitation.cpp \
        ../data-model/atelier.cpp \
    ../data-model/rotation.cpp \
    ../data-model/parameter.cpp

HEADERS  += mainwindow.h \
            widgetRotation.h \
            ../data-model/exploitation.h \
            ../data-model/atelier.h \
    ../data-model/rotation.h \
    ../data-model/parameter.h

FORMS    += mainwindow.ui
