/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#ifndef WIDGETROTATION_H
#define WIDGETROTATION_H

#include <QTreeWidget>
#include "data-model/exploitation.h"

class widgetRotation : public QTreeWidget
{
    Q_OBJECT
public:
    explicit widgetRotation(QWidget *parent = 0);
    bool     setup(Exploitation *exploitation);

public slots:

private slots:

private:
    Exploitation *mExploitation;
};

#endif // WIDGETROTATION_H
