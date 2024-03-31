#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->controller = new Controller(100, ON, QDate::currentDate(), QTime::currentTime());
    this->headset = new Headset();

    // UI SETUP
    ui->progressBar_battery->setValue(this->controller->getBatteryRemaining());

    // Connections
    connect(ui->pushButton_newSession, &QPushButton::released, controller,  &Controller::startNewSession);
    connect(ui->pushButton_dateTime, &QPushButton::released, controller,  &Controller::setDateTime);
    connect(ui->pushButton_viewHistory, &QPushButton::released, controller,  &Controller::viewSessionHistory);

    connect(ui->checkBox_pluggedIn, &QCheckBox::stateChanged, this,  &MainWindow::updateBattery);

    connect(ui->pushButton_chargeBattery, &QPushButton::released, [=]() {
        controller->chargeBattery(5);
    });

    connect(ui->pushButton_reduceBattery, &QPushButton::released, [=]() {
        controller->reduceBattery(5);
    });

    connect(controller, &Controller::updateBattery, this, &MainWindow::updateBattery);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateBattery(int batteryRemaining) {
    ui->progressBar_battery->setValue(this->controller->getBatteryRemaining());
    int currentBatteryLevel = ui->progressBar_battery->value();
    if (currentBatteryLevel <= 10) {
        ui->progressBar_battery->setStyleSheet("background-color: rgb(222, 221, 218);"
                                               "selection-color: rgb(0, 0, 0);"
                                               "selection-background-color: rgb(237, 51, 59);");
    } else if (currentBatteryLevel <= 20) {
        ui->progressBar_battery->setStyleSheet("background-color: rgb(222, 221, 218);"
                                               "selection-color: rgb(0, 0, 0);"
                                               "selection-background-color: rgb(249, 240, 107);");
    } else {
        ui->progressBar_battery->setStyleSheet("background-color: rgb(222, 221, 218);"
                                               "selection-color: rgb(0, 0, 0);"
                                               "selection-background-color: rgb(38, 162, 105);");
    }
}

void MainWindow::toggleBlueLight(PowerState newState) {
    if (newState == OFF) {
        //Controller.toggleBlueLight(PowerState newState) ;

    } else {
         //Controller.toggleBlueLight(PowerState newState) ;
    }
}

void MainWindow::toggleGreenLight(PowerState newState) {
    if (newState == OFF) {
        //Controller.toggleBlueLight(PowerState newState) ;

    } else {
         //Controller.toggleBlueLight(PowerState newState) ;
    }
}

void MainWindow::toggleRedLight(PowerState newState) {
    if (newState == OFF) {
        //Controller.toggleBlueLight(PowerState newState) ;

    } else {
         //Controller.toggleBlueLight(PowerState newState) ;
    }
}
