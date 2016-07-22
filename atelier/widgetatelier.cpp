/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
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

void widgetAtelier::addEntity(QTableWidget *table, Atelier *entity)
{
    // Insert a new row at the bottom of the table
    int row = table->rowCount();
    table->insertRow(row);

    // Set Entity name into header column
    QTableWidgetItem *hItem = new QTableWidgetItem(entity->getName());
    hItem->setData(Qt::UserRole, qVariantFromValue((void *)entity));
    table->setVerticalHeaderItem(row, hItem);

    // Set parameters values
    for (int i = 0; i < entity->countParameter(); ++i)
    {
        QString pvalue = QString::number(entity->getParameterValue(i));
        QTableWidgetItem *item = new QTableWidgetItem(pvalue);
        // Save a pointer to the entity
        QVariant vEntity = qVariantFromValue((void *)entity);
        item->setData(Qt::UserRole, vEntity);
        table->setItem(row, i, item);
    }
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
    entityTable->verticalHeader()->setVisible(true);
    entityTable->setSelectionMode(QAbstractItemView::SingleSelection);
    entityTable->setProperty("atelier", qVariantFromValue((void *)atelier));

    // Set a minimum height to show header when no parameter is defined
    entityTable->horizontalHeader()->setMinimumHeight(20);
    // Set a minimum width to show header when no entity is defined
    entityTable->verticalHeader()->setMinimumWidth(60);

    // Set table headers according to Atelier
    entityTable->setColumnCount( atelier->countParameter() );

    // Insert parameters names into horizontal header
    for (int i = 0; i < atelier->countParameter(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText( atelier->getParameterName(i) );
        item->setData(Qt::UserRole, qVariantFromValue((void *)atelier));
        if (atelier->isParameterMandatory(i))
            item->setFlags( item->flags() ^ Qt::ItemIsEditable);
        else
            item->setFlags( item->flags() | Qt::ItemIsEditable);
        entityTable->setHorizontalHeaderItem(i, item);
    }
    // Catch signal for horizontal header context menu (parameters)
    QHeaderView *paramHeader = entityTable->horizontalHeader();
    paramHeader->setSectionsClickable(true);
    paramHeader->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(paramHeader, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this,        SLOT (slotHeaderMenu(const QPoint &)));
    QObject::connect(paramHeader, SIGNAL(sectionDoubleClicked(int)),
                     this,        SLOT  (slotHeaderEdit(int)));

    // Catch signal for vertical header context menu (entities names)
    QHeaderView *namesHeader = entityTable->verticalHeader();
    namesHeader->setSectionsClickable(true);
    namesHeader->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(namesHeader, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this,        SLOT (slotNamesMenu(const QPoint &)));
    QObject::connect(namesHeader, SIGNAL(sectionDoubleClicked(int)),
                     this,        SLOT  (slotNamesEdit(int)));

    // Insert atelier entities (one per row)
    for (int i = 0; i < atelier->countEntity(); ++i)
        addEntity(entityTable, atelier->getEntity(i));

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

/**
 * @brief Slot called when the value of a cell is modified
 * @param row Position (row) of the modified cell
 * @param col Position (column) of the modified cell
 */
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
    entity->setParameterValue(col, newValue);

    // Send a message to inform the world that a value has been updated
    emit entityValueChanged(entity, col, newValue);
}

/**
 * @brief Slot called on column header double-clicked to edit it
 *
 * @param index Column index
 */
void widgetAtelier::slotHeaderEdit(int index)
{
    if (index < 0)
        return;

    QHeaderView *headerView = qobject_cast<QHeaderView*>( sender() );
    if (headerView == 0)
        return;

    QTableWidget *entityTable = qobject_cast<QTableWidget*>( headerView->parent() );
    QTableWidgetItem *item = entityTable->horizontalHeaderItem(index);

    // Test if this column header can be modified
    if ( ! (item->flags() & Qt::ItemIsEditable))
        return;

    // Create a line-edit widget
    QLineEdit* editor = new QLineEdit(headerView->viewport());
    // Move this widget to overlay the column header
    int editorX = headerView->sectionViewportPosition(index);
    int editorW = headerView->sectionSize(index);
    editor->resize(editorW, 25);
    editor->move(editorX, 0);
    // Save a copy of the column index
    editor->setProperty("index", QVariant(index));
    editor->setProperty("tableWidget", qVariantFromValue((void *)entityTable));

    // Catch the end-of-edition event
    QObject::connect(editor, SIGNAL(editingFinished()),
                     this,   SLOT(slotHeaderEditEnd()));

    // Copy the header title to edit box
    editor->setText( item->text() );

    editor->setFocus();
    editor->show();
}

/**
 * @brief Slot called at the end of edition of a column haeder
 *
 * When the user want to modify the name of a column, a LineEdit widget is
 * created to overlay the header hitself (see slotHeaderEdit). The "end" of
 * edition can be when the user hit "enter" or when the widget lost focus.
 */
void widgetAtelier::slotHeaderEditEnd(void)
{
    QLineEdit *editor = qobject_cast<QLineEdit*>( sender() );

    QVariant vTable = editor->property("tableWidget");
    QVariant vIndex = editor->property("index");
    if ( (vIndex.isValid() == false) || (vTable.isValid() == false) )
        return;

    QTableWidget *entityTable = (QTableWidget *) vTable.value<void *>();
    QTableWidgetItem *item = entityTable->horizontalHeaderItem(vIndex.toInt());

    // Get a pointer to the associated Atelier
    QVariant vAtelier = item->data(Qt::UserRole);
    if ( ! vAtelier.isValid())
    {
        delete editor;
        return;
    }
    Atelier *atelier = (Atelier *)vAtelier.value<void *>();

    int index = vIndex.toInt();

    QString newName(editor->text());

    // Update the parameter name into Atelier
    atelier->setParameterName(index, newName);
    // Update the parameter name into the table
    item->setText( newName );

    // Send a message to inform the world that a parameter has been renamed
    emit parameterNameChanged(atelier, index);

    delete editor;
}

/**
 * @brief Slot called to show context menu on horitontal table header (right click)
 *
 * @param pos Mouse click position into header
 */
void widgetAtelier::slotHeaderMenu(const QPoint &pos)
{
    QHeaderView *headerView = qobject_cast<QHeaderView*>( sender() );
    if (headerView == 0)
        return;

    int selectedColumn = headerView->logicalIndexAt(pos);

    QTableWidget *entityTable = qobject_cast<QTableWidget*>( headerView->parent() );
    if (entityTable == 0)
        return;

    // Search the associated Atelier
    QVariant vAtelier = entityTable->property("atelier");
    if ( ! vAtelier.isValid())
        return;
    Atelier *atelier = (Atelier *) vAtelier.value<void *>();

    QMenu ctxMenu(this);
    QAction *actionAdd = ctxMenu.addAction(tr("Add parameter"));

    QAction *actionRemove = 0;
    if (selectedColumn >= 0)
    {
        actionRemove = ctxMenu.addAction(tr("Remove parameter"));
        if (atelier->isParameterMandatory(selectedColumn))
            actionRemove->setEnabled(false);
    }

    QAction *selectedAction = ctxMenu.exec(QCursor::pos());

    // If the menu is closed without any action selected
    if (selectedAction == 0)
    {
        // Nothing selected, nothing to do
    }
    // If the "Add" action has been selected
    else if (selectedAction == actionAdd)
    {
        // Create a new parameter into this Atelier
        atelier->addParameter("NewParameter", 0);

        // Insert a new column to the table
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setFlags ( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable );
        item->setText("NewParameter");
        item->setData(Qt::UserRole, qVariantFromValue((void *)atelier));
        int pos = entityTable->columnCount();
        // Increment the number of columns into table
        entityTable->setColumnCount( pos + 1);

        entityTable->setHorizontalHeaderItem(pos, item);

        for (int i = 0; i < entityTable->rowCount(); i++)
        {
            Atelier *entity = atelier->getEntity(i);

            QTableWidgetItem *item = new QTableWidgetItem("0");
            entityTable->setItem(i, pos, item);

            // Save a pointer to the entity into cell
            QVariant vEntity = qVariantFromValue((void *)entity);
            item->setData(Qt::UserRole, vEntity);
        }

        // Send a message to inform the world that a new parameter has been added
        emit parameterAdded(atelier, (atelier->countParameter() - 1) );
    }
    // If the "Remove" action has been selected
    else if (selectedAction == actionRemove)
    {
        // Delete the requested parameter into the Atelier
        atelier->delParameter(selectedColumn);
        // Remove the selected column into the table widget
        entityTable->removeColumn(selectedColumn);

        // Send a message to inform the world that a parameter has been deleted
        emit parameterDeleted(atelier, selectedColumn);
    }
}

/**
 * @brief Slot called on row header double-clicked to edit entity name
 *
 * @param index Row index
 */
void widgetAtelier::slotNamesEdit(int index)
{
    if (index < 0)
        return;

    QHeaderView *headerView = qobject_cast<QHeaderView*>( sender() );
    if (headerView == 0)
        return;

    QTableWidget *entityTable = qobject_cast<QTableWidget*>( headerView->parent() );
    QTableWidgetItem *item = entityTable->verticalHeaderItem(index);

    // Test if this entity name can be modified
    if ( ! (item->flags() & Qt::ItemIsEditable))
        return;

    // Create a line-edit widget
    QLineEdit* editor = new QLineEdit(headerView->viewport());
    // Move this widget to overlay the column header
    int editorY = headerView->sectionViewportPosition(index);
    int editorH = headerView->sectionSize(index);
    int editorW = headerView->width();
    editor->resize(editorW, editorH);
    editor->move(0, editorY);
    // Save a copy of the column index
    editor->setProperty("index", QVariant(index));
    editor->setProperty("tableWidget", qVariantFromValue((void *)entityTable));

    // Catch the end-of-edition event
    QObject::connect(editor, SIGNAL(editingFinished()),
                     this,   SLOT(slotNamesEditEnd()));

    // Copy the header title to edit box
    editor->setText( item->text() );

    editor->setFocus();
    editor->show();
}

/**
 * @brief Slot called at the end of edition of an entity name
 *
 * When the user want to modify the name of an entity, a LineEdit widget is
 * created to overlay the header hitself (see slotNamesEdit). The "end" of
 * edition can be when the user hit "enter" or when the widget lost focus.
 */
void widgetAtelier::slotNamesEditEnd (void)
{
    QLineEdit *editor = qobject_cast<QLineEdit*>( sender() );
    if (editor == 0)
        return;

    QVariant vTable = editor->property("tableWidget");
    QVariant vIndex = editor->property("index");
    if ( (vIndex.isValid() == false) || (vTable.isValid() == false) )
        return;

    // Get the table widget
    QTableWidget *entityTable = (QTableWidget *) vTable.value<void *>();
    // Get the item of the selected entity header
    QTableWidgetItem *item = entityTable->verticalHeaderItem(vIndex.toInt());

    // Get a pointer to the entity hitself
    QVariant vEntity = item->data(Qt::UserRole);
    if ( ! vEntity.isValid())
    {
        delete editor;
        return;
    }
    Atelier *entity = (Atelier *)vEntity.value<void *>();

    // Update the entity name into the Atelier
    entity->setName(editor->text());
    // Update the entity name into the table
    item->setText( editor->text() );

    // Send a message to inform the world that an entity has been renamed
    emit entityNameChanged(entity);

    delete editor;
}

/**
 * @brief Slot called to show context menu on vertical table header (right click)
 *
 * @param pos Mouse click position into header
 */
void widgetAtelier::slotNamesMenu(const QPoint &pos)
{
    QHeaderView *headerView = qobject_cast<QHeaderView*>( sender() );
    if (headerView == 0)
        return;

    QTableWidget *entityTable = qobject_cast<QTableWidget*>( headerView->parent() );
    if (entityTable == 0)
        return;

    // Search the associated Atelier
    QVariant vAtelier = entityTable->property("atelier");
    if ( ! vAtelier.isValid())
        return;
    Atelier *atelier = (Atelier *) vAtelier.value<void *>();

    int selectedRow = headerView->logicalIndexAt(pos);

    QMenu ctxMenu(this);
    QAction *actionAdd = ctxMenu.addAction(tr("Add entity"));
    QAction *actionRemove = 0;
    if (selectedRow >= 0)
        actionRemove = ctxMenu.addAction(tr("Remove entity"));

    QAction *selectedAction = ctxMenu.exec(QCursor::pos());

    // If the menu is closed without any action selected
    if (selectedAction == 0)
    {
        // Nothing selected, nothing to do
    }
    // If the "Add" action has been selected
    else if (selectedAction == actionAdd)
    {
        Atelier *entity = atelier->addEntity();
        entity->setName("NewEntity");

        addEntity(entityTable, entity);

        // Send a message to inform the world that a new entity has been added
        emit entityAdded(atelier, (atelier->countEntity() - 1));
    }
    // If the "Remove" action has been selected
    else if (selectedAction == actionRemove)
    {
        // Remove the selected entity from Atelier
        atelier->removeEntity(selectedRow);
        // Remove the selected entity from table
        entityTable->removeRow(selectedRow);

        // Send a message to inform the world that an entity has been deleted
        emit entityDeleted(atelier, selectedRow);
    }
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
    // Check if the new value is a 'long'
    bool valid;
    value.toLong(&valid);
    // Update the cell value only if the new value is a 'long'
    if (valid)
        model->setData(index, value);
}
