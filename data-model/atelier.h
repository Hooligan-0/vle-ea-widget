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
#include <QString>
#include "rotation.h"

class AtelierParameter;

class Atelier
{
public:
    Atelier();
    ~Atelier();
    const QString &getName(void);
    void setName(const QString &name);
    // Entities
    Atelier *addEntity   (void);
    int      countEntity (void);
    Atelier *getEntity   (int index);
    void     removeEntity(int index);
    // Parameters
    void addParameter(const QString &name, double initialValue);
    void addParameter(AtelierParameter *parameter);
    int  countParameter(void);
    void delParameter(int index);
    QString getParameterName(int index);
    double  getParameterValue(int index);
    Rotation *getRotation(void);
    bool    isParameterMandatory(int index);
    void    setParameterValue(int index, double value);
    void    setParameterMandatory(int index);
    void    setParameterName(int index, QString &name);
    void    setRotation(Rotation *rotation);
private:
    QString mName;
    Rotation *mRotation;
    QList<AtelierParameter *> mParameters;
    QList<Atelier *>          mEntities;
};

class AtelierParameter
{
public:
    explicit AtelierParameter(AtelierParameter *model = 0);
    QString getName (void);
    double  getValue(void);
    bool    isMandatory(void);
    void    setMandatory(void);
    void    setName (const QString &name);
    void    setValue(double value);
private:
    QString mName;
    bool    mMandatory;
    double  mValue;
};

#endif // ATELIER_H
