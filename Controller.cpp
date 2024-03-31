#include "Controller.h"

Controller::Controller(int batteryRemaining, PowerState poweredOn, QDate currentDate, QTime currentTime) :
    batteryRemaining(batteryRemaining), poweredOn(poweredOn), currentDate(currentDate), currentTime(currentTime)
{
}

Controller::~Controller()
{

}

int Controller::getBatteryRemaining() {
    return this->batteryRemaining;
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
    if (batteryRemaining > 0) {
        if ((batteryRemaining - percentAmount) < 0) {
            batteryRemaining = 0;
        } else {
            batteryRemaining -= percentAmount;
        }
    }
    emit updateBattery(batteryRemaining);
}
