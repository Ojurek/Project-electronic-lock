#include "drivers.h"

#ifndef SIMULATION_PC
Drivers::Drivers()
{
    wiringPiSetupGpio();

    pinMode(PIN_OUT,OUTPUT);
    digitalWrite(PIN_OUT,1);

    pinMode(PIN_INP,INPUT);
    pullUpDnControl(PIN_INP,PUD_DOWN);
}

Drivers::~Drivers()
{

}

void Drivers::openLock()
{
    digitalWrite(PIN_OUT,0);
    delay(3000);
    digitalWrite(PIN_OUT,1);
}

QString Drivers::getDoorStatus()
{
    if ( digitalRead(PIN_INP) == 0 )
    {
        return QString( DOOR_OPENED );
    }

    return QString( DOOR_CLOSED );
}
#else
Drivers::Drivers()
{
    m_doorState = QString( DOOR_OPENED );
}

Drivers::~Drivers()
{
}

void Drivers::openLock()
{
    qDebug()<<"Drivers inform: Lock is open!!!!!!";
}

QString Drivers::getDoorStatus()
{
    //for debug toggle OPEN - CLOSE status
    if ( m_doorState == QString(DOOR_OPENED) )
    {
        m_doorState = QString(DOOR_CLOSED);
    }
    else
    {
        m_doorState = QString(DOOR_OPENED);
    }

    return m_doorState;
}
#endif //SIMULATION_PC
