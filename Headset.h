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

private:
    QHash<int, ConnectionState> electrodes;

signals:

public slots:
};

#endif // HEADSET_H
