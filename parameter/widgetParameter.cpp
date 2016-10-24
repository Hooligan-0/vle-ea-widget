/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include <QHeaderView>
#include <QTableWidgetItem>
#include "widgetParameter.h"

/**
 * @brief Default constructor for Parameter widget
 *
 * @param parent
 */
widgetParameter::widgetParameter(QWidget *parent) : QTableWidget(parent)
{
    mExploitation = 0;

    setColumnCount(2);

    // Create header for first column (parameter name)
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setFlags( item->flags() ^ Qt::ItemIsEditable);
    item->setText( tr("Name") );
    setHorizontalHeaderItem(0, item);
    // Create header for second column (parameter value)
    item = new QTableWidgetItem();
    item->setFlags( item->flags() ^ Qt::ItemIsEditable);
    item->setText( tr("Value") );
    setHorizontalHeaderItem(1, item);

    horizontalHeader()->setMinimumHeight(20);
    horizontalHeader()->setStretchLastSection(true);
}

/**
 * @brief Initialize widget for a specific Exploitation
 *
 * @param exploitation Pointer to the Exploitation
 * @return boolean True if init success
 */
bool widgetParameter::setup(Exploitation *exploitation)
{
    if (exploitation == 0)
        return false;
    mExploitation = exploitation;

    return true;
}
