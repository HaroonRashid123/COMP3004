#include "Electrode.h"

/*====================================================================================================*\
 * CONSTRUCTOR(S)
\*====================================================================================================*/
Electrode::Electrode(int id) : id(id) {


}

Electrode::Electrode()
{}

/*====================================================================================================*\
 * DESTRUCTOR(S)
\*====================================================================================================*/
Electrode::~Electrode()
{}

/*====================================================================================================*\
 * GETTER(S)
\*====================================================================================================*/
ConnectionState Electrode::getConnectionState()  {
    return this->connectionState;
}

double Electrode::getFrequency(int f_id) {
    if (f_id == 1) {
        return this->f1;
    } else if (f_id == 2) {
        return this->f2;
    } else if (f_id == 3) {
        return this->f3;
    } else {
        return -1;
    }
}

double Electrode::getAmplitude(int a_id) {
    if (a_id == 1) {
        return this->a1;
    } else if (a_id == 2) {
        return this->a2;
    } else if (a_id == 3) {
        return this->a3;
    } else {
        return -1;
    }
}

/*====================================================================================================*\
 * SETTER(S)
\*====================================================================================================*/
void Electrode::setConnectionState(ConnectionState cs) {
     emit connectionStateChanged(this->connectionState = cs);
}

void Electrode::setFrequency(int f_id, double frequency) {
    if (f_id == 1) {
        this->f1 = frequency;
    } else if (f_id == 2) {
        this->f2 = frequency;
    } else if (f_id == 3) {
        this->f3 = frequency;
    }
}

void Electrode::setAmplitude(int a_id, double amplitude) {
    if (a_id == 1) {
        this->a1 = amplitude;
    } else if (a_id == 2) {
        this->a2 = amplitude;
    } else if (a_id == 3) {
        this->a3 = amplitude;
    }
}

/*====================================================================================================*\
 * SLOT FUNCTION(S)
\*====================================================================================================*/
void Electrode::generateFrequency(int f_id, Band bandType) {
    double freq = 0;
    // TODO: Generate random frequency
    if (bandType == DELTA) {
        freq = 1;
    } else if (bandType == THETA) {
        freq = 4;
    } else if (bandType == ALPHA) {
        freq = 8;
    } else if (bandType == BETA) {
        freq = 12;
    } else {
        return;
    }
    this->setFrequency(f_id, freq);
}




