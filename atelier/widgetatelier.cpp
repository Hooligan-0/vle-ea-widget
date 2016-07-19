/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHeaderView>
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
    tabs->setObjectName("rootTabs");

    // Insert one tab for each Atelier
    for (int i = 0; i < mExploitation->atelierCount(); ++i)
    {
        Atelier *a = mExploitation->atelierGet(i);
        addTab(a);
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tabs);
    setLayout(layout);
    show();
    return true;
}

/**
 * @brief Create and insert a new tab for an Atelier
 * @param atelier Pointer to the Atelier
 */
void widgetAtelier::addTab(Atelier *atelier)
{
    QTabWidget *tabs = this->findChild<QTabWidget *>("rootTabs");
    if (tabs == 0)
        return;

    // Create a widget for the new tab content
    QWidget *page = new QWidget;

    // Create a table to show Entities and Parameters
    QTableWidget *entityTable = new QTableWidget(page);
    entityTable->verticalHeader()->setVisible(false);
    entityTable->setProperty("atelier_name", QVariant(atelier->getName()));

    // Set table headers according to Atelier
    entityTable->setColumnCount( 1 + atelier->countParameter() );
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(tr("Nom"));
    // Insert parameters names into horizontal header
    entityTable->setHorizontalHeaderItem(0, item);
    for (int i = 0; i < atelier->countParameter(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText( atelier->getParameterName(i) );
        entityTable->setHorizontalHeaderItem(1 + i, item);
    }

    // Insert atelier entities (one per row)
    for (int i = 0; i < atelier->countEntity(); ++i)
    {
        Atelier *entity = atelier->getEntity(i);

        // Insert a new row at the bottom of the table
        int row = entityTable->rowCount();
        entityTable->insertRow(row);
        // Set Entity name into first column
        entityTable->setCellWidget(row, 0, new QLabel(entity->getName()));

        // Set parameters values
        for (int j = 0; j < entity->countParameter(); ++j)
        {
            QString pvalue = QString::number(entity->getParameterValue(j));
            QTableWidgetItem *item = new QTableWidgetItem(pvalue);
            // Save a pointer to the entity
            QVariant vEntity = qVariantFromValue((void *)entity);
            item->setData(Qt::UserRole, vEntity);
            //entityTable->setCellWidget(row, 1 + j, new QLabel(pvalue));
            entityTable->setItem(row, 1 + j, item);
        }
    }

    entityTable->setItemDelegate(new widgetAtelierDelegate(entityTable));
    entityTable->setEditTriggers(QAbstractItemView::DoubleClicked
                                    | QAbstractItemView::SelectedClicked);

    QObject::connect(entityTable, SIGNAL(cellChanged(int,int)),
                     this,        SLOT(slotCellChanged(int,int)));

    QVBoxLayout *atelierLayout = new QVBoxLayout;
    atelierLayout->addWidget(entityTable);
    page->setLayout(atelierLayout);

    tabs->addTab(page, atelier->getName());
}

void widgetAtelier::slotCellChanged(int row, int col)
{
    // Search the table widget that has emit signal
    QTableWidget *table = qobject_cast<QTableWidget*>( sender() );
    if (table == 0)
        return;

    // Get the cell into this table
    QTableWidgetItem *item = table->item(row, col);
    if (item == 0)
        return;

    // Get the new value and convert it to long
    long newValue = item->text().toLong();

    // Get the associated entity
    Atelier *entity = (Atelier *) item->data(Qt::UserRole).value<void *>();
    if (entity == 0)
        return;
    // Update the entity parameter with the new value
    entity->setParameterValue(col-1, newValue);
}

widgetAtelierDelegate::widgetAtelierDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    // Nothing to do
}

QWidget *widgetAtelierDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

void widgetAtelierDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant value = index.model()->data(index, Qt::EditRole);
    QLineEdit *line = static_cast<QLineEdit*>(editor);
    line->setText(value.toString());
}

void widgetAtelierDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *line = static_cast<QLineEdit*>(editor);
    QString value = line->text();
    model->setData(index, value);
}
