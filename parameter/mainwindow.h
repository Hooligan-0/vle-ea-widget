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
    void slotAdded        (Parameter *param);
    void slotRemoved      (const QString &name, double value);
    void slotRenamed      (Parameter *param, const QString &oldName, const QString &newName);
    void slotValueChanged (Parameter *param, double oldValue, double newValue);

protected:
    void loadTestData(void);

private:
    Ui::MainWindow *ui;
    Exploitation mExploitation;
};

#endif // MAINWINDOW_H
