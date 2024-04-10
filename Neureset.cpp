#include "Neureset.h"

/*====================================================================================================*\
 * CONSTRUCTOR(S)
\*====================================================================================================*/
Neureset::Neureset(int batteryRemaining, PowerState powerState, QDateTime currentDateTime) :
    batteryRemaining(batteryRemaining), powerState(powerState), currentDateTime(currentDateTime),
    chargingState(DISCONNECTED), blueLight(OFF), greenLight(OFF), redLight(OFF),
    connectionState(CONNECTED), inSession(false), sessionPaused(false), sessionTime(MAX_SESSION_TIME)
{

    // Electrode Connections Setup
    this->electrodes = new Electrode[MAX_ELECTRODES];
    for (int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        this->electrodes[e_id].setConnectionState(CONNECTED);
    }
    this->connectionState = CONNECTED;

    // Automated Electrode Connection checking
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(false);
    connect(timer, &QTimer::timeout, [=]() {
       if (this->connectionState == CONNECTED && this->hasDisconnection()) {
           this->updateConnectionState(DISCONNECTED);
       } else if (this->connectionState == DISCONNECTED && !this->hasDisconnection()) {
           this->updateConnectionState(CONNECTED);
       }
    });
    timer->start(1000);

    // Progress Bar Countdown
    this->sessionTimer = new QTimer(this);
    connect(this->sessionTimer, &QTimer::timeout, [=]() {
        this->sessionTime--;
        emit updateUI_progressBar(((300 - this->sessionTime) * 100) / 300);
        emit updateUI_timerLabel(QString::number(this->sessionTime / 60) + ":" + QString::number(this->sessionTime % 60).rightJustified(2, '0'));
        if (this->sessionTime <= 0) {
            this->sessionTimer->stop();
         }
    });

    // 5 minute cap before session ends
    this->endSessionTimer = new QTimer(this);
    connect(this->endSessionTimer, &QTimer::timeout, [=]() {
        this->stopSession();
        this->togglePower(OFF);
    });

    // Will reduce battery when Power is ON, reduce extra if operating a session. Repeats every 15 seconds
    this->batteryDeplete = new QTimer(this);
    connect(this->batteryDeplete, &QTimer::timeout, [=]() {
        if ((this->powerState == ON) && (this->inSession)) {
            reduceBattery(2);
        } else if ((this->powerState == ON) && (!this->inSession)) {
            reduceBattery(1);
        }
    });
    this->batteryDeplete->start(15000);

    // Will Charge Battery when Plugged in. Repeats every 15 seconds
    this->batteryCharge = new QTimer(this);
    connect(this->batteryCharge, &QTimer::timeout, [=]() {
        if (this->chargingState == CONNECTED) {
            chargeBattery(1);
        }
    });
    this->batteryCharge->start(5000);

//    connect(this,&Neureset::readyForTreatment(), this, &Neureset::deliverTreatment());
}

/*====================================================================================================*\
 * DESTRUCTOR(S)
\*====================================================================================================*/
Neureset::~Neureset() {
    delete[] this->electrodes;
 }

/*====================================================================================================*\
 * GETTER(S)
\*====================================================================================================*/
int Neureset::getBatteryRemaining() { return this->batteryRemaining; }
ConnectionState Neureset::getChargingState() { return this->chargingState; }
PowerState Neureset::getPowerState() { return this->powerState; }
PowerState Neureset::getBlueLight() { return this->blueLight; }
PowerState Neureset::getGreenLight() { return this->greenLight; }
PowerState Neureset::getRedLight() { return this->redLight; }
Electrode* Neureset::getElectrode(int e_id) { return &this->electrodes[e_id]; }
Electrode* Neureset::getElectrodes() {return this->electrodes; }
QDateTime Neureset::getCurrentDateTime() { return this->currentDateTime; }
QVector<Session*> Neureset::getSessionLogs() { return this->sessionLogs; }

/*====================================================================================================*\
 * SETTER(S)
\*====================================================================================================*/
void Neureset::setChargingState(ConnectionState cs) {
    this->chargingState = cs;
}

void Neureset::setPowerState(PowerState ps) {
    emit updateUI_power(this->powerState = ps);
}

void Neureset::setBlueLight(PowerState ps) {
    emit updateUI_blueLight(this->blueLight = ps);
}

void Neureset::setGreenLight(PowerState ps) {
    emit updateUI_greenLight(this->greenLight = ps);
}

void Neureset::setRedLight(PowerState ps) {
    emit updateUI_redLight(this->redLight = ps);
}

void Neureset::setElectrode(int e_id, ConnectionState cs){
    this->electrodes[e_id].setConnectionState(cs);
}

void Neureset::setDateTime(QDateTime dt) {
    this->currentDateTime = dt;
    emit updateUI_dateTimeChanged();
}

