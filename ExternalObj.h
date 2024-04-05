#ifndef EXTERNALOBJ_H
#define EXTERNALOBJ_H

#include <QDateTime>
#include <QTimer>

#include <QString>

#define MAX_ELECTRODES 21

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

enum SessionState {
    IN_PROGRESS,
    PAUSED,
    COMPLETE,
};


enum Colour {
    RED,
    BLUE,
    GREEN,
    YELLOW,
    PINK,
    GREY,
    BLACK,
};

QString ColourToStr(Colour colour);


// Enum to string mapping function for PowerState
QString powerStateToStr(PowerState state);

// Enum to string mapping function for ConnectionState
QString connectionStateToStr(ConnectionState state);

#endif // EXTERNALOBJ_H
