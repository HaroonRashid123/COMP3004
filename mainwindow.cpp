#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QTimer>
#include <QDebug>

bool isFlashOn = false;

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

    // For easier referencing...
    QCheckBox* electrodes[MAX_ELECTRODES];
    electrodes[0] = ui->checkBox_e0;
    electrodes[1] = ui->checkBox_e1;
    electrodes[2] = ui->checkBox_e2;
    electrodes[3] = ui->checkBox_e3;
    electrodes[4] = ui->checkBox_e4;
    electrodes[5] = ui->checkBox_e5;
    electrodes[6] = ui->checkBox_e6;
    electrodes[7] = ui->checkBox_e7;
    electrodes[8] = ui->checkBox_e8;
    electrodes[9] = ui->checkBox_e9;
    electrodes[10] = ui->checkBox_e10;
    electrodes[11] = ui->checkBox_e11;
    electrodes[12] = ui->checkBox_e12;
    electrodes[13] = ui->checkBox_e13;
    electrodes[14] = ui->checkBox_e14;
    electrodes[15] = ui->checkBox_e15;
    electrodes[16] = ui->checkBox_e16;
    electrodes[17] = ui->checkBox_e17;
    electrodes[18] = ui->checkBox_e18;
    electrodes[19] = ui->checkBox_e19;
    electrodes[20] = ui->checkBox_e20;


    /*====================================================================================================*\
     * MENU - DATE/TIME
    \*====================================================================================================*/
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
     * MENU - SESSION LOGS
    \*====================================================================================================*/
    connect(ui->pushButton_upload, &QPushButton::released, [=]() {
        QVector<Session> sessionLogs = this->controller->getSessionLogs();
        // upload Strings to PC
        // Change text browser on display montor
    });

    /*====================================================================================================*\
     * POWER
    \*====================================================================================================*/
    connect(ui->pushButton_power, &QPushButton::released, [=]() {
        this->controller->setPowerState(this->controller->getPowerState() == ON ? OFF : ON);
    });
    connect(controller, &Controller::togglePower, this, &MainWindow::togglePower);

    /*====================================================================================================*\
     * NEW SESSION
    \*====================================================================================================*/
    connect(controller, &Controller::updateProgressBar, ui->progressBar_session, &QProgressBar::setValue);
    connect(controller, &Controller::updateTimerLabel, ui->label_progressTimer, &QLabel::setText);
    connect(ui->pushButton_play, &QPushButton::released, controller, &Controller::playPauseTimer);
    connect(ui->pushButton_stop, &QPushButton::released, controller, &Controller::resetTimer);

    /*====================================================================================================*\
     * BATTERY
    \*====================================================================================================*/
    connect(ui->checkBox_pluggedIn, &QCheckBox::stateChanged, [=]() {
        ConnectionState newCS = (ui->checkBox_pluggedIn->checkState() == Qt::Checked) ? CONNECTED : DISCONNECTED;
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
     * ELECTRODES
    \*====================================================================================================*/
    for(int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        connect(electrodes[e_id], &QCheckBox::stateChanged, [=]() {
            ConnectionState newCS = (electrodes[e_id]->isChecked()) ? CONNECTED : DISCONNECTED;
            this->headset->setElectrode(e_id, newCS);
            qInfo("Electrode %d is %s", e_id, qPrintable(connectionStateToStr(this->headset->getElectrode(e_id))));
        });
    }

    connect(headset, &Headset::connectionStateChanged, this, &MainWindow::showConnection);
//    connect(controller, &Controller::updateTimerLabel, ui->label_redLight, &MainWindow::toggleRedLight);

    /*====================================================================================================*\
     * UI SETUP (to match controller state)
    \*====================================================================================================*/
    //Set Power
    this->togglePower();

    // Set Menu
    this->showMenu();

    // Set Date/Time
    ui->progressBar_battery->setValue(this->controller->getBatteryRemaining());
    if (this->controller->getChargingState() == CONNECTED){
        ui->checkBox_pluggedIn->setChecked(true);
    } else {
        ui->checkBox_pluggedIn->setChecked(false);
    }
    qInfo("Date/Time is %s", qPrintable(this->controller->getCurrentDateTime().toString("yyyy-MM-dd HH:mm:ss")));

    // Set Electrode Connections
    for(int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        bool connected = (this->headset->getElectrode(e_id) == CONNECTED) ? true : false;
        electrodes[e_id]->setChecked(connected);
    }
}

MainWindow::~MainWindow() {
    delete ui;
    delete controller;
    delete headset;
}

/*====================================================================================================*\
 * MENU NAVIGATION
\*====================================================================================================*/
void MainWindow::toggleMenu() {
    if (ui->tabWidget_menu->isEnabled()) {
        this->hideMenu();
    } else {
        this->showMenu();
    }
}

void MainWindow::showMenu(){
    ui->tabWidget_menu->setCurrentIndex(0);
    ui->tabWidget_menu->setEnabled(true);
    ui->tabWidget_menu->show();

//    ui->label_progressTimer->setDisabled(true);
//    ui->label_progressTimer->hide();
//    ui->progressBar_session->setDisabled(true);
//    ui->progressBar_session->hide();

    ui->label_dateTimeChanged->setDisabled(true);
    ui->label_dateTimeChanged->hide();
}

void MainWindow::hideMenu(){
    ui->tabWidget_menu->setDisabled(true);
    ui->tabWidget_menu->hide();
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

    if (this->controller->getPowerState() == ON) {
        if (currentBatteryLevel <= 10) {
            ui->progressBar_battery->setStyleSheet("background-color: " + ColourToStr(GREY) +
                                                   "selection-color: " + ColourToStr(BLACK) +
                                                   "selection-background-color: " + ColourToStr(RED));
        } else if (currentBatteryLevel <= 20) {
            ui->progressBar_battery->setStyleSheet("background-color: " + ColourToStr(GREY) +
                                                   "selection-color: " + ColourToStr(BLACK) +
                                                   "selection-background-color: " + ColourToStr(YELLOW));
        } else {
            ui->progressBar_battery->setStyleSheet("background-color: " + ColourToStr(GREY) +
                                                   "selection-color: " + ColourToStr(BLACK) +
                                                   "selection-background-color: " + ColourToStr(GREEN));
        }
    }
}

/*====================================================================================================*\
 * POWER
\*====================================================================================================*/
void MainWindow::togglePower(){
    if (this->controller->getPowerState() == ON) {
        ui->label_powerLight->setStyleSheet("background-color: " + ColourToStr(PINK));
        this->showMenu();
        this->updateBattery();
    } else {
        ui->label_powerLight->setStyleSheet("background-color: " + ColourToStr(GREY));
        ui->progressBar_battery->setStyleSheet("background-color: " + ColourToStr(GREY) +
                                               "selection-color: " + ColourToStr(GREY) +
                                               "selection-background-color: " + ColourToStr(GREY));
        this->hideMenu();
    }

    this->toggleBlueLight(OFF);
    this->toggleGreenLight(OFF);
    this->toggleRedLight(OFF);
}

/*====================================================================================================*\
 * CONNECTION LIGHTS
\*====================================================================================================*/
void MainWindow::showConnection(ConnectionState cs) {
    if (cs == CONNECTED) {
        toggleRedLight(OFF);
        toggleBlueLight(ON);
    } else {
        toggleRedLight(ON);
        toggleBlueLight(OFF);

        // Timer to flash Red Lights
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [=]() {
            if (isFlashOn) {
                ui->label_redLight->setStyleSheet("background-color: " + ColourToStr(GREY));
                isFlashOn = false;
            } else {
                ui->label_redLight->setStyleSheet("background-color: " + ColourToStr(RED));
                isFlashOn = true;
            }

            if (this->controller->getRedLight() == OFF) {
                ui->label_redLight->setStyleSheet("background-color: " + ColourToStr(GREY));
                timer->deleteLater();
            }
        });
        timer->setSingleShot(false);
        timer->start(200);
    }
}

