/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#ifndef ATELIER_H
#define ATELIER_H

#include <QList>
#include <QMap>
#include <QString>

class Atelier
{
public:
    Atelier();
    const QString &getName(void);
    void setName(const QString &name);
    void addParameter(const QString &name, double initialValue);
    int  countParameter(void);
    QString getParameterName(int index);
private:
    QString mName;
    QMap<QString, double> mParameters;
    QList<Atelier *>      mEntities;
};

#endif // ATELIER_H
