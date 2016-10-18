/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include <QTreeWidgetItem>
#include "widgetRotation.h"

/**
 * @brief Default constructor for Rotation widget
 *
 * @param parent
 */
widgetRotation::widgetRotation(QWidget *parent) : QTreeWidget(parent)
{
    mExploitation = 0;

    // Create two columns by setting her names
    headerItem()->setText(0, "Name");
    headerItem()->setText(1, " ");

    // Insert a global root item for the tree
    QTreeWidgetItem *root = new QTreeWidgetItem(this);
    root->setText(0, "Rotations");
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

    QTreeWidgetItem *root = topLevelItem(0);

    // Insert all Rotation(s) of the Exploitation
    for (uint i = 0; i < mExploitation->countRotation(); ++i)
    {
        Rotation *rot = mExploitation->getRotation(i);
        // Create a tree-item for this Rotation
        QTreeWidgetItem *newItem = new QTreeWidgetItem();
        newItem->setText(0, rot->getName());
        newItem->setText(1, QString("%1 an(s)").arg(rot->getDuration()));

        // Insert the activity plans of this Rotation
        for (uint j = 0; j < rot->countPlans(); ++j)
        {
            ActivityPlan *plan = rot->getPlan(j);
            QTreeWidgetItem *planItem = new QTreeWidgetItem();
            planItem->setText(0, plan->getName());
            planItem->setText(1, QString("année %1").arg(plan->getPosition()));

            // Insert it as sub-tree of current Rotation item
            newItem->addChild(planItem);
        }
        // Insert this Rotation as child of the root item
        root->addChild(newItem);
    }
    root->setExpanded(true);
    resizeColumnToContents(0);

    return true;
}

