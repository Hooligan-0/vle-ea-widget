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
    setWindowTitle("VLE EA Unit-test for Parameters");

    loadTestData();

    ui->ParameterWidget->setup( &mExploitation );

    // Catch signal emited when a new parameter is added
    QObject::connect(ui->ParameterWidget, SIGNAL(added(Parameter*)),
                     this,                SLOT(slotAdded(Parameter*)) );
    // Catch signal emited when a parameter is removed
    QObject::connect(ui->ParameterWidget, SIGNAL(removed(QString,double)),
                     this,                SLOT(slotRemoved(QString,double)) );
    // Catch signal emited when a parameter is renamed
    QObject::connect(ui->ParameterWidget, SIGNAL(renamed(Parameter*,QString,QString)),
                     this,                SLOT(slotRenamed(Parameter*,QString,QString)) );
    // Catch signal emited when the value of a parameter is modified
    QObject::connect(ui->ParameterWidget, SIGNAL(valueChanged(Parameter*,double,double)),
                     this,                SLOT(slotValueChanged(Parameter*,double,double)) );
}

/**
 * @brief Default destructor of the main window
 *
 */
MainWindow::~MainWindow()
{
    qWarning() << "--=={ Dump Global Parameter List }==--";
    for (uint i = 0; i < mExploitation.countParameter(); ++i)
    {
        Parameter *p = mExploitation.getParameter(i);
        qWarning() << " -" << p->getName() << "=" << p->getValue();
    }
    delete ui;
}

/**
 * @brief Load some dummy datas into local Exploitation
 *
 */
void MainWindow::loadTestData(void)
{
    mExploitation.setParameter("Vitesse", 3.14);
}

/**
 * @brief Slot called a new Parameter is created
 *
 * @param param Pointer to the newly created parameter
 */
void MainWindow::slotAdded(Parameter *param)
{
    qWarning() << "A new global parameter has been created " << param->getName();
}

/**
 * @brief Slot called a Parameter is removed
 *
 * @param name  Name of the removed parameter
 * @param value Last known value of the removed parameter
 */
void MainWindow::slotRemoved(const QString &name, double value)
{
    (void)value;
    qWarning() << "A parameter has been removed " << name;
}

/**
 * @brief Slot called a Parameter is renamed
 *
 * @param param Pointer to the renamed parameter
 * @param oldName Previous name of the parameter
 * @param newName New name set to the parameter
 */
void MainWindow::slotRenamed(Parameter *param, const QString &oldName, const QString &newName)
{
    (void)param;
    qWarning() << "A parameter has been renamed " << oldName << "--->" << newName;
}

/**
 * @brief Slot called a value is modified
 *
 * @param param Pointer to the modified parameter
 * @param oldValue Previous value for the parameter
 * @param newValue New value for the parameter
 */
void MainWindow::slotValueChanged(Parameter *param, double oldValue, double newValue)
{
    qWarning() << "The value has been modified " << param->getName() << " : " << oldValue << "--->" << newValue;
}
