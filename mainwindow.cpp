#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QTimer>
#include <QDebug>
#include <QComboBox>

// Assuming you have a QComboBox instance called comboBox


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

    QStringList types = {"Delta", "Theta", "Alpha", "Beta"};

    for (int i = 0; i <= 6; ++i) {
        for (int j = 1; j <= 3; ++j) {
            QString temp = QString("comboBox_e%1_f%2").arg(i).arg(j);
            QComboBox *filter = findChild<QComboBox *>(temp);
            if (filter)
                filter->addItems(types);
        }
    }

    // For easier referencing...
    QCheckBox* electrodes[MAX_ELECTRODES];
    electrodes[0] = ui->checkBox_e0;
    electrodes[1] = ui->checkBox_e1;
    electrodes[2] = ui->checkBox_e2;
    electrodes[3] = ui->checkBox_e3;
    electrodes[4] = ui->checkBox_e4;
    electrodes[5] = ui->checkBox_e5;
    electrodes[6] = ui->checkBox_e6;

    /*====================================================================================================*\
     * MENU
    \*====================================================================================================*/

    // NEW SESSION
    connect(ui->pushButton_playPause, &QPushButton::released, controller, &Controller::playOrPauseSession);
    connect(ui->pushButton_stop, &QPushButton::released, controller, &Controller::stopSession);

    connect(controller, &Controller::updateUI_progressBar, ui->progressBar_session, &QProgressBar::setValue);
    connect(controller, &Controller::updateUI_timerLabel, ui->label_progressTimer, &QLabel::setText);

    // SESSION LOGS
//    connect(ui->pushButton_upload, &QPushButton::released, controller, &Controller::uploadLogs);

    // DATE/TIME
    connect(ui->pushButton_changeDateTime, &QPushButton::released, [=] {
        // Update Controller Date
        QDateTime dateTime = ui->dateTimeEdit->dateTime();
        this->controller->setDateTime(dateTime);
    });
    connect(controller, &Controller::updateUI_dateTimeChanged, this, &MainWindow::updateUI_dateTime);

    /*====================================================================================================*\
     * POWER
    \*====================================================================================================*/
    connect(ui->pushButton_power, &QPushButton::released, [=]() {
        if (this->controller->getPowerState() == ON) {
            this->controller->togglePower(OFF);
        } else {
            this->controller->togglePower(ON);
        }

    });
    connect(controller, &Controller::updateUI_power, this, &MainWindow::updateUI_power);

    /*====================================================================================================*\
     * BATTERY
    \*====================================================================================================*/
    connect(ui->checkBox_pluggedIn, &QCheckBox::stateChanged, [=]() {
        ConnectionState cs = (ui->checkBox_pluggedIn->checkState() == Qt::Checked) ? CONNECTED : DISCONNECTED;
        this->controller->setChargingState(cs);
    });
    connect(controller, &Controller::updateUI_battery, this, &MainWindow::updateUI_battery);

    /*====================================================================================================*\
     * TREATMENT LIGHTS
    \*====================================================================================================*/
    connect(controller, &Controller::updateUI_blueLight, this, &MainWindow::updateUI_blueLight);
    connect(controller, &Controller::updateUI_greenLight, this, &MainWindow::updateUI_greenLight);
    connect(controller, &Controller::updateUI_redLight, this, &MainWindow::updateUI_redLight);

    /*====================================================================================================*\
     * HEADSET/ELECTRODES
    \*====================================================================================================*/
    for(int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        connect(electrodes[e_id], &QCheckBox::stateChanged, [=]() {
            ConnectionState cs = (electrodes[e_id]->isChecked()) ? CONNECTED : DISCONNECTED;
            this->headset->setElectrode(e_id, cs);
//            qInfo("Electrode %d is %s", e_id, qPrintable(connectionStateToStr(this->headset->getElectrode(e_id))));
        });
    }

    connect(headset, &Headset::connectionStateChanged, controller, &Controller::updateConnectionState);


    /*====================================================================================================*\
     * UI SETUP (to match controller state)
    \*====================================================================================================*/
    //Set Power
    this->updateUI_blueLight(OFF);
    this->updateUI_greenLight(OFF);
    this->updateUI_redLight(OFF);
    this->controller->togglePower(ON);

    // Set Menu
    if (this->controller->getPowerState() == ON) {
        this->updateUI_showMenu();
    }

    // Set Date/Time
    ui->label_dateTimeChanged->hide();
    ui->dateTimeEdit->setDateTime(this->controller->getCurrentDateTime());
    qInfo("Date/Time is %s", qPrintable(this->controller->getCurrentDateTime().toString("yyyy-MM-dd HH:mm:ss")));

    // Battery
    ui->progressBar_battery->setValue(this->controller->getBatteryRemaining());
    if (this->controller->getChargingState() == CONNECTED){
        ui->checkBox_pluggedIn->setChecked(true);
    } else {
        ui->checkBox_pluggedIn->setChecked(false);
    }


    // Set Electrode Connections
    for(int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        bool connected = (this->headset->getElectrode(e_id)->getConnectionState() == CONNECTED) ? true : false;
        electrodes[e_id]->setChecked(connected);
    }
}

