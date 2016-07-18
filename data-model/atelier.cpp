/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include "atelier.h"

/**
 * @brief Default constructor for Atelier object
 *
 */
Atelier::Atelier()
{
    mName.clear();
    mEntities.clear();
}

/**
 * @brief Get the Atelier name
 * @return QString
 */
const QString &Atelier::getName(void)
{
    return mName;
}

/**
 * @brief Set the name of this Atelier
 * @param name String of the name to set
 */
void Atelier::setName(const QString &name)
{
    mName = name;
}

/**
 * @brief Create a new parameter
 * @param name String of the parameter name
 * @param initialValue Default value for this parameter
 */
void Atelier::addParameter(const QString &name, double initialValue)
{
    mParameters.insert(name, initialValue);
}

/**
 * @brief Get the number of parameters into this Atelier
 * @return integer Current number of parameters
 */
int Atelier::countParameter(void)
{
    return mParameters.count();
}

/**
 * @brief Get the name of one parameter
 * @param index
 * @return QString Parameter name
 */
QString Atelier::getParameterName(int index)
{
    QList<QString> k = mParameters.keys();

    return k.at(index);
}
