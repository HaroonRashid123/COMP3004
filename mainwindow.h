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
    void toggleBlueLight(PowerState newState);
    void toggleGreenLight(PowerState newState);
    void toggleRedLight(PowerState newState);

    void updateBattery(int batteryRemaining);

};
#endif // MAINWINDOW_H
