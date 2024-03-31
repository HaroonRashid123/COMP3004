#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QVector>

#include "ExternalObj.h"


class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(int batteryRemaining = 100, PowerState poweredOn = ON, QDate currentDate = QDate(2024, 1, 1), QTime currentTime = QTime(0,0,0));
    ~Controller();
    int getBatteryRemaining();

private:
    int batteryRemaining;
    PowerState poweredOn;
    QDate currentDate;
    QTime currentTime;
    QVector<Session> sessionHistory;

signals:
    void updateBattery(int batteryRemaining);

public slots:

    void startNewSession();
    void setDateTime();
    void viewSessionHistory();

    void chargeBattery(int percentAmount);
    void reduceBattery(int percentAmount);
};

#endif // CONTROLLER_H
