#ifndef EXTERNALOBJ_H
#define EXTERNALOBJ_H

#include <QDateTime>
#include <QTimer>

#include <QString>

#define MAX_ELECTRODES 7            // 7 for testing purposesl as per the testing specification
#define MAX_DISCONNECT_TIME 20000   // milliseconds ( = 5 min)
#define MAX_SESSION_TIME 300        // seconds (= 5 min)

#define DELTA_MIN 1.0
#define DELTA_MAX 4.0

#define THETA_MIN 4.0
#define THETA_MAX 8.0

#define ALPHA_MIN 8.0
#define ALPHA_MAX 12.0

#define BETA_MIN 12.0
#define BETA_MAX 30.0

#define OFFSET 5


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
    INCOMPLETE = -1,
    ROUND_ONE,
    ROUND_TWO,
    ROUND_THREE,
    ROUND_FOUR,
    COMPLETE,
};

enum TreatmentState {
    PRE_ANALYSIS,
    TREATMENT,
    POST_ANALYSIS
};

enum Band {
    UNKNOWN_BAND = -1,
    DELTA,
    THETA,
    ALPHA,
    BETA,
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

// Index to ENUM mapping function for UI wave input
Band indexToBand(int index);

// Enum to string mapping function for Colour
QString ColourToStr(Colour colour);

// Enum to string mapping function for PowerState
QString powerStateToStr(PowerState state);

// Enum to string mapping function for ConnectionState
QString connectionStateToStr(ConnectionState state);

#endif // EXTERNALOBJ_H
