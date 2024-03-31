#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*====================================================================================================*\
     * INIT MEMBER(S)
    \*====================================================================================================*/
    this->controller = new Controller(100, ON, QDateTime::currentDateTime());
    this->headset = new Headset();

    /*====================================================================================================*\
     * UI SETUP (to match controller state)
    \*====================================================================================================*/
    this->togglePower();
    this->showMenu();
    ui->progressBar_battery->setValue(this->controller->getBatteryRemaining());
    if (this->controller->getChargingState() == CONNECTED){
        ui->checkBox_pluggedIn->setChecked(true);
    } else {
        ui->checkBox_pluggedIn->setChecked(false);
    }
    qInfo("Date/Time is %s", qPrintable(this->controller->getCurrentDateTime().toString("yyyy-MM-dd HH:mm:ss")));


    /*====================================================================================================*\
     * MENU CONNECTIONS
    \*====================================================================================================*/
//    connect(ui->pushButton_menuNewSession, &QPushButton::released, this,  &MainWindow::startNewSession);
    connect(ui->pushButton_menuDateTime, &QPushButton::released, this,  &MainWindow::showMenu_dateTime);
//    connect(ui->pushButton_menuSessionLogs, &QPushButton::released, this,  &MainWindow::viewSessionHistory);
    connect(ui->pushButton_back, &QPushButton::released, this,  &MainWindow::showMenu);
    connect(ui->pushButton_changeDateTime, &QPushButton::released, [=]() {
        QDateTime dateTime = ui->dateTimeEdit->dateTime();
        this->controller->setDateTime(dateTime);
        ui->label_dateTimeChanged->show();
        qInfo("Date/Time is %s", qPrintable(this->controller->getCurrentDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [=]() {
            ui->label_dateTimeChanged->hide();
            timer->deleteLater();
        });
        timer->start(3000);
    });
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*====================================================================================================*\
 * MENU NAVIGATION
\*====================================================================================================*/

void MainWindow::showMenu(){
    this->hideMenu_newSession();
    this->hideMenu_dateTime();
    this->hideMenu_sessionLogs();

    ui->pushButton_menuNewSession->setEnabled(true);
    ui->pushButton_menuNewSession->show();
    ui->pushButton_menuNewSession->setEnabled(true);
    ui->pushButton_menuDateTime->show();
    ui->pushButton_menuNewSession->setEnabled(true);
    ui->pushButton_menuSessionLogs->show();
}

void MainWindow::showMenu_newSession(){
    this->hideMenu();

    ui->pushButton_back->setEnabled(true);
    ui->pushButton_back->show();
}

void MainWindow::showMenu_dateTime(){
    this->hideMenu();
    ui->label_dateTimeChanged->hide();

    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit->setEnabled(true);
    ui->dateTimeEdit->show();
    ui->pushButton_changeDateTime->setEnabled(true);
    ui->pushButton_changeDateTime->show();

    ui->pushButton_back->setEnabled(true);
    ui->pushButton_back->show();

}

void MainWindow::showMenu_sessionLogs(){
    this->hideMenu();

    ui->pushButton_back->setEnabled(true);
    ui->pushButton_back->show();
}
void MainWindow::hideMenu(){
    ui->pushButton_menuNewSession->setDisabled(true);
    ui->pushButton_menuNewSession->hide();
    ui->pushButton_menuNewSession->setDisabled(true);
    ui->pushButton_menuDateTime->hide();
    ui->pushButton_menuNewSession->setDisabled(true);
    ui->pushButton_menuSessionLogs->hide();
}

void MainWindow::hideMenu_newSession() {

    ui->pushButton_back->setDisabled(true);
    ui->pushButton_back->hide();
}

void MainWindow::hideMenu_dateTime(){
    ui->dateTimeEdit->setDisabled(true);
    ui->dateTimeEdit->hide();
    ui->pushButton_changeDateTime->setDisabled(true);
    ui->pushButton_changeDateTime->hide();
    ui->label_dateTimeChanged->hide();

    ui->pushButton_back->setDisabled(true);
    ui->pushButton_back->hide();

}

void MainWindow::hideMenu_sessionLogs(){

    ui->pushButton_back->setDisabled(true);
    ui->pushButton_back->hide();
}

/*====================================================================================================*\
 * MENU CONTROL
\*====================================================================================================*/



/*====================================================================================================*\
 * BATTERY
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
 * POWER
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
 * SESSION LIGHTS
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

