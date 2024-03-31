#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ExternalObj.h"
#include "Controller.h"
#include "Headset.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Controller *controller;
    Headset *headset;
signals:

public slots:
    void toggleBlueLight();
    void toggleGreenLight();
    void toggleRedLight();

    void updateBattery();
    void togglePower();
};
#endif // MAINWINDOW_H
