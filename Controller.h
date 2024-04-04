#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "ExternalObj.h"

#include "Session.h"


class Controller : public QObject
{
    Q_OBJECT
public:
    // Constructor(s)
    Controller(int batteryRemaining = 100, PowerState powerState = ON, QDateTime currentDateTime = QDateTime(QDate(2024, 1, 1), QTime(0,0,0)));

    // Destructor(s)
    ~Controller();

    // Getter(s)
    int getBatteryRemaining();
    ConnectionState getChargingState();
    PowerState getPowerState();
    PowerState getBlueLight();
    PowerState getRedLight();
    PowerState getGreenLight();
    QDateTime getCurrentDateTime();
    QVector<Session> getSessionLogs();

    // Setter(s)
    void setChargingState(ConnectionState newCS);
    void setPowerState(PowerState newPS);
    void setBlueLight(PowerState newPS);
    void setRedLight(PowerState newPS);
    void setGreenLight(PowerState newPS);
    void setDateTime(QDateTime newDT);

    // Member Variable(s)
private:
    int batteryRemaining;
    ConnectionState chargingState;
    PowerState powerState;
    PowerState blueLight;
    PowerState greenLight;
    PowerState redLight;
    QDateTime currentDateTime;

    //Session Related
    QVector<Session> sessionLogs;
    bool inSession;
    QTimer *sessionTimer;
        bool sessionPaused;
        int currentTime;

signals:
    void updateBattery();
    void togglePower();
    void updateProgressBar(int value);
     void updateTimerLabel(const QString &text);

public slots:
    void startNewSession();
    void setDateTime();
    void viewsessionLogs();
    void chargeBattery(int percentAmount);
    void reduceBattery(int percentAmount);
    void playPauseTimer();
    void resetTimer();

};

#endif // CONTROLLER_H
