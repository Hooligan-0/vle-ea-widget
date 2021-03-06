/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include "atelier.h"
#include "exploitation.h"

/**
 * @brief Default constructor for Atelier object
 *
 * @param parent Pointer to the parent Atelier (the new object is an entity)
 */
Atelier::Atelier(Atelier *parent)
{
    mName.clear();
    mEntities.clear();
    mExploitation = 0;
    mParent   = parent;
    mRotation = 0;
}

/**
 * @brief Constructor for Atelier object
 *
 * @param parent Pointer to an exploitation that owns this Atelier
 */
Atelier::Atelier(Exploitation * exploitation)
{
    mName.clear();
    mEntities.clear();
    mExploitation = exploitation;
    mParent   = 0;
    mRotation = 0;
}

/**
 * @brief Default destructor for an Atelier object
 *
 */
Atelier::~Atelier()
{
    while( ! mEntities.isEmpty())
    {
        // Get the first list item
        Atelier *entity = mEntities.first();
        // Remove this item from list
        mEntities.removeFirst();
        // Then, delete it
        delete entity;
    }

    // Delete all parameters
    while( ! mParameters.isEmpty())
    {
        // Get the first list item
        AtelierParameter *parameter = mParameters.first();
        // Remove this item from list
        mParameters.removeFirst();
        // Then, delete it
        delete parameter;
    }
}

Exploitation *Atelier::getExploitation(void)
{
    if (mExploitation)
        return mExploitation;
    else if (mParent)
        return mParent->getExploitation();
    else
        return 0;
}

/**
 * @brief Get the Atelier name
 *
 * @return QString
 */
const QString &Atelier::getName(void)
{
    return mName;
}

/**
 * @brief Get the current Rotation used by the Atelier
 *
 * @return Pointer to the Rotation
 */
Rotation *Atelier::getRotation(void)
{
    return mRotation;
}

/**
 * @brief Set the name of this Atelier
 *
 * @param name String of the name to set
 */
void Atelier::setName(const QString &name)
{
    mName = name;
}

/**
 * @brief Create a new entity into Atelier
 *
 * @return Pointer to the newly created Atelier
 */
Atelier *Atelier::addEntity(void)
{
    Atelier *newEntity = new Atelier(this);
    for (int i = 0; i < mParameters.count(); ++i)
        newEntity->addParameter( mParameters.at(i) );
    mEntities.push_back(newEntity);
    return newEntity;
}

/**
 * @brief Count the number of entities
 *
 * @return Number of entities into Atelier
 */
int Atelier::countEntity(void)
{
    return mEntities.count();
}

/**
 * @brief Get one entity, identified by his index
 *
 * @param index Position of the requested entity
 * @return Pointer to the requested entity (or NULL)
 */
Atelier *Atelier::getEntity(int index)
{
    return mEntities.at(index);
}

/**
 * @brief Remove one entity from current Atelier
 *
 * @param index Index of the entity to remove
 */
void Atelier::removeEntity(int index)
{
    if (index > (mEntities.count() - 1))
        return;

    Atelier *oldEntity = mEntities.at(index);
    mEntities.removeAt(index);
    delete oldEntity;
}

/**
 * @brief Create a new parameter
 *
 * @param name String of the parameter name
 * @param initialValue Default value for this parameter
 */
void Atelier::addParameter(const QString &name, double initialValue)
{
    AtelierParameter *newParam = new AtelierParameter();
    newParam->setName (name);
    newParam->setValue(initialValue);

    mParameters.push_back(newParam);

    if ( ! mEntities.isEmpty())
    {
        int index = (mParameters.count() - 1);
        for (int i = 0; i < mEntities.count(); i++)
        {
            Atelier *entity = mEntities.at(i);
            entity->addParameter(newParam);
            entity->setParameterValue(index, initialValue);
        }
    }
}

/**
 * @brief Create a new parameter from another
 *
 * @param parameter Pointer to the source parameter
 */
void Atelier::addParameter(AtelierParameter *parameter)
{
    AtelierParameter *newParam = new AtelierParameter();
    newParam->setName ( parameter->getName()  );
    newParam->setValue( parameter->getValue() );

    mParameters.push_back(newParam);
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
 * @brief Delete one of the parameters
 *
 * @param index Parameter to remove
 */
void Atelier::delParameter(int index)
{
    if (index > (mParameters.count() - 1))
        return;

    // Remove the selected parameter into all entities
    for (int i = 0; i < mEntities.count(); i++)
    {
        Atelier *entity = mEntities.at(i);
        entity->delParameter(index);
    }

    AtelierParameter *oldParameter = mParameters.at(index);

    // Remove the selected parameter into the local parameter list
    mParameters.removeAt(index);

    delete oldParameter;
}

/**
 * @brief Get the name of one parameter
 *
 * @param index
 * @return QString Parameter name
 */
QString Atelier::getParameterName(int index)
{
    if (index > (mParameters.count() - 1))
        return QString();

    return mParameters.at(index)->getName();
}

/**
 * @brief Get the value of one parameter
 *
 * @param index
 * @return double Current value of the parameter
 */
double Atelier::getParameterValue(int index)
{
    if (index > (mParameters.count() - 1))
        return 0;

    return mParameters.at(index)->getValue();
}

/**
 * @brief Check if a parameter is mandatory (or not)
 *
 * @param index
 * @return bool
 */
bool Atelier::isParameterMandatory(int index)
{
    if (index > (mParameters.count() - 1))
        return false;

    return mParameters.at(index)->isMandatory();
}

/**
 * @brief Mark a parameter as mandatory
 *
 * @param index
 */
void Atelier::setParameterMandatory(int index)
{
    if (index > (mParameters.count() - 1))
        return;

    mParameters.at(index)->setMandatory();
}

/**
 * @brief Set the name of a parameter (rename it)
 *
 * @param index
 * @param name New name to set
 */
void Atelier::setParameterName(int index, QString &name)
{
    if (index > (mParameters.count() - 1))
        return;

    mParameters.at(index)->setName(name);

    if ( ! mEntities.isEmpty())
    {
        for (int i = 0; i < mEntities.count(); i++)
            mEntities.at(i)->setParameterName(index, name);
    }
}

/**
 * @brief Set the value of a parameter
 *
 * @param index
 * @param value New value for the specified parameter
 */
void Atelier::setParameterValue(int index, double value)
{
    if (index > (mParameters.count() - 1))
        return;

    AtelierParameter *parameter = mParameters.at(index);
    parameter->setValue(value);
}

/**
 * @brief Set the rotation used by this Atelier
 *
 * @param rotation Pointer to the Rotation to use
 */
void Atelier::setRotation(Rotation *rotation)
{
    mRotation = rotation;
}

// -------------------- Parameters --------------------

AtelierParameter::AtelierParameter(AtelierParameter *model)
{
    mMandatory = false;
    mName.clear();
    mValue = 0;

    // Init from model
    if (model)
    {
        mName  = model->getName();
        mValue = model->getValue();
        if (model->isMandatory())
            mMandatory = true;
    }
}

QString AtelierParameter::getName(void)
{
    return mName;
}

double AtelierParameter::getValue(void)
{
    return mValue;
}

bool AtelierParameter::isMandatory(void)
{
    return mMandatory;
}

void AtelierParameter::setMandatory(void)
{
    mMandatory = true;
}

void AtelierParameter::setName(const QString &name)
{
    mName = name;
}

void AtelierParameter::setValue(double value)
{
    mValue = value;
}
