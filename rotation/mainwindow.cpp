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
}

/**
 * @brief Default destructor of the main window
 *
 */
MainWindow::~MainWindow()
{
    delete ui;
}