MainWindow::~MainWindow() {
    delete ui;
    delete controller;
    delete headset;
}

/*====================================================================================================*\
 * MENU
\*====================================================================================================*/
void MainWindow::toggleMenu() {
    if (ui->tabWidget_menu->isEnabled()) {
        this->updateUI_hideMenu();
    } else {
        this->updateUI_showMenu();
    }
}

void MainWindow::updateUI_showMenu(){
    ui->tabWidget_menu->setCurrentIndex(0);
    ui->tabWidget_menu->setEnabled(true);
    ui->tabWidget_menu->show();

    ui->pushButton_playPause->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);

//    ui->label_progressTimer->setDisabled(true);
//    ui->label_progressTimer->hide();
//    ui->progressBar_session->setDisabled(true);
//    ui->progressBar_session->hide();


}

void MainWindow::updateUI_hideMenu(){
    ui->tabWidget_menu->setDisabled(true);
    ui->tabWidget_menu->hide();

    ui->pushButton_playPause->setDisabled(true);
    ui->pushButton_stop->setDisabled(true);
}

void MainWindow::updateUI_dateTime() {
    // Update UI with Date/Time changed message
    ui->label_dateTimeChanged->show();
    qInfo("Date/Time is %s", qPrintable(this->controller->getCurrentDateTime().toString("yyyy-MM-dd HH:mm:ss")));
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        ui->label_dateTimeChanged->hide();
        timer->deleteLater();
    });
    timer->start(3000);
}
/*====================================================================================================*\
 * BATTERY
\*====================================================================================================*/
void MainWindow::updateUI_battery() {
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
void MainWindow::updateUI_power(PowerState ps){
    if (ps == ON) {
        ui->label_powerLight->setStyleSheet("background-color: " + ColourToStr(PINK));
        this->updateUI_showMenu();
        this->updateUI_battery();
    } else {
        ui->label_powerLight->setStyleSheet("background-color: " + ColourToStr(GREY));
        this->updateUI_hideMenu();
//        ui->progressBar_battery->setStyleSheet("background-color: " + ColourToStr(GREY) +
//                                               "selection-color: " + ColourToStr(GREY) +
//                                               "selection-background-color: " + ColourToStr(GREY));
    }
}

/*====================================================================================================*\
 * CONNECTION LIGHTS
\*====================================================================================================*/

void MainWindow::updateUI_blueLight(PowerState ps) {
    if (ps == ON) {
        ui->label_blueLight->setStyleSheet("background-color: " + ColourToStr(BLUE));
    } else {
        ui->label_blueLight->setStyleSheet("background-color: " + ColourToStr(GREY));
    }
}

void MainWindow::updateUI_greenLight(PowerState ps) {
    if (ps == ON) {
        ui->label_greenLight->setStyleSheet("background-color:" + ColourToStr(GREEN));
    } else {
        ui->label_greenLight->setStyleSheet("background-color: " + ColourToStr(GREY));
    }
}

void MainWindow::updateUI_redLight(PowerState ps) {
    if (ps == ON) {
        ui->label_redLight->setStyleSheet("background-color: " + ColourToStr(RED));

        /*
        // Timer to flash Red Lights
        QTimer *timer = new QTimer(this);
        timer->setSingleShot(false);
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
        timer->start(200);
        */

    } else {
        ui->label_redLight->setStyleSheet("background-color: " + ColourToStr(GREY));
    }
}



