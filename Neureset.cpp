#include "Neureset.h"

/*====================================================================================================*\
 * CONSTRUCTOR(S)
\*====================================================================================================*/
Neureset::Neureset(int batteryRemaining, PowerState powerState, QDateTime currentDateTime) :
    batteryRemaining(batteryRemaining), powerState(powerState), currentDateTime(currentDateTime),
    chargingState(DISCONNECTED), blueLight(OFF), greenLight(OFF), redLight(OFF),
    connectionState(CONNECTED), inSession(false), sessionPaused(false),
    remainingSessionTime(MAX_SESSION_TIME), remainingDisconnectTime(MAX_DISCONNECT_TIME)
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
        this->remainingSessionTime--;
        emit updateUI_progressBar(((MAX_SESSION_TIME - this->remainingSessionTime) * 100) / MAX_SESSION_TIME);
        emit updateUI_timerLabel(QString::number(this->remainingSessionTime / 60) + ":" + QString::number(this->remainingSessionTime % 60).rightJustified(2, '0'));
        // Session Flow
        this->deliverTreatment();
        this->reduceBattery(1);
    });

    // 5 minute cap before session ends
    this->endSessionTimer = new QTimer(this);
    connect(this->endSessionTimer, &QTimer::timeout, [=]() {
        if (this->remainingDisconnectTime == 0) {
            this->stopSession();
            this->togglePower(OFF);
            // Reset Timer
            this->remainingDisconnectTime = MAX_DISCONNECT_TIME;
            this->endSessionTimer->stop();
        } else {
            this->remainingDisconnectTime--;
        }
    });

    // Will reduce battery when Power is ON, reduce extra if operating a session. Repeats every 15 seconds
    this->batteryDeplete = new QTimer(this);
    connect(this->batteryDeplete, &QTimer::timeout, [=]() {
        if ((this->powerState == ON) && (!this->inSession)) {
            this->reduceBattery(1);
        }
    });
    this->batteryDeplete->start(15000);

    // Will Charge Battery when Plugged in. Repeats every 15 seconds
    this->batteryCharge = new QTimer(this);
    connect(this->batteryCharge, &QTimer::timeout, [=]() {
        if (this->chargingState == CONNECTED) {
            chargeBattery(2);
        }
    });
    this->batteryCharge->start(1000);

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
bool Neureset::getSessionPaused() { return this->sessionPaused; }
QVector<Session*> Neureset::getSessionLogs() { return this->sessionLogs; }
bool Neureset::getinSession() {return this->inSession; }

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

        if (this->inSession) {
            // Pause
            this->sessionTimer->stop();
            this->sessionPaused = true;
            this->endSessionTimer->stop();
            qInfo("Saving session progress.");
        }
        qInfo("Neureset powering OFF");
    } else {
        this->setPowerState(ON);
        qInfo("Neureset powering ON");

        if (this->connectionState == CONNECTED) {
            this->setBlueLight(ON);
            this->setRedLight(OFF);
        } else {
            this->setBlueLight(OFF);
            this->setRedLight(ON);
        }

        if (this->inSession) {
            // Pause
            this->sessionTimer->stop();
            this->sessionPaused = true;
            this->endSessionTimer->start(this->remainingDisconnectTime);
            qInfo("Press play to resume session.");
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
            this->remainingSessionTime = MAX_SESSION_TIME;
            this->sessionTimer->start(1000);
            qInfo("Therapy session starting.");
        }
    } else {
        if (this->sessionPaused) {
            // Play
            this->sessionTimer->start(1000);
            this->sessionPaused = false;
            this->endSessionTimer->stop();
            qInfo("Therapy session resuming.");
        } else {
            // Pause
            this->sessionTimer->stop();
            this->sessionPaused = true;
            this->endSessionTimer->start(MAX_DISCONNECT_TIME);
            qInfo("Therapy session paused.");
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
        this->remainingSessionTime = MAX_SESSION_TIME;
        this->sessionTimer->stop();

        // Reset TimerUI
        emit updateUI_progressBar(0);
        emit updateUI_timerLabel(QString::number(remainingSessionTime / 60) + ":" + QString::number(remainingSessionTime % 60).rightJustified(2, '0'));
    }
}

