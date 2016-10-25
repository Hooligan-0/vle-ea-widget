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
    setWindowTitle("VLE EA Unit-test for Rotation");

    loadTest();
    ui->RotationWidget->setup(&mExploitation);

    // Catch signal emited when the duration of a Rotation is modified
    QObject::connect(ui->RotationWidget, SIGNAL(durationChanged(Rotation*,ulong,ulong)),
                     this,               SLOT(slotDurationChanged(Rotation*,ulong,ulong)) );
    // Catch signal emited when a new plan is created
    QObject::connect(ui->RotationWidget, SIGNAL(planAdded(ActivityPlan*)),
                     this,               SLOT(slotPlanAdded(ActivityPlan*)) );
    // Catch signal emited when a plan has been deleted
    QObject::connect(ui->RotationWidget, SIGNAL(planDeleted(Rotation*,QString,ulong)),
                     this,               SLOT(slotPlanDeleted(Rotation*,QString,ulong)) );
    // Catch signal emited when a plan has been renamed
    QObject::connect(ui->RotationWidget, SIGNAL(planRenamed(ActivityPlan*,QString,QString)),
                     this,               SLOT(slotPlanRenamed(ActivityPlan*,QString,QString)) );
    // Catch signal emited when a plan have a new position
    QObject::connect(ui->RotationWidget, SIGNAL(positionChanged(ActivityPlan*,ulong,ulong)),
                     this,               SLOT(slotPositionChanged(ActivityPlan*,ulong,ulong)) );
    // Catch signal emited when a new rotation is created
    QObject::connect(ui->RotationWidget, SIGNAL(rotationAdded(Rotation*)),
                     this,               SLOT(slotRotationAdded(Rotation*)) );
    // Catch signal emited when a rotation has been deleted
    QObject::connect(ui->RotationWidget, SIGNAL(rotationDeleted(QString,ulong)),
                     this,               SLOT(slotRotationDeleted(QString,ulong)) );
    // Catch signal emited when the name of a Rotation is modified
    QObject::connect(ui->RotationWidget, SIGNAL(rotationRenamed(Rotation*,QString,QString)),
                     this,               SLOT(slotRotationRenamed(Rotation*,QString,QString)) );
}

/**
 * @brief Default destructor of the main window
 *
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Load some dummy datas into local Exploitation
 *
 */
void MainWindow::loadTest()
{
    Rotation *r;
    // Create the first test rotation
    r = mExploitation.createRotation("MaïsIntensif", 1);
    r->addPlan(1, "maïs");

    // Create another test rotation
    r = mExploitation.createRotation("BléTournesol", 1);
    r->addPlan(1, "Tournesol");
    r->addPlan(1, "Blé");

    // Create another test rotation for "Culture Bio"
    r = mExploitation.createRotation("CultureBio", 2);
    r->addPlan(1, "Tournesol");
    r->addPlan(2, "solnu");
}

/**
 * @brief Slot called when the duration of a Rotation is modified
 *
 * @param rot Pointer to the modified rotation
 * @param oldDuration Previous duration of the rotation
 * @param newDuration New duration of the rotation
 */
void MainWindow::slotDurationChanged(Rotation *rot, ulong oldDuration, ulong newDuration)
{
    qWarning() << "The duration of the rotation" << rot->getName()
               << "has been modified :" << oldDuration
               << "--->" << newDuration;
}

/**
 * @brief Slot called when a new plan has been added to a rotation
 *
 * @param plan Pointer to the newly created ActivityPlan
 */
void MainWindow::slotPlanAdded(ActivityPlan *plan)
{
    Rotation *rot = plan->parent();

    qWarning() << "A new plan has been added to rotation" << rot->getName()
               << ":" << plan->getName();
}

/**
 * @brief Slot called when a plan has been deleted
 *
 * @param rot  Pointer to the rotation that owned the plan
 * @param name Name of the removed plan
 * @param position Last known position of the deleted plan
 */
void MainWindow::slotPlanDeleted(Rotation *rot, const QString &name, ulong position)
{
    (void)position;
    qWarning() << "A plan has been deleted from rotation" << rot->getName()
               << ":" << name;
}

/**
 * @brief Slot called when an plan has been renamed
 *
 * @param plan    Pointer to the modified plan
 * @param oldName Old name of the plan
 * @param newName New name of the plan
 */
void MainWindow::slotPlanRenamed(ActivityPlan *plan, const QString &oldName, const QString &newName)
{
    Rotation *rot = plan->parent();
    qWarning() << "A Plan of the rotation" << rot->getName()
               << "has been renamed :"
               << oldName << "--->" << newName;
}

/**
 * @brief Slot called when the position of a plan is modified
 *
 * @param plan Pointer to the modified plan
 * @param oldPosition Old position of the plan
 * @param newPosition New position of the plan
 */
void MainWindow::slotPositionChanged(ActivityPlan *plan, ulong oldPosition, ulong newPosition)
{
    qWarning() << "The position of the plan" << plan->getName()
               << "has been modified :" << oldPosition << "--->" << newPosition;
}

/**
 * @brief Slot called when a new rotation is created into Exploitation
 *
 * @param rot Pointer to the newly created rotation
 */
void MainWindow::slotRotationAdded(Rotation *rot)
{
    qWarning() << "A new rotation has been created" << rot->getName();
}

/**
 * @brief Slot called when a rotation has been deleted
 *
 * @param name     Name of the deleted rotation
 * @param duration Last known duration of the deleted rotation
 */
void MainWindow::slotRotationDeleted(const QString &name, ulong duration)
{
    (void)duration;
    qWarning() << "A rotation has been deleted :" << name;
}

/**
 * @brief Slot called when a rotation is renamed
 *
 * @param rot     Pointer to the modified rotation
 * @param oldName Old name of the rotation
 * @param newName New name of the rotation
 */
void MainWindow::slotRotationRenamed(Rotation *rot, const QString &oldName, const QString &newName)
{
    (void)rot;
    qWarning() << "A rotation has been renamed :"
               << oldName << "--->" << newName;
}
