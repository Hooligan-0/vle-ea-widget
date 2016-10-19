/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#ifndef ROTATION_H
#define ROTATION_H

#include <QList>
#include <QString>
#include <QtGlobal>

class ActivityPlan;

class Rotation
{
public:
    explicit Rotation(const QString &name, ulong duration = 0);
    ~Rotation();
    ActivityPlan *addPlan(ulong position, const QString &name);
    ActivityPlan *addPlan(ulong position);
    uint  countPlans(void);
    ulong getDuration(void);
    const QString &getName(void);
    ActivityPlan *getPlan(int index);
    bool removePlan(ActivityPlan *plan);
    bool removePlan(int index);
    void setDuration(ulong duration);
    void setName(const QString &name);
private:
    QString mName;
    ulong   mDuration;
    QList<ActivityPlan *> mPlans;
};

class ActivityPlan
{
public:
    explicit  ActivityPlan(Rotation *parent);
    QString   getName(void);
    ulong     getPosition(void);
    Rotation *parent(void);
    void      setName(const QString &name);
    void      setPosition(ulong position);
private:
    Rotation *mParent;
    ulong     mPosition;
    QString   mName;
};

#endif // ROTATION_H
