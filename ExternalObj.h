#ifndef EXTERNALOBJ_H
#define EXTERNALOBJ_H

#include <QDateTime>
#include <QTimer>

#include <QString>

#define MAX_ELECTRODES 7            // 7 for testing purposesl as per the testing specification
#define MAX_DISCONNECT_TIME 20000   // milliseconds ( = 5 min)
#define MAX_SESSION_TIME 300        // seconds (= 5 min)

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

// Enum to string mapping function for Colour
QString ColourToStr(Colour colour);

// Enum to string mapping function for PowerState
QString powerStateToStr(PowerState state);

// Enum to string mapping function for ConnectionState
QString connectionStateToStr(ConnectionState state);

#endif // EXTERNALOBJ_H
