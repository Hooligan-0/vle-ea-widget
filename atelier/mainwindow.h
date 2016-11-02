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
#include "data-model/atelier.h"
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

protected:
    void loadTestData(void);

public slots:
    void entityAdded         (Atelier *entity,  int index);
    void entityDeleted       (Atelier *atelier, int index);
    void entityNameChanged   (Atelier *entity);
    void entityValueChanged  (Atelier *entity,  int index, double value);
    void entityRotationChanged(Atelier *entity);
    void parameterAdded      (Atelier *atelier, int index);
    void parameterDeleted    (Atelier *atelier, int index);
    void parameterNameChanged(Atelier *atelier, int index);

private:
    Ui::MainWindow *ui;
    Exploitation    mExploitation;
};

#endif // MAINWINDOW_H