void MainWindow::toggleBlueLight(PowerState ps) {
    if ((ps == ON) && (this->controller->getPowerState() == ON) && (/*in session*/true) && /* all nodes are connected */true) {
        this->controller->setBlueLight(ON);
        ui->label_blueLight->setStyleSheet("background-color: " + ColourToStr(BLUE));
    } else {
        this->controller->setBlueLight(OFF);
        ui->label_blueLight->setStyleSheet("background-color: " + ColourToStr(GREY));
    }
}

void MainWindow::toggleGreenLight(PowerState ps) {
    if ((ps == ON) && (this->controller->getPowerState() == ON) && (/*in session*/true) && /* all nodes are connected */true) {
        this->controller->setGreenLight(ON);
        ui->label_greenLight->setStyleSheet("background-color:" + ColourToStr(GREEN));
    } else {
        this->controller->setGreenLight(OFF);
        ui->label_greenLight->setStyleSheet("background-color: " + ColourToStr(GREY));
    }
}

void MainWindow::toggleRedLight(PowerState ps) {
    if ((ps == ON) && (this->controller->getPowerState() == ON) && (/*in session*/true) && /* all nodes are connected */true) {
        this->controller->setRedLight(ON);
        ui->label_redLight->setStyleSheet("background-color: " + ColourToStr(RED));
    } else {
        this->controller->setRedLight(OFF);
        ui->label_redLight->setStyleSheet("background-color: " + ColourToStr(GREY));
    }
}

