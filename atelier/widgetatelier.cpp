/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include <QTableWidget>
#include <QVBoxLayout>
#include "widgetatelier.h"

/**
 * @brief Default constructor for Atelier widget
 * @param parent
 */
widgetAtelier::widgetAtelier(QWidget *parent) : QWidget(parent)
{
    mExploitation = 0;
}

/**
 * @brief Initialize widget for a specific Exploitation
 * @param exploitation Pointer to the Exploitation
 * @return boolean True if init success
 */
bool widgetAtelier::setup(Exploitation *exploitation)
{
    if (exploitation == 0)
        return false;
    mExploitation = exploitation;

    // Create the main tab widget to hold all Ateliers
    QTabWidget *tabs = new QTabWidget(this);

    // Insert one tab for each Atelier
    for (int i = 0; i < mExploitation->atelierCount(); ++i)
    {
        Atelier *a = mExploitation->atelierGet(i);
        QWidget *atelierPage = new QWidget;
        tabs->addTab(atelierPage, a->getName());

        //
        QTableWidget *atelierTable = new QTableWidget(atelierPage);
        atelierTable->setColumnCount( a->countParameter() );
        for (int j = 0; j < a->countParameter(); ++j)
        {
            QTableWidgetItem *item = new QTableWidgetItem(); //atelierTable->horizontalHeaderItem(j);
            item->setText( a->getParameterName(j) );
            atelierTable->setHorizontalHeaderItem(j, item);
        }

        QVBoxLayout *atelierLayout = new QVBoxLayout;
        atelierLayout->addWidget(atelierTable);
        atelierPage->setLayout(atelierLayout);
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tabs);
    setLayout(layout);
    show();
    return true;
}
