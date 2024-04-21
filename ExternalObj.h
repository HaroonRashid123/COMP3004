#ifndef EXTERNALOBJ_H
#define EXTERNALOBJ_H

#include <random>

#include <QDateTime>
#include <QTimer>
#include <QString>

#define MAX_ELECTRODES 7            // 7 for testing purposesl as per the testing specification
#define MAX_DISCONNECT_TIME 20      // 20 seconds for testing ( usually 5 min)
#define MAX_SESSION_TIME 30         // Adapted to 30 seconds for testing (usually 5 min)
#define SESSION_ROUND_TIME 6

#define DELTA_MIN 1.0
#define DELTA_MAX 4.0

#define THETA_MIN 4.0
#define THETA_MAX 8.0

#define ALPHA_MIN 8.0
#define ALPHA_MAX 12.0

#define BETA_MIN 12.0
#define BETA_MAX 30.0

#define MIN_CHANGE -2.0
#define MAX_CHANGE 5.0

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
    ROUND_1_ANALYSIS,
    ROUND_1_TREATMENT,
    ROUND_2_ANALYSIS,
    ROUND_2_TREATMENT,
    ROUND_3_ANALYSIS,
    ROUND_3_TREATMENT,
    ROUND_4_ANALYSIS,
    ROUND_4_TREATMENT,
    POST_ANALYSIS,
    COMPLETE,
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

// To geenrate variance in readings/results
double getRandomDouble(double min, double max);

#endif // EXTERNALOBJ_H
