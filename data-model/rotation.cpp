/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include "atelier.h"
#include "rotation.h"

/**
 * @brief Default constructor for Rotation object
 *
 */
Rotation::Rotation(const QString &name, ulong duration)
{
    mDuration = duration;
    mName     = name;
}

/**
 * @brief Default destructor for an Rotation object
 *
 */
Rotation::~Rotation()
{
    // Nothing to do
}

/**
 * @brief Add a new activity plan into the Rotation
 *
 * @param position Position of the new plan into the Rotation
 * @param name Name of the new plan
 * @return Pointer to the newly created ActivityPlan
 */
ActivityPlan *Rotation::addPlan(ulong position, const QString &name)
{
    // Create a new ActivityPlan
    ActivityPlan *newPlan = new ActivityPlan(this);
    newPlan->setName(name);
    newPlan->setPosition(position);
    // Insert it to the current Rotation
    mPlans.push_back(newPlan);
    // ... and return it
    return newPlan;
}

/**
 * @brief Add a new activity plan into the Rotation, with a default name
 *
 * @param position Position of the new plan into the Rotation
 * @return Pointer to the newly created ActivityPlan
 */
ActivityPlan *Rotation::addPlan(ulong position)
{
    return addPlan(position, "NewPlan");
}

/**
 * @brief Get the number of activity plans into the Rotation
 *
 * @return Number of available activity plans
 */
uint Rotation::countPlans(void)
{
    return mPlans.count();
}

/**
 * @brief Get the Rotation duration
 *
 * @return ulong
 */
ulong Rotation::getDuration(void)
{
    return mDuration;
}

/**
 * @brief Get the Rotation name
 *
 * @return QString
 */
const QString &Rotation::getName(void)
{
    return mName;
}

/**
 * @brief Get the plan at 'index' position
 *
 * @param index Index of the plan
 * @return Pointer to the specified plan
 */
ActivityPlan *Rotation::getPlan(int index)
{
    if (index > (mPlans.count() - 1))
        return 0;

    return mPlans.at(index);
}

/**
 * @brief Remove one activity plan from Rotation
 *
 * @param plan Pointer to the plan to remove
 */
bool Rotation::removePlan(ActivityPlan *plan)
{
    bool result = false;

    for (int i = 0; i < mPlans.size(); ++i)
    {
        if (mPlans.at(i) == plan)
        {
            // Remove item at current position from the list
            mPlans.removeAt(i);
            // Delete it
            delete plan;

            result = true;
            break;
        }
    }
    return result;
}

/**
 * @brief Remove one activity plan from Rotation
 *
 * @param index Index of the plan to remove
 */
bool Rotation::removePlan(int index)
{
    if (index > (mPlans.count() - 1))
        return false;

    // Take the specified activity plan from Rotation
    ActivityPlan *p = mPlans.takeAt(index);
    // Delete it
    delete p;

    return true;
}

/**
 * @brief Set the duration of this Rotation
 *
 * @param duration Number of years of the Rotation
 */
void Rotation::setDuration(ulong duration)
{
    mDuration = duration;
}

/**
 * @brief Set the name of this Rotation
 *
 * @param name String of the name to set
 */
void Rotation::setName(const QString &name)
{
    mName = name;
}

// -------------------- Activity Plans --------------------

ActivityPlan::ActivityPlan(Rotation *parent)
{
    mParent = parent;
}

QString ActivityPlan::getName(void)
{
    return mName;
}

ulong ActivityPlan::getPosition(void)
{
    return mPosition;
}

Rotation *ActivityPlan::parent(void)
{
    return mParent;
}

void ActivityPlan::setName(const QString &name)
{
    mName = name;
}

void ActivityPlan::setPosition(ulong position)
{
    mPosition = position;
}
