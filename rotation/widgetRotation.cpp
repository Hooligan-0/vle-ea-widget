/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include <QVBoxLayout>
#include <QWidget>
#include "widgetRotation.h"

/**
 * @brief Default constructor for Rotation widget
 *
 * @param parent
 */
widgetRotation::widgetRotation(QWidget *parent) : QTreeWidget(parent)
{
    mExploitation = 0;

    setHeaderHidden(true);
}

/**
 * @brief Initialize widget for a specific Exploitation
 *
 * @param exploitation Pointer to the Exploitation
 * @return boolean True if init success
 */
bool widgetRotation::setup(Exploitation *exploitation)
{
    if (exploitation == 0)
        return false;
    mExploitation = exploitation;

    // Create an empty widget
    QWidget *tree = new QWidget(this);
    tree->setObjectName("rotationTree");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tree);
    setLayout(layout);
    show();
    return true;
}

