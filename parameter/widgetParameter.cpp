/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include <QHeaderView>
#include <QLineEdit>
#include <QMenu>
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
    mDefaultValue = 0;

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

    verticalHeader()->setVisible(false);

    // Configure a custom context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(QPoint)),
                     this, SLOT(slotContextMenu(QPoint))            );
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

    for (uint i = 0; i < exploitation->countParameter(); ++i)
    {
        Parameter *param = exploitation->getParameter(i);

        // Add a row to hold this parameter
        insertRow(i);

        // Create an item for the parameter name
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, QVariant::fromValue((void *)param));
        item->setText( param->getName() );
        setItem(i, 0, item);

        // Create an item for the parameter value
        item = new QTableWidgetItem();
        item->setData(Qt::UserRole, QVariant::fromValue((void *)param));
        item->setText( QString::number(param->getValue()));
        setItem(i, 1, item);
    }

    setItemDelegateForColumn( 1, new widgetParameterDelegate(this) );

    QObject::connect(this, SIGNAL(cellChanged(int,int)),
                     this, SLOT  (slotCellChanged(int,int)) );

    return true;
}

/**
 * @brief Slot called when the value of a cell has been modified
 *
 * @param row The row number of the modified cell
 * @param col The col number of the modified cell
 */
void widgetParameter::slotCellChanged(int row, int col)
{
    // Get the modified item (based on row-col)
    QTableWidgetItem *selectedItem = item(row, col);
    if (selectedItem == 0)
        return;

    // Get the associated parameter
    QVariant vParameter = selectedItem->data(Qt::UserRole);
    if ( ! vParameter.isValid())
        return;
    Parameter *p = (Parameter *)vParameter.value<void *>();

    // The column "0" contains parameter names
    if (col == 0)
    {
        QString oldName( p->getName() );
        QString newName( selectedItem->text() );
        // Update the parameter name
        p->setName( newName );
        // Send a message to inform the world that a parameter has been renamed
        emit renamed(p, oldName, newName);
    }

    // The column "1" contains parameter value
    if (col == 1)
    {
        double oldValue = p->getValue();
        double newValue = selectedItem->text().toDouble();
        // Update the parameter value
        p->setValue( newValue );
        // Send a message to inform the world that a value has been modified
        emit valueChanged(p, oldValue, newValue);
    }
}

/**
 * @brief Slot called to show context menu (right click)
 *
 * @param pos Mouse click position
 */
void widgetParameter::slotContextMenu(const QPoint &pos)
{
    QTableWidgetItem *clickedItem = itemAt(pos);

    QMenu ctxMenu(this);
    QAction *actionAdd    = ctxMenu.addAction(tr("Add parameter"));
    QAction *actionRemove = ctxMenu.addAction(tr("Remove parameter"));
    if (clickedItem == 0)
        actionRemove->setEnabled(false);

    // Show context-menu
    QAction *selectedAction = ctxMenu.exec(QCursor::pos());

    // Process selected action
    if (selectedAction == actionAdd)
    {
        Parameter *param = mExploitation->addParameter("NewParam");
        param->setValue(mDefaultValue);

        // Send a message to inform the world that a new parameter has been added
        emit added(param);

        // Insert a new row at the bottom of the table
        int rowPos = rowCount();
        insertRow( rowPos );

        // Create an item for the parameter value
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, QVariant::fromValue((void *)param));
        item->setText( QString::number(param->getValue()));
        setItem(rowPos, 1, item);

        // Create an item for the parameter name
        item = new QTableWidgetItem();
        item->setFlags( item->flags() | Qt::ItemIsEditable);
        item->setData(Qt::UserRole, QVariant::fromValue((void *)param));
        item->setText( param->getName() );
        setItem(rowPos, 0, item);

        editItem(item);
    }
    else if (selectedAction == actionRemove)
    {
        // Get the associated parameter
        QVariant vParameter = clickedItem->data(Qt::UserRole);
        if (vParameter.isValid())
        {
            Parameter *p = (Parameter *)vParameter.value<void *>();

            // Send a message to inform the world that a parameter is removed
            emit removed(p->getName(), p->getValue());

            if (mExploitation->removeParameter(p))
                removeRow( clickedItem->row() );
        }
    }
}

// -------------------- Delegate --------------------

widgetParameterDelegate::widgetParameterDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    // Nothing to do
}

QWidget *widgetParameterDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)option;
    (void)index;
    QLineEdit* editor = new QLineEdit(parent);

    QPalette palette;
    palette.setColor(QPalette::Base, QColor(200,255,225));
    palette.setColor(QPalette::Text, Qt::black);
    editor->setPalette(palette);

    return editor;
}

void widgetParameterDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant value = index.model()->data(index, Qt::EditRole);
    QLineEdit *line = static_cast<QLineEdit*>(editor);
    line->setText(value.toString());
}

void widgetParameterDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *line = static_cast<QLineEdit*>(editor);
    QString value = line->text();
    // Check if the new value is a 'double'
    bool valid;
    value.toDouble(&valid);
    // Update the cell value only if the new value is a 'double'
    if (valid)
        model->setData(index, value);
}
