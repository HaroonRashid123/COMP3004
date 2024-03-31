#ifndef EXTERNALOBJ_H
#define EXTERNALOBJ_H

#include <QDateTime>
#include <QVector>

enum PowerState {
    OFF,
    ON
};

enum ConnectionState {
    DISCONNECTED,
    CONNECTED
};

struct Session {
    QDate date;
    QTime time;
    int baselines_before[21];
    int baselines_after[21];
};

#endif // EXTERNALOBJ_H
