/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#ifndef PARAMETER_H
#define PARAMETER_H

#include <QString>

class Parameter
{
public:
    explicit Parameter(const QString &name, double value = 0);
    const QString & getName(void);
    double getValue(void);
    void   setName (const QString &name);
    void   setValue(double value);
private:
    QString mName;
    double  mValue;
};

#endif // PARAMETER_H
