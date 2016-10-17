/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
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
}

/**
 * @brief Default destructor of the main window
 *
 */
MainWindow::~MainWindow()
{
    delete ui;
}

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
