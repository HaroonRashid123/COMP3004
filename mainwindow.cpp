#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*====================================================================================================*\
     * INIT MEMBER(S)
    \*====================================================================================================*/
    this->controller = new Controller(100, ON, QDate::currentDate(), QTime::currentTime());
    this->headset = new Headset();

    /*====================================================================================================*\
     * UI SETUP (to match controller state)
    \*====================================================================================================*/
    ui->progressBar_battery->setValue(this->controller->getBatteryRemaining());
    this->togglePower();
    if (this->controller->getChargingState() == CONNECTED){
        ui->checkBox_pluggedIn->setChecked(true);
    } else {
        ui->checkBox_pluggedIn->setChecked(false);
    }

    /*====================================================================================================*\
     * CONNECTIONS
    \*====================================================================================================*/
    connect(ui->pushButton_newSession, &QPushButton::released, this,  &Controller::startNewSession);
    connect(ui->pushButton_dateTime, &QPushButton::released, this,  &Controller::setDateTime);
    connect(ui->pushButton_viewHistory, &QPushButton::released, this,  &Controller::viewSessionHistory);

    /*====================================================================================================*\
     * BATTERY RELATED CONNECTIONS
    \*====================================================================================================*/
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
    connect(controller, &Controller::updateBattery, this, &MainWindow::updateBattery);

    /*====================================================================================================*\
     * POWER RELATED CONNECTIONS
    \*====================================================================================================*/
    connect(ui->pushButton_power, &QPushButton::released, [=]() {
        if (this->controller->getPowerState() == ON) {
            this->controller->setPowerState(OFF);
        } else {
            this->controller->setPowerState(ON);
        }
    });
    connect(controller, &Controller::togglePower, this, &MainWindow::togglePower);

    /*====================================================================================================*\
     * TEMPLATE
    \*====================================================================================================*/

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*====================================================================================================*\
 * BATTERY CONTROL
\*====================================================================================================*/
void MainWindow::updateBattery() {
    ui->progressBar_battery->setValue(this->controller->getBatteryRemaining());
    int currentBatteryLevel = ui->progressBar_battery->value();
    if (currentBatteryLevel <= 10) {
        ui->progressBar_battery->setStyleSheet("background-color: rgb(94, 92, 100);"
                                               "selection-color: rgb(0, 0, 0);"
                                               "selection-background-color: rgb(237, 51, 59);");
    } else if (currentBatteryLevel <= 20) {
        ui->progressBar_battery->setStyleSheet("background-color: rgb(94, 92, 100);"
                                               "selection-color: rgb(0, 0, 0);"
                                               "selection-background-color: rgb(249, 240, 107);");
    } else {
        ui->progressBar_battery->setStyleSheet("background-color: rgb(94, 92, 100);"
                                               "selection-color: rgb(0, 0, 0);"
                                               "selection-background-color: rgb(38, 162, 105);");
    }
}

/*====================================================================================================*\
 * POWER CONTROL
\*====================================================================================================*/
void MainWindow::togglePower(){
    if (this->controller->getPowerState() == ON) {
        ui->label_powerLight->setStyleSheet("background-color: rgb(220, 138, 221);");
        this->updateBattery();

    } else {
        ui->label_powerLight->setStyleSheet("background-color: rgb(94, 92, 100);");
        ui->progressBar_battery->setStyleSheet("background-color: rgb(94, 92, 100);"
                                               "selection-color: rgb(94, 92, 100);;"
                                               "selection-background-color: rgb(94, 92, 100);");
    }
    this->toggleBlueLight();
    this->toggleGreenLight();
    this->toggleRedLight();
}

/*====================================================================================================*\
 * SESSION LIGHTS CONTROL
\*====================================================================================================*/
void MainWindow::toggleBlueLight() {
    if ((this->controller->getPowerState() == ON) && (/*in session*/true) && /* all nodes are connected */true) {
        ui->label_blueLight->setStyleSheet("background-color: rgb(26, 95, 180);");
    } else {
        ui->label_blueLight->setStyleSheet("background-color: rgb(94, 92, 100);");
    }
}

void MainWindow::toggleGreenLight() {
    if ((this->controller->getPowerState() == ON) && (/*in session*/true) && /* all nodes are connected */true) {
        ui->label_greenLight->setStyleSheet("background-color: rgb(38, 162, 105);");
    } else {
        ui->label_greenLight->setStyleSheet("background-color: rgb(94, 92, 100);");
    }
}

void MainWindow::toggleRedLight() {
    if ((this->controller->getPowerState() == ON) && (/*in session*/true) && /* all nodes are connected */true) {
        ui->label_redLight->setStyleSheet("background-color: rgb(224, 27, 36);");
    } else {
        ui->label_redLight->setStyleSheet("background-color: rgb(94, 92, 100);");
    }
}

