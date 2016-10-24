/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#ifndef WIDGETPARAMETER_H
#define WIDGETPARAMETER_H

#include <QTableWidget>
#include "data-model/exploitation.h"

class widgetParameter : public QTableWidget
{
    Q_OBJECT
public:
    explicit widgetParameter(QWidget *parent = 0);
    bool     setup(Exploitation *exploitation);

public slots:

private slots:

private:
    Exploitation *mExploitation;
};

#endif // WIDGETPARAMETER_H
