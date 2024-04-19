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
    int getBaseline(bool baseline_final, int e_id);
    int* getBaselines(bool baseline_final);

    // Setters
    void setSessionState(SessionState ss);
    void setStartDateTime(QDateTime startDT);
    void setBaseline(bool baseline_final, int e_id, int b_value);

private:
    SessionState sessionState;
    QDateTime startDateTime;

    int baselines_before[21];
    int baselines_after[21];

signals:

public slots:

};

#endif // SESSION_H