/*====================================================================================================*\
 * SLOT FUNCTION(S)
\*====================================================================================================*/
void Neureset::togglePower(PowerState ps) {
    if (ps == OFF) {
        this->setPowerState(OFF);
        this->setBlueLight(OFF);
        this->setRedLight(OFF);
    } else {
        this->setPowerState(ON);
        if (this->connectionState == CONNECTED) {
            this->setBlueLight(ON);
            this->setRedLight(OFF);
        } else {
            this->setBlueLight(OFF);
            this->setRedLight(ON);
        }
    }
    this->setGreenLight(OFF);
}

void Neureset::chargeBattery(int percentAmount) {
    if ((this->chargingState == CONNECTED) && (batteryRemaining < 100)) {
        if ((batteryRemaining + percentAmount) > 100) {
            batteryRemaining = 100;
        } else {
            batteryRemaining += percentAmount;
        }
        emit updateUI_battery();
    }
}

void Neureset::reduceBattery(int percentAmount) {
    if ((this->powerState == ON) && (this->chargingState == DISCONNECTED) && (batteryRemaining > 0)) {
        if ((batteryRemaining - percentAmount) < 0) {
            batteryRemaining = 0;
        } else {
            batteryRemaining -= percentAmount;
        }
        emit updateUI_battery();
    }
}

bool Neureset::hasDisconnection() {
    for(int e_id=0; e_id < MAX_ELECTRODES; ++e_id) {
        if (this->electrodes[e_id].getConnectionState() == DISCONNECTED) { return true; }
    }
    return false;
}

void Neureset::updateConnectionState(ConnectionState cs) {
    if (cs == CONNECTED) {
        this->connectionState = CONNECTED;
        this->setBlueLight(ON);
        this->setRedLight(OFF);
        if (this->inSession && this->sessionPaused) {
            // Play
           this->sessionTimer->start(1000);
           this->sessionPaused = false;
           this->endSessionTimer->stop();
        }
    } else {
        this->connectionState = DISCONNECTED;
        this->setBlueLight(OFF);
        this->setRedLight(ON);
        if (this->inSession && !this->sessionPaused) {
            // Pause
            this->sessionTimer->stop();
            this->sessionPaused = true;
            this->endSessionTimer->start(MAX_DISCONNECT_TIME);
        }
    }

}

void Neureset::playOrPauseSession() {
    if (!this->inSession) {
        if (this->connectionState == CONNECTED) {
            // Start New Session
            Session* session = new Session(this->getCurrentDateTime());
            this->sessionLogs.append(session);
            this->inSession = true;
            this->sessionPaused = false;
            this->sessionTime = MAX_SESSION_TIME;
            this->sessionTimer->start(1000);
        }
    } else {
        if (this->sessionPaused) {
            // Play
            this->sessionTimer->start(1000);
            this->sessionPaused = false;
            this->endSessionTimer->stop();
        } else {
            // Pause
            this->sessionTimer->stop();
            this->sessionPaused = true;
            this->endSessionTimer->start(MAX_DISCONNECT_TIME);
        }
    }
}

void Neureset::stopSession() {
    if (this->inSession) {
        // Stop the End-Session timer
        this->endSessionTimer->stop();

        // Reset Session attributes
        this->sessionLogs.removeLast();
        this->inSession = false;
        this->sessionPaused = false;
        this->sessionTime = MAX_SESSION_TIME;
        this->sessionTimer->stop();

        // Reset TimerUI
        emit updateUI_progressBar(((300 - sessionTime) * 100) / 300);
        emit updateUI_timerLabel(QString::number(sessionTime / 60) + ":" + QString::number(sessionTime % 60).rightJustified(2, '0'));
    }
}

void Neureset::deliverTreatment() {
    // PRE Analysis
    for (int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        double bl = this->electrodes[e_id].calculateBaseline();
        this->sessionLogs.last()->setBaseline(false, e_id, bl);
    }

    // find average of all baselines
    double average_df_preTreatment = 0;
    for (int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        average_df_preTreatment = this->sessionLogs.last()->getBaseline(false, e_id);
    }
    average_df_preTreatment = average_df_preTreatment / MAX_ELECTRODES;

    // Treatmet/Therapy
    // IDK WHAT TO DO:
    /*
    //            make memeber variables
    QTimer *treatmentTimer = new QTimer(this);
    int treatmentCount = 0;
    connect(this->treatmentTimer, &QTimer::timeout, [=]() {
        // deliver treatment (1/16 dominant frequency + offset)
        // find new baseline
        treatmentCount++;
        if (treatmentCount >= 16) {
            this->treatmentTimer->stop();
            treatmentCount = 0;
        }
    });
    */

    // Post Analysis
    for (int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        double bl = this->electrodes[e_id].calculateBaseline();
        this->sessionLogs.last()->setBaseline(true, e_id, bl);
    }

    // find average of all final baselines
    double average_df_postTreatment = 0;
    for (int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        average_df_postTreatment = this->sessionLogs.last()->getBaseline(true, e_id);
    }
    average_df_postTreatment = average_df_postTreatment / MAX_ELECTRODES;
}
