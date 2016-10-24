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
