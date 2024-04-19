#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QTimer>
#include <QDebug>
#include <qcustomplot.h>
#include <QComboBox>
#include <random>

bool isFlashOn = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    /*====================================================================================================*\
     * INIT MEMBER(S)
    \*====================================================================================================*/
    this->neureset = new Neureset(100, ON, QDateTime::currentDateTime());
//    this->headset = new Headset();

    /*
    QStringList types = {"Delta", "Theta", "Alpha", "Beta"};
    for (int i = 0; i <= 6; ++i) {
        for (int j = 1; j <= 3; ++j) {
            QString temp = QString("comboBox_e%1_f%2").arg(i).arg(j);
            QComboBox *filter = findChild<QComboBox *>(temp);
            if (filter)
                filter->addItems(types);
        }
    }
    */

    // For easier referencing, organize the electrode widget into an array...
    for (int i=0; i<MAX_ELECTRODES; ++i) {
            this->electrodes[i] = findChild<QCheckBox*>(QString("checkBox_e%1").arg(i));
    }

    for (int i=0; i<MAX_ELECTRODES; ++i) {
        for (int j = 0; j < 3; ++j) {
            this->electrodeFrequencies[i][j] = findChild<QComboBox*>(QString("comboBox_e%1_f%2").arg(i).arg(j + 1));
            // qInfo("%s", qPrintable(electrode_frequencies[i][j]->objectName()));
        }
    }

    for (int i=0; i<MAX_ELECTRODES; ++i) {
        for (int j = 0; j < 3; ++j) {
            this->electrodeAmplitudes[i][j] = findChild<QSpinBox*>(QString("spinBox_e%1_a%2").arg(i).arg(j + 1));
            // qInfo("%s", qPrintable(electrode_amplitudes[i][j]->objectName()));
        }
    }

    // Timer to flash Red Lights
    this->flashTimer = new QTimer(this);
    connect(this->flashTimer, &QTimer::timeout, [=]() {
        if (isFlashOn) {
            ui->label_redLight->setStyleSheet("background-color: " + ColourToStr(GREY));
            isFlashOn = false;
        } else {
            ui->label_redLight->setStyleSheet("background-color: " + ColourToStr(RED));
            isFlashOn = true;
        }
    });

    /*====================================================================================================*\
     * MENU
    \*====================================================================================================*/

    // NEW SESSION
    connect(ui->pushButton_playPause, &QPushButton::released, neureset, &Neureset::playOrPauseSession);

    connect(ui->pushButton_stop, &QPushButton::released, neureset, &Neureset::stopSession);

    connect(neureset, &Neureset::updateUI_progressBar, ui->progressBar_session, &QProgressBar::setValue);
    connect(neureset, &Neureset::updateUI_timerLabel, ui->label_progressTimer, &QLabel::setText);

    // SESSION LOGS
    connect(ui->pushButton_upload, &QPushButton::released, neureset, &Neureset::uploadLogs);

    // DATE/TIME
    connect(ui->pushButton_changeDateTime, &QPushButton::released, [=] {
        // Update Neureset Date
        QDateTime dateTime = ui->dateTimeEdit->dateTime();
        this->neureset->setDateTime(dateTime);
    });
    connect(neureset, &Neureset::updateUI_dateTimeChanged, this, &MainWindow::updateUI_dateTime);

    /*====================================================================================================*\
     * POWER
    \*====================================================================================================*/
    connect(ui->pushButton_power, &QPushButton::released, [=]() {
        if (this->neureset->getPowerState() == ON) {
            this->neureset->togglePower(OFF);
        } else {
            this->neureset->togglePower(ON);
        }

    });
    connect(neureset, &Neureset::updateUI_power, this, &MainWindow::updateUI_power);


    /*====================================================================================================*\
     * GRAPH
    \*====================================================================================================*/
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::graphData);
    graphData();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(graphData()));
    int interval = 1000;
    timer->start(interval);

    /*====================================================================================================*\
     * BATTERY
    \*====================================================================================================*/
    connect(ui->checkBox_pluggedIn, &QCheckBox::stateChanged, [=] () {
        ConnectionState cs = (ui->checkBox_pluggedIn->checkState() == Qt::Checked) ? CONNECTED : DISCONNECTED;
        this->neureset->setChargingState(cs);
    });
    connect(neureset, &Neureset::updateUI_battery, this, &MainWindow::updateUI_battery);

    /*====================================================================================================*\
     * TREATMENT LIGHTS
    \*====================================================================================================*/
    connect(neureset, &Neureset::updateUI_blueLight, this, &MainWindow::updateUI_blueLight);
    connect(neureset, &Neureset::updateUI_greenLight, this, &MainWindow::updateUI_greenLight);
    connect(neureset, &Neureset::updateUI_redLight, this, &MainWindow::updateUI_redLight);

    /*====================================================================================================*\
     * ELECTRODES
    \*====================================================================================================*/
    for(int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        connect(electrodes[e_id], &QCheckBox::stateChanged, [=]() {
            ConnectionState cs = (electrodes[e_id]->isChecked()) ? CONNECTED : DISCONNECTED;
            this->neureset->setElectrode(e_id, cs);
//            qInfo("Electrode %d is %s", e_id, qPrintable(connectionStateToStr(this->headset->getElectrode(e_id))));
        });
    }

    for(int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        for(int fa_id=0; fa_id<3; ++fa_id) {
            connect(electrodeFrequencies[e_id][fa_id], QOverload<int>::of(&QComboBox::currentIndexChanged), [=] (int index) {
                this->neureset->getElectrode(e_id)->generateFrequency(fa_id, indexToBand(index));
            });

            connect(electrodeAmplitudes[e_id][fa_id], QOverload<int>::of(&QSpinBox::valueChanged), [=] (int value) {
                this->neureset->getElectrode(e_id)->setAmplitude(fa_id, (double)value);
            });

        }
    }

