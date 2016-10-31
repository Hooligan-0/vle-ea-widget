/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief Default constructor for the main window of the test-app
 *
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("VLE EA Unit-test for Atelier");

    // Load some tests datas into Exploitation
    loadTestData();

    // Catch signal emited when an entity is added to an Atelier
    QObject::connect(ui->AtelierWidget, SIGNAL(entityAdded(Atelier*,int)),
                     this,              SLOT  (entityAdded(Atelier*,int)));
    // Catch signal emited when an entity is removed from an Atelier
    QObject::connect(ui->AtelierWidget, SIGNAL(entityDeleted(Atelier*,int)),
                     this,              SLOT  (entityDeleted(Atelier*,int)));
    // Catch signal emited when an entity is renamed
    QObject::connect(ui->AtelierWidget, SIGNAL(entityNameChanged(Atelier*)),
                     this,              SLOT  (entityNameChanged(Atelier*)));
    // Catch signal when one value of an entity is modified
    QObject::connect(ui->AtelierWidget, SIGNAL(entityValueChanged(Atelier*,int,double)),
                     this,              SLOT  (entityValueChanged(Atelier*,int,double)));
    // Catch signal emited when a new parameter is added to an Atelier
    QObject::connect(ui->AtelierWidget, SIGNAL(parameterAdded(Atelier*,int)),
                     this,              SLOT  (parameterAdded(Atelier*,int)));
    // Catch signal emited when a parameter is deleted
    QObject::connect(ui->AtelierWidget, SIGNAL(parameterDeleted(Atelier*,int)),
                     this,              SLOT  (parameterDeleted(Atelier*,int)));
    // Catch signal emited when a parameter is renamed
    QObject::connect(ui->AtelierWidget, SIGNAL(parameterNameChanged(Atelier*,int)),
                     this,              SLOT  (parameterNameChanged(Atelier*,int)));

    ui->AtelierWidget->setup(&mExploitation);
}

/**
 * @brief Default destructor of the main window
 *
 */
MainWindow::~MainWindow()
{
    qWarning() << "--=={ Dump Atelier(s) Datas }==--";
    for (uint i = 0; i < mExploitation.countAtelier(); ++i)
    {
        Atelier *a = mExploitation.getAtelier(i);

        qWarning() << "    ======== Atelier" << a->getName() << "========";
        for (int j = 0; j < a->countEntity(); ++j)
        {
            Atelier *entity = a->getEntity(j);
            QString dbgLine("    ");
            dbgLine += entity->getName() + " : ";
            for (int k = 0; k < entity->countParameter(); ++k)
            {
                dbgLine += QString::number(entity->getParameterValue(k));
                if (k != (entity->countParameter() - 1))
                    dbgLine += " , ";
            }
            qWarning() << dbgLine.toStdString().c_str();
        }
    }

    delete ui;
}

/**
 * @brief Slot called when a new entity is added to an Atelier
 *
 * @param atelier Pointer to the modified Atelier
 * @param index   Index of the new entity into the Atelier
 */
void MainWindow::entityAdded(Atelier *atelier, int index)
{
    Atelier *entity = atelier->getEntity(index);
    qWarning() << "New entity added into Atelier " << atelier->getName() << " at index " << index << " named " << entity->getName();
}

/**
 * @brief Slot called when an entity is deleted
 *
 * @param atelier Pointer to the modified Atelier (owner of the deleted entity)
 * @param index   Index of the deleted entity
 */
void MainWindow::entityDeleted(Atelier *atelier, int index)
{
    qWarning() << "An entity has been deleted from atelier " << atelier->getName() << " at index " << index;
}

/**
 * @brief Slot called when an entity is renamed
 *
 * @param entity Pointer to the modified entity
 */
void MainWindow::entityNameChanged (Atelier *entity)
{
    qWarning() << "Entity renamed " << entity->getName();
}

/**
 * @brief Slot called when a value of an entity has been modified
 *
 * @param entity Pointer to the modified entity
 * @param index  Index of the parameter
 * @param value  New value of the parameter
 */
void MainWindow::entityValueChanged(Atelier *entity, int index, double value)
{
    QString pName(entity->getParameterName(index));
    qWarning() << "Entity " << entity->getName()
               << " new value for parameter " << pName
               << ":" << value;
}

/**
 * @brief Slot called when a new parameter is added to an Atelier
 *
 * @param atelier Pointer to the modified Atelier
 * @param index   Index of the new parameter
 */
void MainWindow::parameterAdded(Atelier *atelier, int index)
{
    QString pName(atelier->getParameterName(index));
    qWarning() << "Atelier " << atelier->getName() << " new parameter added " << pName;
}

/**
 * @brief Slot called when a parameter is deleted
 *
 * @param atelier Pointer to the modified Atelier
 * @param index   Index of the deleted parameter
 */
void MainWindow::parameterDeleted(Atelier *atelier, int index)
{
    qWarning() << "Atelier " << atelier->getName() << " one parameter has been deleted at index " << index;
}

/**
 * @brief Slot called when a parameter is renamed
 *
 * @param atelier Pointer to the modified Atelier
 * @param index   Index of the parameter that has been renamed
 */
void MainWindow::parameterNameChanged(Atelier *atelier, int index)
{
    QString pName(atelier->getParameterName(index));
    qWarning() << "Atelier " << atelier->getName() << " parameter renamed " << pName;
}

/**
 * @brief Load some dummy datas into local Exploitation
 *
 */
void MainWindow::loadTestData(void)
{
    Rotation *rotBio = mExploitation.createRotation("Culture Bio", 3);
    Rotation *rotInt = mExploitation.createRotation("Intensif", 2);

    Atelier *a1 = mExploitation.createAtelier("Grande culture");
    a1->addParameter("Profondeur", 4);
    a1->addParameter("Surface",   42);
    a1->setParameterMandatory(1);

    Atelier * a1e1 = a1->addEntity();
    a1e1->setRotation(rotBio);
    a1e1->setName("Champ de blé #1");
    a1e1->setParameterValue(0,  8);
    a1e1->setParameterValue(1, 16);
    Atelier * a1e2 = a1->addEntity();
    a1e2->setRotation(rotInt);
    a1e2->setName("Tournesol");
    a1e2->setParameterValue(0, 128);
    a1e2->setParameterValue(1, 256);

    Atelier *a2 = mExploitation.createAtelier("Troupeau");
    a2->addParameter("Nombre", 0);
    Atelier * a2e1 = a2->addEntity();
    a2e1->setName("Bovins");
    a2e1->setParameterValue(0,  38);
    Atelier * a2e2 = a2->addEntity();
    a2e2->setName("Moutons");
    a2e2->setParameterValue(0,  130);

    Atelier *a3 = mExploitation.createAtelier("Maraichage");
    a3->addParameter("Surface", 0);
}
