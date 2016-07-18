##
 # This file is part of VLE, a framework for multi-modeling,
 # simulation and analysis of complex dynamical systems.
 # http://www.vle-project.org
 #
 # Copyright (c) 2016 Agilack

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = atelier
TEMPLATE = app

INCLUDEPATH += ../../

SOURCES += main.cpp\
        mainwindow.cpp \
    widgetatelier.cpp \
    ../../data-model/exploitation.cpp \
    ../../data-model/atelier.cpp

HEADERS  += mainwindow.h \
    widgetatelier.h \
    ../../data-model/exploitation.h \
    ../../data-model/atelier.h

FORMS    += mainwindow.ui
