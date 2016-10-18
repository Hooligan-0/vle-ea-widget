/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include <QLineEdit>
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

    setItemDelegate(new widgetRotationDelegate(this));

    QObject::connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
                     this, SLOT  (slotItemEdit(QTreeWidgetItem*,int))    );
    QObject::connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)      ),
                     this, SLOT  (slotItemChanged(QTreeWidgetItem*,int)) );
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
        newItem->setFlags( newItem->flags() |  Qt::ItemIsEditable);
        newItem->setText(0, rot->getName());
        newItem->setText(1, QString("%1 an(s)").arg(rot->getDuration()));
        newItem->setData(0, Qt::UserRole, qVariantFromValue((void*)rot));
        newItem->setData(1, Qt::UserRole, qVariantFromValue((void*)rot));

        // Insert the activity plans of this Rotation
        for (uint j = 0; j < rot->countPlans(); ++j)
        {
            ActivityPlan *plan = rot->getPlan(j);
            QTreeWidgetItem *planItem = new QTreeWidgetItem();
            planItem->setText(0, plan->getName());
            planItem->setText(1, QString("année %1").arg(plan->getPosition()));
            planItem->setFlags( planItem->flags() |  Qt::ItemIsEditable);
            planItem->setData(0, Qt::UserRole + 1, qVariantFromValue((void *)plan));
            planItem->setData(1, Qt::UserRole + 1, qVariantFromValue((void *)plan));

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

/**
 * @brief Slot called when an item has been modified
 *
 * @param item Pointer to the modified item
 * @param column Column where value has been modified
 */
void widgetRotation::slotItemChanged(QTreeWidgetItem *item, int column)
{
    QVariant vRotation = item->data(0, Qt::UserRole);
    QVariant vPlan     = item->data(0, Qt::UserRole + 1);
    if ( ( ! vRotation.isValid()) && ( ! vPlan.isValid()) )
        return;

    if (vRotation.isValid())
    {
        Rotation *rot = (Rotation *)vRotation.value<void *>();
        // If the modifed column is the first, update Rotation name
        if (column == 0)
            rot->setName( item->text(0) );
        // Is the modified column is the second, update Rotation duration
        else if (column == 1)
        {
            bool valid;
            int duration = item->text(1).toInt(&valid);
            if (valid)
                rot->setDuration(duration);
        }
    }
    else if (vPlan.isValid())
    {
        ActivityPlan *plan = (ActivityPlan *)vPlan.value<void *>();
        // If the modifed column is the first, update Plan name
        if (column == 0)
            plan->setName( item->text(0) );
        // Is the modified column is the second, update Rotation duration
        else if (column == 1)
        {
            bool valid;
            int position = item->text(1).toInt(&valid);
            if (valid)
                plan->setPosition(position);
        }
    }
}

/**
 * @brief Slot called when an item is double-clicked, edit it
 *
 * @param item Pointer to the item to edit
 * @param column Column to edit
 */
void widgetRotation::slotItemEdit(QTreeWidgetItem *item, int column)
{
    QVariant vRotation = item->data(0, Qt::UserRole);
    QVariant vPlan     = item->data(0, Qt::UserRole + 1);
    if ( ( ! vRotation.isValid()) && ( ! vPlan.isValid()) )
        return;

    editItem(item, column);
}

// -------------------- Delegate --------------------

widgetRotationDelegate::widgetRotationDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    // Nothing to do
}

QWidget *widgetRotationDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)option;
    (void)index;
    QLineEdit* editor = new QLineEdit(parent);

    QVariant vRotation = index.model()->data(index, Qt::UserRole);
    QVariant vPlan     = index.model()->data(index, Qt::UserRole + 1);
    if (vRotation.isValid())
        editor->setProperty("mode", QVariant((int)1) );
    else if (vPlan.isValid())
        editor->setProperty("mode", QVariant((int)2) );
    else
        editor->setProperty("mode", QVariant((int)0) );

    QPalette palette;
    palette.setColor(QPalette::Base, QColor(200,255,225));
    palette.setColor(QPalette::Text, Qt::black);
    editor->setPalette(palette);

    return editor;
}

void widgetRotationDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant vMode = editor->property("mode");
    if ( ! vMode.isValid())
        return;
    int mode = vMode.toInt();

    QLineEdit *line = static_cast<QLineEdit*>(editor);

    if (mode == 1)
    {
        QVariant vRotation = index.model()->data(index, Qt::UserRole);
        if ( ! vRotation.isValid())
            return; // ToDo : notify this very bad error ?
        Rotation *rot = (Rotation *)vRotation.value<void *>();

        if (index.column() == 0)
            line->setText( rot->getName() );
        else if (index.column() == 1)
            line->setText( QString::number(rot->getDuration()) );
    }
    else if (mode == 2)
    {
        QVariant vPlan = index.model()->data(index, Qt::UserRole + 1);
        if ( ! vPlan.isValid())
            return; // ToDo : notify this very bad error ?
        ActivityPlan *plan = (ActivityPlan *)vPlan.value<void *>();

        if (index.column() == 0)
            line->setText( plan->getName() );
        else if (index.column() == 1)
            line->setText( QString::number(plan->getPosition()) );
    }
}

void widgetRotationDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QVariant vMode = editor->property("mode");
    if ( ! vMode.isValid())
        return;
    int mode = vMode.toInt();

    QLineEdit *line = static_cast<QLineEdit*>(editor);

    QString value = line->text();
    // Mode 1 : update a Rotation (name or duration)
    if (mode == 1)
    {
        if (index.column() == 0)
            model->setData(index, value);
        else if(index.column() == 1)
        {
            bool valid;
            value.toInt(&valid);
            if (valid)
                model->setData(index, QString("%1 an(s)").arg(value));
        }
    }
    // Mode 2 : update an Ancivity Plan (name or Position)
    else if (mode == 2)
    {
        if (index.column() == 0)
            model->setData(index, value);
        else if(index.column() == 1)
        {
            bool valid;
            value.toInt(&valid);
            if (valid)
                model->setData(index, QString("année %1").arg(value));
        }
    }
}
