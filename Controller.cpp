#include "Controller.h"

/*====================================================================================================*\
 * CONSTRUCTOR(S)
\*====================================================================================================*/
Controller::Controller(int batteryRemaining, PowerState powerState, QDate currentDate, QTime currentTime) :
    batteryRemaining(batteryRemaining), powerState(powerState), currentDate(currentDate), currentTime(currentTime),
    chargingState(DISCONNECTED), blueLight(OFF), greenLight(OFF), redLight(OFF)
{}

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

/*====================================================================================================*\
 * SETTER(S)
\*====================================================================================================*/
void Controller::setChargingState(ConnectionState newCS) { this->chargingState = newCS; }
void Controller::setPowerState(PowerState newPS) { this->powerState = newPS; emit togglePower();}

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

void Controller::viewSessionHistory()
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
    }
    emit updateBattery();
}

void Controller::reduceBattery(int percentAmount)
{
    if ((this->powerState == ON) && (this->chargingState == DISCONNECTED) && (batteryRemaining > 0)) {
        if ((batteryRemaining - percentAmount) < 0) {
            batteryRemaining = 0;
        } else {
            batteryRemaining -= percentAmount;
        }
    }
    emit updateBattery();
}
