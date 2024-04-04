#include "ExternalObj.h"

// Helper Functions
QString ColourToStr(Colour colour) {
    switch(colour) {
        case RED:
            return "rgb(224, 27, 36);";
        case BLUE:
            return "rgb(26, 95, 180);";
        case GREEN:
            return "rgb(38, 162, 105);";
        case YELLOW:
            return "rgb(249, 240, 107);";
        case PINK:
            return "rgb(220, 138, 221);";
        case GREY:
            return "rgb(94, 92, 100);";
        case BLACK:
            return "rgb(0, 0, 0);";
        default:
            return "";
    }
}

QString powerStateToStr(PowerState state) {
    switch (state) {
        case OFF: return "OFF";
        case ON: return "ON";
        default: return "Unknown Power State";
    }
}

QString connectionStateToStr(ConnectionState state) {
    switch (state) {
        case DISCONNECTED: return "DISCONNECTED";
        case CONNECTED: return "CONNECTED";
        default: return "Unknown Connection State";
    }
}