//    connect(ui->comboBox_e0_f1, &QComboBox::currentIndexChanged, headset, &Headset::getElectrode());

    /*====================================================================================================*\
     * UI SETUP (to match neureset state)
    \*====================================================================================================*/
    //Set Power
    this->updateUI_blueLight(OFF);
    this->updateUI_greenLight(OFF);
    this->updateUI_redLight(OFF);
    this->neureset->togglePower(ON);

    // Set Menu
    if (this->neureset->getPowerState() == ON) {
        this->updateUI_showMenu();
    }

    // Set Date/Time
    ui->label_dateTimeChanged->hide();
    ui->dateTimeEdit->setDateTime(this->neureset->getCurrentDateTime());
    qInfo("Date/Time is %s", qPrintable(this->neureset->getCurrentDateTime().toString("yyyy-MM-dd HH:mm:ss")));

    // Battery
    ui->progressBar_battery->setValue(this->neureset->getBatteryRemaining());
    if (this->neureset->getChargingState() == CONNECTED){
        ui->checkBox_pluggedIn->setChecked(true);
    } else {
        ui->checkBox_pluggedIn->setChecked(false);
    }

    // Set Electrode Connections
    for(int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        bool connected = (this->neureset->getElectrode(e_id)->getConnectionState() == CONNECTED) ? true : false;
        this->electrodes[e_id]->setChecked(connected);
    }
}



MainWindow::~MainWindow() {
    delete ui;
    delete neureset;
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
    qInfo("Date/Time is %s", qPrintable(this->neureset->getCurrentDateTime().toString("yyyy-MM-dd HH:mm:ss")));
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
    ui->progressBar_battery->setValue(this->neureset->getBatteryRemaining());
    int currentBatteryLevel = ui->progressBar_battery->value();

    if (this->neureset->getPowerState() == ON) {
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

        // Enable Inputs on UI
        ui->frame_e0->setEnabled(true);
        ui->frame_e1->setEnabled(true);
        ui->frame_e2->setEnabled(true);
        ui->frame_e3->setEnabled(true);
        ui->frame_e4->setEnabled(true);
        ui->frame_e5->setEnabled(true);
        ui->frame_e6->setEnabled(true);
        ui->frame_graph->setEnabled(true);
    } else {
        ui->label_powerLight->setStyleSheet("background-color: " + ColourToStr(GREY));
        this->updateUI_hideMenu();
//        ui->progressBar_battery->setStyleSheet("background-color: " + ColourToStr(GREY) +
//                                               "selection-color: " + ColourToStr(GREY) +
//                                               "selection-background-color: " + ColourToStr(GREY));
        // Disable Inputs on UI
        ui->frame_e0->setDisabled(true);
        ui->frame_e1->setDisabled(true);
        ui->frame_e2->setDisabled(true);
        ui->frame_e3->setDisabled(true);
        ui->frame_e4->setDisabled(true);
        ui->frame_e5->setDisabled(true);
        ui->frame_e6->setDisabled(true);
        ui->frame_graph->setDisabled(true);
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
        this->flashTimer->start(200);
    } else {
        if (this->flashTimer->isActive()) {
            this->flashTimer->stop();
        }
        ui->label_redLight->setStyleSheet("background-color: " + ColourToStr(GREY));
    }
}

/*====================================================================================================*\
 * GRAPH
\*====================================================================================================*/
void MainWindow::graphData(){
        int data = 50;
        QVector<double> x(data), y(data);
        if (this->neureset){
            if (this->neureset->getPowerState() == ON){
                ui->customPlot->setEnabled(true);
                ui->customPlot->show();
                    if (this->neureset->getinSession()){
                        double sinMin = 0;
                        double sinMax = 40;
                        double change = (sinMax - sinMin) / (data - 1);
                        for (int i = 0; i < data; ++i) {
                            x[i] = sinMin + i * change;
                            y[i] = rand() / ((double)RAND_MAX) * 2 - 1;
                        }
                            ui->customPlot->addGraph();
                            ui->customPlot->graph(0)->setData(x, y);
                            ui->customPlot->xAxis->setRange(sinMin, sinMax);
                            ui->customPlot->yAxis->setRange(-1, 1);
                            ui->customPlot->replot();
                    } else {
//                        ui->customPlot->graph(0)->setData(x, y);
                        ui->customPlot->clearItems();
                        ui->customPlot->clearPlottables();
                        ui->customPlot->clearGraphs();
                    }

            } else {
                    ui->customPlot->setDisabled(true);
                    ui->customPlot->hide();
            }
        }


}


