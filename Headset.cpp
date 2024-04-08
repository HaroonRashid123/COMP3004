#include "Headset.h"

/*====================================================================================================*\
 * CONSTRUCTOR(S)
\*====================================================================================================*/
Headset::Headset() {
    this->electrodes = new Electrode[MAX_ELECTRODES];
    for (int e_id=0; e_id<MAX_ELECTRODES; ++e_id) {
        this->electrodes[e_id].setConnectionState(CONNECTED);
    }
    this->connectionState = CONNECTED;
    emit connectionStateChanged(CONNECTED);

    QTimer *timer = new QTimer(this);
    timer->setSingleShot(false);
    connect(timer, &QTimer::timeout, [=]() {
       if (this->connectionState == CONNECTED && this->hasDisconnection()) {
           this->connectionState = DISCONNECTED;
           emit connectionStateChanged(DISCONNECTED);
       } else if (this->connectionState == DISCONNECTED && !this->hasDisconnection()) {
           this->connectionState = CONNECTED;
           emit connectionStateChanged(CONNECTED);
       }
    });

    timer->start(1000);
}

/*====================================================================================================*\
 * DESTRUCTOR(S)
\*====================================================================================================*/
Headset::~Headset() { delete[] this->electrodes; }

/*====================================================================================================*\
 * GETTER(S)
\*====================================================================================================*/
Electrode* Headset::getElectrode(int e_id) { return &this->electrodes[e_id]; }
Electrode* Headset::getElectrodes() {return this->electrodes; }

/*====================================================================================================*\
 * SETTER(S)
\*====================================================================================================*/
void Headset::setElectrode(int e_id, ConnectionState cs){
    this->electrodes[e_id].setConnectionState(cs);
}

/*====================================================================================================*\
 * SLOT FUNCTIONS(S)
\*====================================================================================================*/
bool Headset::hasDisconnection() {
    for(int e_id=0; e_id < MAX_ELECTRODES; ++e_id) {
        if (this->electrodes[e_id].getConnectionState() == DISCONNECTED) { return true; }
    }
    return false;
}
