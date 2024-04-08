#ifndef HEADSET_H
#define HEADSET_H

#include <QObject>
#include "ExternalObj.h"
#include "Electrode.h"

class Headset : public QObject
{
    Q_OBJECT
public:
    // Constructor(s)
    Headset();

    // Destructor(s)
    ~Headset();

    //Getter(s)
    Electrode* getElectrode(int e_id);
    Electrode* getElectrodes();

    //Setter(s)
    void setElectrode(int e_id, ConnectionState cs);

    //Member Variable(s)
private:
    ConnectionState connectionState;
//    ConnectionState *electrodes;
    Electrode *electrodes;

signals:
    void connectionStateChanged(ConnectionState cs);

public slots:
    bool hasDisconnection();
};

#endif // HEADSET_H
