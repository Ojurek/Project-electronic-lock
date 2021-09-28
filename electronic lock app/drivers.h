#ifndef DRIVERS_H
#define DRIVERS_H

#include "define.h"
#include <QObject>
#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <QString>
#include <QTime>

#ifndef SIMULATION_PC
    #include <wiringPi.h>
#else
    #include <QDebug>
#endif //SIMULATION_PC

class Drivers: public QObject
{
    Q_OBJECT
private:
#ifdef SIMULATION_PC
    QString m_doorState;
#endif //SIMULATION_PC

public:
    explicit Drivers( QObject *parent = nullptr );
    ~Drivers();

    void openLock();
    void closeLock();
    QString getDoorStatus();
};

#endif // DRIVERS_H
