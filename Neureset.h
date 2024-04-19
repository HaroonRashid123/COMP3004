#ifndef NEURESET_H
#define NEURESET_H

#include <QObject>
#include "ExternalObj.h"

#include "Session.h"
#include "Electrode.h"

class Neureset : public QObject
{
    Q_OBJECT
public:
    // Constructor(s)
    Neureset(int batteryRemaining = 100, PowerState powerState = ON, QDateTime currentDateTime = QDateTime(QDate(2024, 1, 1), QTime(0,0,0)));

    // Destructor(s)
    ~Neureset();

    // Getter(s)
    int getBatteryRemaining();
    bool getinSession();
    ConnectionState getChargingState();
    PowerState getPowerState();
    PowerState getBlueLight();
    PowerState getRedLight();
    PowerState getGreenLight();

    Electrode* getElectrode(int e_id);
    Electrode* getElectrodes();

    QDateTime getCurrentDateTime();

    bool getSessionPaused();
    QVector<Session*> getSessionLogs();

    // Setter(s)
    void setChargingState(ConnectionState cs);
    void setPowerState(PowerState ps);
    void setBlueLight(PowerState ps);
    void setRedLight(PowerState ps);
    void setGreenLight(PowerState ps);

    void setElectrode(int e_id, ConnectionState cs);
    void setDateTime(QDateTime dt);

    // Member Variable(s)
private:
    int batteryRemaining;
    QTimer* batteryCharge;
    QTimer* batteryDeplete;
    ConnectionState chargingState;
    PowerState powerState;

    PowerState blueLight;
    PowerState greenLight;
    PowerState redLight;

    ConnectionState connectionState;
    Electrode *electrodes;
    QDateTime currentDateTime;

    //Session Related
    bool inSession;
    bool sessionPaused;
    int remainingSessionTime;
    int remainingDisconnectTime;
    QVector<Session*> sessionLogs;
    QTimer* sessionTimer;
    QTimer* endSessionTimer;

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
    void togglePower(PowerState ps);
    void chargeBattery(int percentAmount);
    void reduceBattery(int percentAmount);

    bool hasDisconnection();
    void updateConnectionState(ConnectionState cs);

    void uploadLogs();
    void deliverTreatment();
    void playOrPauseSession();
    void stopSession();

};

#endif // NEURESET_H
