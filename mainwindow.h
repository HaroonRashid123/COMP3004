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

public slots:
    // Menu
    void showMenu();
    void showMenu_newSession();
    void showMenu_dateTime();
    void showMenu_sessionLogs();
    void hideMenu();
    void hideMenu_newSession();
    void hideMenu_dateTime();
    void hideMenu_sessionLogs();

//    void startNewSession();
//    void setDateTime();
//    void viewSessionHistory();

    void updateBattery();
    void togglePower();
    void toggleBlueLight();
    void toggleGreenLight();
    void toggleRedLight();
};
#endif // MAINWINDOW_H
