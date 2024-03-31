#include "Controller.h"

// Constructor(s)
Controller::Controller(int batteryRemaining, PowerState powerState, QDate currentDate, QTime currentTime) :
    batteryRemaining(batteryRemaining), powerState(powerState), currentDate(currentDate), currentTime(currentTime)
{
}

// Destructor(s)
Controller::~Controller()
{

}

// Getter(s)
int Controller::getBatteryRemaining() {
    return this->batteryRemaining;
}

ConnectionState Controller::getChargingState() {
    return this->chargingState;
}

PowerState Controller::getPowerState() {
    return this->powerState;
}

// Setter(s)
void Controller::setChargingState(ConnectionState newCS) {
    this->chargingState = newCS;
    if (this->chargingState == CONNECTED) {
        this->chargeBattery(100);
    }
}

void Controller::setPowerState(PowerState newPS) {
    this->powerState = newPS;
    emit togglePower();
}


void Controller::startNewSession()
{

}

void Controller::setDateTime()
{

}


void Controller::viewSessionHistory()
{

}

void Controller::chargeBattery(int percentAmount)
{
    if (batteryRemaining < 100) {
        if ((batteryRemaining + percentAmount) > 100) {
            batteryRemaining = 100;
        } else {
            batteryRemaining += percentAmount;
        }
    }
    emit updateBattery(batteryRemaining);
}

void Controller::reduceBattery(int percentAmount)
{
    if ((this->chargingState == DISCONNECTED) && (batteryRemaining > 0)) {
        if ((batteryRemaining - percentAmount) < 0) {
            batteryRemaining = 0;
        } else {
            batteryRemaining -= percentAmount;
        }
    }
    emit updateBattery(batteryRemaining);
}
