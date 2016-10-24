/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include "parameter.h"

/**
 * @brief Default constructor for Parameter object
 *
 */
Parameter::Parameter(const QString &name, double value)
{
    mName  = name;
    mValue = value;
}

/**
 * @brief Get the Parameter name
 *
 * @return QString
 */
const QString &Parameter::getName(void)
{
    return mName;
}

/**
 * @brief Get the Parameter value
 *
 * @return double
 */
double Parameter::getValue(void)
{
    return mValue;
}

/**
 * @brief Rename this parameter
 *
 * @param name New name for this parameter
 */
void Parameter::setName(const QString &name)
{
    mName = name;
}

/**
 * @brief Set a new value for this parameter
 *
 * @param value New value to set
 */
void Parameter::setValue(double value)
{
    mValue = value;
}
