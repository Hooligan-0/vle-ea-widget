/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#ifndef WIDGETROTATION_H
#define WIDGETROTATION_H

#include <QPoint>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "data-model/exploitation.h"

class widgetRotation : public QTreeWidget
{
    Q_OBJECT
public:
    explicit widgetRotation(QWidget *parent = 0);
    bool     setup(Exploitation *exploitation);

signals:
    void durationChanged(Rotation *rot, ulong oldDuration, ulong newDuration);
    void planAdded      (ActivityPlan *plan);
    void planDeleted    (Rotation *rot, const QString &name, ulong position);
    void planRenamed    (ActivityPlan *plan, const QString &oldName, const QString &newName);
    void positionChanged(ActivityPlan *plan, ulong oldPosition, ulong newPosition);
    void rotationAdded  (Rotation *rot);
    void rotationDeleted(const QString &name, ulong duration);
    void rotationRenamed(Rotation *rot, const QString &oldName, const QString &newName);

private slots:
    void slotItemChanged(QTreeWidgetItem *item, int column);
    void slotItemEdit(QTreeWidgetItem *item, int column);
    void slotMenu(const QPoint &pos);

private:
    Exploitation *mExploitation;
};

#include <QStyledItemDelegate>

class widgetRotationDelegate : public QStyledItemDelegate
{
Q_OBJECT

public:
    widgetRotationDelegate(QObject* parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // WIDGETROTATION_H
