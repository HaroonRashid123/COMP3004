#include "Session.h"

/*====================================================================================================*\
 * CONSTRUCTOR(S)
\*====================================================================================================*/
Session::Session(QDateTime startDateTime) : startDateTime(startDateTime) {}

//Session::Session(const Session &session) {
//    this->startDateTime = session.startDateTime;
//    std::copy(std::begin(session.baselines_after), std::end(session.baselines_after), std::begin(this->baselines_after));
//    std::copy(std::begin(session.baselines_before), std::end(session.baselines_before), std::begin(this->baselines_before));
//}

/*====================================================================================================*\
 * DESTRUCTOR(S)
\*====================================================================================================*/
Session::~Session() {}

/*====================================================================================================*\
 * GETTER(S)
\*====================================================================================================*/
SessionState Session::getSessionState() {
    return this->sessionState;
}

QDateTime Session::getStartDateTime() { return this->startDateTime; }

QDateTime Session::getEndDateTime() { return this->endDateTime; }

double Session::getBaseline(bool baseline_final, int e_id) {
    if ((e_id < 0) || (e_id > 21)){ return -1; }

    if (baseline_final) {
        return this->baselines_after[e_id];
    }
    return this->baselines_before[e_id];
}

double* Session::getBaselines(bool baseline_final) {
    if (baseline_final) {
        return this->baselines_after;
    }
    return this->baselines_before;
}

/*====================================================================================================*\
 * SETTER(S)
\*====================================================================================================*/
void Session::setSessionState(SessionState ss) {
    this->sessionState = ss;
}

void Session::setStartDateTime(QDateTime startDT) {
    this->startDateTime = startDT;
}

void Session::setEndDateTime(QDateTime endDT) {
    this->endDateTime = endDT;
}


void Session::setBaseline(bool baseline_final, int e_id, double b_value) {
    if ((e_id < 0) || (e_id > 21)){ return; }

    if (baseline_final) {
        this->baselines_after[e_id] = b_value;
    } else {
        this->baselines_before[e_id] = b_value;
    }
}

