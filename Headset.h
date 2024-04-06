#ifndef HEADSET_H
#define HEADSET_H

#include <QObject>
#include "ExternalObj.h"

class Headset : public QObject
{
    Q_OBJECT
public:
    // Constructor(s)
    Headset();

    // Destructor(s)
    ~Headset();

    //Getter(s)
    ConnectionState getElectrode(int e_id);
    ConnectionState* getElectrodes();

    //Setter(s)
    void setElectrode(int e_id, ConnectionState cs);

    //Member Variable(s)
private:
    ConnectionState connectionState;
    ConnectionState *electrodes;

signals:
    void connectionStateChanged(ConnectionState cs);

public slots:
    bool hasDisconnection();
};

#endif // HEADSET_H
