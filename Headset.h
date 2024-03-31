#ifndef HEADSET_H
#define HEADSET_H

#include <QObject>
#include <QHash>
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
    void setElectrode(int e_id, ConnectionState newCS);

    //Member Variable(s)
private:
    ConnectionState *electrodes;

signals:

public slots:
};

#endif // HEADSET_H
