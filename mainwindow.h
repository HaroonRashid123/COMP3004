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

    // Members
    Controller *controller;
    Headset *headset;
//    QTimer *timer;
//    bool isTimerPaused;
    bool redLightOn = false;

public slots:
    // Menu
    void toggleMenu();
    void showMenu();
    void hideMenu();

//    void startNewSession();
//    void setDateTime();
//    void viewSessionHistory();

    void updateBattery();
    void togglePower();

    void showConnection(ConnectionState cs);
    void toggleBlueLight(PowerState ps);
    void toggleGreenLight(PowerState ps);
    void toggleRedLight(PowerState ps);
};
#endif // MAINWINDOW_H
