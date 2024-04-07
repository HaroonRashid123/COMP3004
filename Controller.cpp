#include "Controller.h"

/*====================================================================================================*\
 * CONSTRUCTOR(S)
\*====================================================================================================*/
Controller::Controller(int batteryRemaining, PowerState powerState, QDateTime currentDateTime) :
    batteryRemaining(batteryRemaining), powerState(powerState), currentDateTime(currentDateTime),
    chargingState(DISCONNECTED), blueLight(OFF), greenLight(OFF), redLight(OFF),
    connectionState(CONNECTED), inSession(false), sessionPaused(false), sessionTime(MAX_SESSION_TIME)
{
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
}

/*====================================================================================================*\
 * DESTRUCTOR(S)
\*====================================================================================================*/
Controller::~Controller() {}

/*====================================================================================================*\
 * GETTER(S)
\*====================================================================================================*/
int Controller::getBatteryRemaining() { return this->batteryRemaining; }
ConnectionState Controller::getChargingState() { return this->chargingState; }
PowerState Controller::getPowerState() { return this->powerState; }
PowerState Controller::getBlueLight() { return this->blueLight; }
PowerState Controller::getGreenLight() { return this->greenLight; }
PowerState Controller::getRedLight() { return this->redLight; }
QDateTime Controller::getCurrentDateTime() { return this->currentDateTime; }
QVector<Session*> Controller::getSessionLogs() { return this->sessionLogs; }

/*====================================================================================================*\
 * SETTER(S)
\*====================================================================================================*/
void Controller::setChargingState(ConnectionState cs) {
    this->chargingState = cs;
}

void Controller::setPowerState(PowerState ps) {
    emit updateUI_power(this->powerState = ps);
}

void Controller::setBlueLight(PowerState ps) {
    emit updateUI_blueLight(this->blueLight = ps);
}

void Controller::setGreenLight(PowerState ps) {
    emit updateUI_greenLight(this->greenLight = ps);
}

void Controller::setRedLight(PowerState ps) {
    emit updateUI_redLight(this->redLight = ps);
}

void Controller::setDateTime(QDateTime dt) {
    this->currentDateTime = dt;
    emit updateUI_dateTimeChanged();
}

/*====================================================================================================*\
 * SLOT FUNCTION(S)
\*====================================================================================================*/
void Controller::togglePower(PowerState ps) {
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

void Controller::chargeBattery(int percentAmount) {
    if ((this->chargingState == CONNECTED) && (batteryRemaining < 100)) {
        if ((batteryRemaining + percentAmount) > 100) {
            batteryRemaining = 100;
        } else {
            batteryRemaining += percentAmount;
        }
        emit updateUI_battery();
    }
}

void Controller::reduceBattery(int percentAmount) {
    if ((this->powerState == ON) && (this->chargingState == DISCONNECTED) && (batteryRemaining > 0)) {
        if ((batteryRemaining - percentAmount) < 0) {
            batteryRemaining = 0;
        } else {
            batteryRemaining -= percentAmount;
        }
        emit updateUI_battery();
    }
}

void Controller::updateConnectionState(ConnectionState cs) {
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

void Controller::playOrPauseSession() {
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

void Controller::stopSession() {
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
