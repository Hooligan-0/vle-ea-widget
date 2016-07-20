/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#ifndef WIDGETATELIER_H
#define WIDGETATELIER_H

#include <QModelIndex>
#include <QPoint>
#include <QTableWidget>
#include <QTabWidget>
#include <QWidget>
#include "data-model/exploitation.h"

class widgetAtelier : public QWidget
{
    Q_OBJECT
public:
    explicit widgetAtelier(QWidget *parent = 0);
    bool     setup(Exploitation *exploitation);
private:
    void addTab(Atelier *atelier);

signals:

public slots:

private slots:
    void slotCellChanged  (int row, int column);
    void slotHeaderEdit   (int index);
    void slotHeaderEditEnd(void);
    void slotHeaderMenu   (const QPoint &pos);

private:
    Exploitation *mExploitation;
};

#include <QStyledItemDelegate>

class widgetAtelierDelegate : public QStyledItemDelegate
{
Q_OBJECT

public:
    widgetAtelierDelegate(QObject* parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};


#endif // WIDGETATELIER_H
