#ifndef DRIVERS_H
#define DRIVERS_H

#include "define.h"
#include <stdio.h>
#include <stdlib.h>
#include <QString>

#ifndef SIMULATION_PC
    #include <wiringPi.h>
#else
    #include <QDebug>
#endif //SIMULATION_PC

class Drivers
{
private:
#ifdef SIMULATION_PC
    QString m_doorState;
#endif //SIMULATION_PC

public:
    Drivers();    
    ~Drivers();

    void openLock();
    QString getDoorStatus();
};

#endif // DRIVERS_H