void Neureset::deliverTreatment() {
    Session* currentSession = this->sessionLogs.back();

    // Session Flow
    if (this->remainingSessionTime == (MAX_SESSION_TIME - 1)) {
        qInfo("ROUND 1: Reading input waveforms, calculating ADF.");
        currentSession->setSessionState(ROUND_1_ANALYSIS);
        for (int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
            currentSession->setBaseline(false, e_id, this->electrodes[e_id].calculateBaseline());
        }
    } else if (this->remainingSessionTime == (MAX_SESSION_TIME - 6)) {
        qInfo("ROUND 1: Delivering the 1 sec feedback at 1/16 of dominant + offset.");
        this->setGreenLight(ON);
        currentSession->setSessionState(ROUND_1_TREATMENT);
    } else if (this->remainingSessionTime == (MAX_SESSION_TIME - 7)) {
        qInfo("ROUND 1: Treatment finished.");
        this->setGreenLight(OFF);
        qInfo("ROUND 2: Reading input waveforms, calculating ADF.");
        currentSession->setSessionState(ROUND_2_ANALYSIS);
    } else if (this->remainingSessionTime == (MAX_SESSION_TIME - 12)) {
        qInfo("ROUND 2: Delivering the 1 sec feedback at 1/16 of dominant + offset.");
        this->setGreenLight(ON);
        currentSession->setSessionState(ROUND_2_TREATMENT);
    } else if (this->remainingSessionTime == (MAX_SESSION_TIME - 13)) {
        qInfo("ROUND 2: Treatment finished.");
        this->setGreenLight(OFF);
        qInfo("ROUND 3: Reading input waveforms, calculating ADF.");
        currentSession->setSessionState(ROUND_3_ANALYSIS);
    } else if (this->remainingSessionTime == (MAX_SESSION_TIME - 18)) {
        qInfo("ROUND 3: Delivering the 1 sec feedback at 1/16 of dominant + offset.");
        this->setGreenLight(ON);
        currentSession->setSessionState(ROUND_3_TREATMENT);
    } else if (this->remainingSessionTime == (MAX_SESSION_TIME - 19)) {
        qInfo("ROUND 3: Treatment finished.");
        this->setGreenLight(OFF);
        qInfo("ROUND 4: Reading input waveforms, calculating ADF.");
        currentSession->setSessionState(ROUND_4_ANALYSIS);
    } else if (this->remainingSessionTime == (MAX_SESSION_TIME - 24)) {
        qInfo("ROUND 4: Delivering the 1 sec feedback at 1/16 of dominant + offset.");
        this->setGreenLight(ON);
        currentSession->setSessionState(ROUND_4_TREATMENT);
    } else if (this->remainingSessionTime == (MAX_SESSION_TIME - 25)) {
        qInfo("ROUND 4: Treatment finished.");
        this->setGreenLight(OFF);
        qInfo("POST ANALYSIS: Reading input waveforms, calculating ADF.");
        currentSession->setSessionState(POST_ANALYSIS);
        int random = 1;
        for (int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
            currentSession->setBaseline(true, e_id, this->electrodes[e_id].calculateBaseline()+random);
        }
    } else if (this->remainingSessionTime == (MAX_SESSION_TIME - 30) /* == 0 */ ) {
        qInfo("POST ANALYSIS: Finished. Saving Session.");
        currentSession->setSessionState(COMPLETE);

        // Reset Session attributes
        this->sessionTimer->stop();
        this->inSession = false;
        this->sessionPaused = false;
        this->remainingSessionTime = MAX_SESSION_TIME;
        this->remainingDisconnectTime = MAX_DISCONNECT_TIME;

        qInfo("Session Saved.");
    }
}

void Neureset::uploadLogs() {
}
