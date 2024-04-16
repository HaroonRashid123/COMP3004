#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ExternalObj.h"
#include "Neureset.h"

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
    Neureset *neureset;
//    QTimer *timer;
//    bool isTimerPaused;
    bool redLightOn = false;

public slots:
    // Menu
    void toggleMenu();
    void updateUI_showMenu();
    void updateUI_hideMenu();

//    void startNewSession();
//    void setDateTime();
//    void viewSessionHistory();

    void updateUI_battery();
    void updateUI_power(PowerState ps);

    void updateUI_dateTime();
    void graphData();

//    void showConnection(ConnectionState cs);
    void updateUI_blueLight(PowerState ps);
    void updateUI_greenLight(PowerState ps);
    void updateUI_redLight(PowerState ps);
};
#endif // MAINWINDOW_H
