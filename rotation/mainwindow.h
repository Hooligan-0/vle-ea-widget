/*
 * This file is part of VLE, a framework for multi-modeling,
 * simulation and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2016 Agilack
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "data-model/exploitation.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotDurationChanged(Rotation *rot, ulong oldDuration, ulong newDuration);
    void slotPlanAdded      (ActivityPlan *plan);
    void slotPlanDeleted    (Rotation *rot, const QString &name, ulong position);
    void slotPlanRenamed    (ActivityPlan *plan, const QString &oldName, const QString &newName);
    void slotPositionChanged(ActivityPlan *plan, ulong oldPosition, ulong newPosition);
    void slotRotationAdded  (Rotation *rot);
    void slotRotationDeleted(const QString &name, ulong duration);
    void slotRotationRenamed(Rotation *rot, const QString &oldName, const QString &newName);

protected:
    void loadTest(void);

private:
    Ui::MainWindow *ui;
    Exploitation mExploitation;
};

#endif // MAINWINDOW_H
