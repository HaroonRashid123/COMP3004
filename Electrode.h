#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>
#include "ExternalObj.h"

class Electrode : public QObject
{
    Q_OBJECT
public:
    // Constructor(s)
    Electrode(int id);
    Electrode();

    // Destructor(s)
    ~Electrode();

    // Getter(s)
    ConnectionState getConnectionState() ;
    double getFrequency(int f_id);
    double getAmplitude(int a_id);

    // Setter(s)
    void setConnectionState(ConnectionState cs);
    void setFrequency(int f_id, double frequency);
    void setAmplitude(int a_id, double amplitude);

    // Member Variable(s)
private:
    int id;
    ConnectionState connectionState;
    double f1, f2, f3;
    double a1, a2, a3;

signals:
    void connectionStateChanged(ConnectionState cs);

public slots:

};

#endif // ELECTRODE_H
