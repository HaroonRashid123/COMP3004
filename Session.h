#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "ExternalObj.h"

class Session : public QObject
{
    Q_OBJECT

public:
    // Constructor(s)
    Session(QDateTime startDT);

    // Destructor(s)
    ~Session();

    // Getters
    SessionState getSessionState();
    QDateTime getStartDateTime();
    QDateTime getEndDateTime();
    double getBaseline(bool baseline_final, int e_id);
    double* getBaselines(bool baseline_final);

    // Setters
    void setSessionState(SessionState ss);
    void setStartDateTime(QDateTime startDT);
    void setEndDateTime(QDateTime endDT);
    void setBaseline(bool baseline_final, int e_id, double b_value);

private:
    SessionState sessionState;
    QDateTime startDateTime;
    QDateTime endDateTime;
    double baselines_before[MAX_ELECTRODES];
    double baselines_after[MAX_ELECTRODES];

signals:

public slots:

};

#endif // SESSION_H
