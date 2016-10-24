/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#ifndef WIDGETPARAMETER_H
#define WIDGETPARAMETER_H

#include <QStyledItemDelegate>
#include <QTableWidget>
#include "data-model/exploitation.h"

class widgetParameter : public QTableWidget
{
    Q_OBJECT
public:
    explicit widgetParameter(QWidget *parent = 0);
    bool     setup(Exploitation *exploitation);

public slots:

private slots:
    void slotCellChanged(int row, int col);
    void slotContextMenu(const QPoint &pos);

protected:

private:
    Exploitation *mExploitation;
    double        mDefaultValue;
};

class widgetParameterDelegate : public QStyledItemDelegate
{
Q_OBJECT

public:
    widgetParameterDelegate(QObject* parent = 0);
    QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void     setEditorData(QWidget *editor, const QModelIndex &index) const;
    void     setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // WIDGETPARAMETER_H
