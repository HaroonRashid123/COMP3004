#include "Controller.h"

/*====================================================================================================*\
 * CONSTRUCTOR(S)
\*====================================================================================================*/
Controller::Controller(int batteryRemaining, PowerState powerState, QDateTime currentDateTime) :
    batteryRemaining(batteryRemaining), powerState(powerState), currentDateTime(currentDateTime),
    chargingState(DISCONNECTED), blueLight(OFF), greenLight(OFF), redLight(OFF), isTimerPaused(false), currentTime(300)
{
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [=]() {
            currentTime--;
            emit updateProgressBar(((300 - currentTime) * 100) / 300);
            emit updateTimerLabel(QString::number(currentTime / 60) + ":" + QString::number(currentTime % 60).rightJustified(2, '0'));
            if (currentTime <= 0) {
                timer->stop();
             }
        });
}

/*====================================================================================================*\
 * DESTRUCTOR(S)
\*====================================================================================================*/
Controller::~Controller()
{}

/*====================================================================================================*\
 * GETTER(S)
\*====================================================================================================*/
int Controller::getBatteryRemaining() { return this->batteryRemaining; }
ConnectionState Controller::getChargingState() { return this->chargingState; }
PowerState Controller::getPowerState() { return this->powerState; }
QDateTime Controller::getCurrentDateTime() { return this->currentDateTime; }
QVector<Session> Controller::getSessionLogs() { return this->sessionLogs; }

/*====================================================================================================*\
 * SETTER(S)
\*====================================================================================================*/
void Controller::setChargingState(ConnectionState newCS) { this->chargingState = newCS; }
void Controller::setPowerState(PowerState newPS) { this->powerState = newPS; emit togglePower();}
void Controller::setDateTime(QDateTime newDT) { this->currentDateTime = newDT; }

/*====================================================================================================*\
 * SLOT FUNCTION(S)
\*====================================================================================================*/
void Controller::startNewSession()
{
    if (this->powerState == ON) {}
}

void Controller::setDateTime() {
    if (this->powerState == ON) {}
}

void Controller::viewsessionLogs()
{
    if (this->powerState == ON) {}
}

void Controller::chargeBattery(int percentAmount)
{
    if ((this->chargingState == CONNECTED) && (batteryRemaining < 100)) {
        if ((batteryRemaining + percentAmount) > 100) {
            batteryRemaining = 100;
        } else {
            batteryRemaining += percentAmount;
        }
        emit updateBattery();
    }
}

void Controller::reduceBattery(int percentAmount)
{
    if ((this->powerState == ON) && (this->chargingState == DISCONNECTED) && (batteryRemaining > 0)) {
        if ((batteryRemaining - percentAmount) < 0) {
            batteryRemaining = 0;
        } else {
            batteryRemaining -= percentAmount;
        }
        emit updateBattery();
    }
}

void Controller::playPauseTimer()
{
    if (timer->isActive()) {
        timer->stop();
        isTimerPaused = true;
    } else {
        timer->start(1000);
        isTimerPaused = false;
    }
}

void Controller::resetTimer()
{
    currentTime = 300;
    emit updateProgressBar(((300 - currentTime) * 100) / 300);
    emit updateTimerLabel(QString::number(currentTime / 60) + ":" + QString::number(currentTime % 60).rightJustified(2, '0'));
    timer->stop();
}


