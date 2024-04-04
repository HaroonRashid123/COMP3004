#include "Headset.h"

/*====================================================================================================*\
 * CONSTRUCTOR(S)
\*====================================================================================================*/
Headset::Headset() {
    this->electrodes = new ConnectionState[MAX_ELECTRODES];
    this->electrodes[0]  = CONNECTED;
    this->electrodes[1]  = CONNECTED;
    this->electrodes[2]  = CONNECTED;
    this->electrodes[3]  = CONNECTED;
    this->electrodes[4]  = CONNECTED;
    this->electrodes[5]  = CONNECTED;
    this->electrodes[6]  = CONNECTED;
    this->electrodes[7]  = CONNECTED;
    this->electrodes[8]  = CONNECTED;
    this->electrodes[9]  = CONNECTED;
    this->electrodes[10] = CONNECTED;
    this->electrodes[11] = CONNECTED;
    this->electrodes[12] = CONNECTED;
    this->electrodes[13] = CONNECTED;
    this->electrodes[14] = CONNECTED;
    this->electrodes[15] = CONNECTED;
    this->electrodes[16] = CONNECTED;
    this->electrodes[17] = CONNECTED;
    this->electrodes[18] = CONNECTED;
    this->electrodes[19] = CONNECTED;
    this->electrodes[20] = CONNECTED;

    this->connectionState = CONNECTED;
    emit connectionStateChanged(CONNECTED);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
       if (this->connectionState == CONNECTED && this->hasDisconnection()) {
           this->connectionState = DISCONNECTED;
           emit connectionStateChanged(DISCONNECTED);
       } else if (this->connectionState == DISCONNECTED && !this->hasDisconnection()) {
           this->connectionState = CONNECTED;
           emit connectionStateChanged(CONNECTED);
       }
    });
    timer->setSingleShot(false);
    timer->start(1000);
}

/*====================================================================================================*\
 * DESTRUCTOR(S)
\*====================================================================================================*/
Headset::~Headset() { delete[] this->electrodes; }

/*====================================================================================================*\
 * GETTER(S)
\*====================================================================================================*/
ConnectionState Headset::getElectrode(int e_id) { return this->electrodes[e_id]; }
ConnectionState* Headset::getElectrodes() { return this->electrodes; }

/*====================================================================================================*\
 * SETTER(S)
\*====================================================================================================*/
void Headset::setElectrode(int e_id, ConnectionState newCS){ this->electrodes[e_id] = newCS; }

/*====================================================================================================*\
 * SLOT FUNCTIONS(S)
\*====================================================================================================*/
bool Headset::hasDisconnection() {
    for(int e_id=0; e_id < MAX_ELECTRODES; ++e_id) {
        if (this->electrodes[e_id] == DISCONNECTED) { return true; }
    }
    return false;
}
