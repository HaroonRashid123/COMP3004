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
    QVector<Session*> getSessionLogs();

    // Setter(s)
    void setChargingState(ConnectionState cs);
    void setPowerState(PowerState ps);
    void setBlueLight(PowerState ps);
    void setRedLight(PowerState ps);
    void setGreenLight(PowerState ps);
    void setDateTime(QDateTime newDT);

    // Member Variable(s)
private:
    int batteryRemaining;
    ConnectionState chargingState;
    PowerState powerState;
    PowerState blueLight;
    PowerState greenLight;
    PowerState redLight;

    ConnectionState connectionState;
    QDateTime currentDateTime;

    //Session Related
    QVector<Session*> sessionLogs;
    QTimer* sessionTimer;
    bool inSession; 
    bool sessionPaused;
    int currentTime;

signals:
    void updateUI_power(PowerState ps);
    void updateUI_battery();
    void updateUI_blueLight(PowerState ps);
    void updateUI_greenLight(PowerState ps);
    void updateUI_redLight(PowerState ps);
    void updateUI_progressBar(int value);
    void updateUI_timerLabel(const QString &text);

    void updateUI_dateTimeChanged();

public slots:
    void togglePower();
    void chargeBattery(int percentAmount);
    void reduceBattery(int percentAmount);

     void updateConnectionState(ConnectionState cs);

    void startNewSession();
    void playOrPauseSession();
    void stopSession();

};

#endif // CONTROLLER_H
