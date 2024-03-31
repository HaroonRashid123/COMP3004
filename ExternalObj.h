#ifndef EXTERNALOBJ_H
#define EXTERNALOBJ_H

#include <QDateTime>
#include <QVector>

// To represent the power state of an electronic componenent
enum PowerState {
    OFF,
    ON
};

// To represent the connection state of a connection componenent
enum ConnectionState {
    DISCONNECTED,
    CONNECTED
};

// Models a neureset session
struct Session {
    QDate date;
    QTime time;
    int baselines_before[21];
    int baselines_after[21];
};

#endif // EXTERNALOBJ_H
