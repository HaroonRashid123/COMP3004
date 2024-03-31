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

    // UI SETUP (to match controller state)
    ui->progressBar_battery->setValue(this->controller->getBatteryRemaining());
    this->togglePower();
    if (this->controller->getChargingState() == CONNECTED){
        ui->checkBox_pluggedIn->setChecked(true);
    } else {
        ui->checkBox_pluggedIn->setChecked(false);
    }

    // Connections
    connect(ui->pushButton_newSession, &QPushButton::released, controller,  &Controller::startNewSession);
    connect(ui->pushButton_dateTime, &QPushButton::released, controller,  &Controller::setDateTime);
    connect(ui->pushButton_viewHistory, &QPushButton::released, controller,  &Controller::viewSessionHistory);

    // Battery Functionality
    connect(ui->checkBox_pluggedIn, &QCheckBox::stateChanged, [=]() {
        ConnectionState newCS = DISCONNECTED;
        if (ui->checkBox_pluggedIn->checkState() == Qt::Checked) {
            newCS = CONNECTED;
        }
        this->controller->setChargingState(newCS);
    });
    connect(ui->pushButton_chargeBattery, &QPushButton::released, [=]() {
        this->controller->chargeBattery(5);
    });
    connect(ui->pushButton_reduceBattery, &QPushButton::released, [=]() {
        this->controller->reduceBattery(5);
    });

    // Power Functionality.
    connect(ui->pushButton_power, &QPushButton::released, [=]() {
        if (this->controller->getPowerState() == ON) {
            this->controller->setPowerState(OFF);
        } else {
            this->controller->setPowerState(ON);
        }
    });
    connect(controller, &Controller::togglePower, this, &MainWindow::togglePower);
    connect(controller, &Controller::updateBattery, this, &MainWindow::updateBattery);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Battery Control
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

// Power Control
void MainWindow::togglePower(){
    if (this->controller->getPowerState() == ON) {
        ui->label_powerLight->setStyleSheet("background-color: rgb(220, 138, 221);");
    } else {
        ui->label_powerLight->setStyleSheet("background-color: rgb(94, 92, 100);");
    }
}

// Light Control
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
