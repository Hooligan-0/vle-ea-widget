/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#ifndef WIDGETATELIER_H
#define WIDGETATELIER_H

#include <QTabWidget>
#include <QWidget>
#include "data-model/exploitation.h"

class widgetAtelier : public QWidget
{
    Q_OBJECT
public:
    explicit widgetAtelier(QWidget *parent = 0);
    bool     setup(Exploitation *exploitation);
private:
    void addTab(Atelier *atelier);
signals:

public slots:

private:
    Exploitation *mExploitation;
};

#endif // WIDGETATELIER_H
