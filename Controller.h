#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QVector>

#include "ExternalObj.h"


class Controller : public QObject
{
    Q_OBJECT
public:
    // Constructor(s)
    Controller(int batteryRemaining = 100, PowerState powerState = ON, QDate currentDate = QDate(2024, 1, 1), QTime currentTime = QTime(0,0,0));

    // Destructor(s)
    ~Controller();

    // Getter(s)
    int getBatteryRemaining();
    ConnectionState getChargingState();
    PowerState getPowerState();

    // Setter(s)
    void setChargingState(ConnectionState newCS);
    void setPowerState(PowerState newPS);

    // Member Variable(s)
private:
    int batteryRemaining;
    ConnectionState chargingState;
    PowerState powerState;
    PowerState blueLight;
    PowerState greenLight;
    PowerState redLight;

    QDate currentDate;
    QTime currentTime;
    QVector<Session> sessionHistory;

signals:
    void updateBattery();
    void togglePower();

public slots:
    void startNewSession();
    void setDateTime();
    void viewSessionHistory();

    void chargeBattery(int percentAmount);
    void reduceBattery(int percentAmount);
};

#endif // CONTROLLER_H
