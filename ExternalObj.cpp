#include "ExternalObj.h"

// Helper Functions
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
