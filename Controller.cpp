#include "Controller.h"

/*====================================================================================================*\
 * CONSTRUCTOR(S)
\*====================================================================================================*/
Controller::Controller(int batteryRemaining, PowerState powerState, QDateTime currentDateTime) :
    batteryRemaining(batteryRemaining), powerState(powerState), currentDateTime(currentDateTime),
    chargingState(DISCONNECTED), blueLight(OFF), greenLight(OFF), redLight(OFF),
    inSession(false), sessionPaused(false), currentTime(300)
{
    this->sessionTimer = new QTimer(this);
    connect(this->sessionTimer, &QTimer::timeout, [=]() {
        currentTime--;
        emit updateUI_progressBar(((300 - currentTime) * 100) / 300);
        emit updateUI_timerLabel(QString::number(currentTime / 60) + ":" + QString::number(currentTime % 60).rightJustified(2, '0'));
        if (currentTime <= 0) {
            this->sessionTimer->stop();
         }
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
    this->powerState = ps;
    emit updateUI_power(ps);
}

void Controller::setBlueLight(PowerState ps) {
    this->blueLight = ps;
    emit updateUI_blueLight(ps);
}

void Controller::setGreenLight(PowerState ps) {
    this->greenLight = ps;
    emit updateUI_greenLight(ps);
}

void Controller::setRedLight(PowerState ps) {
    this->redLight = ps;
    emit updateUI_redLight(ps);
}

void Controller::setDateTime(QDateTime newDT) { this->currentDateTime = newDT; }

/*====================================================================================================*\
 * SLOT FUNCTION(S)
\*====================================================================================================*/
void Controller::togglePower() {
    if (this->powerState == ON) {
        this->setPowerState(OFF);
    } else {

        this->setPowerState(ON);
    }
    this->setBlueLight(OFF);
    this->setGreenLight(OFF);
    this->setRedLight(OFF);
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
    if (inSession) {
        if (cs == CONNECTED) {
            emit updateUI_blueLight(ON);
            emit updateUI_redLight(OFF);
        } else {
            emit updateUI_blueLight(OFF);
            emit updateUI_redLight(ON);
        }
    }
}

void Controller::startNewSession()
{
    Session* session = new Session(this->getCurrentDateTime());
    this->sessionLogs.append(session);
    this->sessionTimer->start(1000);

    // Connection Lights
    emit checkHeadsetConnections();

}

void Controller::playOrPauseSession() {
    if (!this->inSession) {
        startNewSession();
    } else {
        if (this->sessionTimer->isActive()) {
            this->sessionTimer->stop();
            this->sessionPaused = true;
        } else {
            this->sessionTimer->start(1000);
            this->sessionPaused = false;
        }
    }
}

void Controller::stopSession() {
    if (this->inSession) {
        // TODO: Erase session
        currentTime = 300;
        emit updateUI_progressBar(((300 - currentTime) * 100) / 300);
        emit updateUI_timerLabel(QString::number(currentTime / 60) + ":" + QString::number(currentTime % 60).rightJustified(2, '0'));
        this->sessionTimer->stop();
    }
}


