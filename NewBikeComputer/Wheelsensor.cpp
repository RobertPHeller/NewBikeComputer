// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Sep 15 11:35:09 2013
//  Last Modified : <220701.1226>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013 Deepwoods Software.
// 
//  All Rights Reserved.
// 
// This  document  may  not, in  whole  or in  part, be  copied,  photocopied,
// reproduced,  translated,  or  reduced to any  electronic  medium or machine
// readable form without prior written consent from Deepwoods Software.
//
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include <Arduino.h>
#include "Wheelsensor.h"

/* Assumes 20 inch wheels */
#define FEETPERCYCLE ((PI * 20) / 12.0)
/* How many feet in a mile */
#define FEETPERMILE 5280

volatile int Wheelsensor::rotationcount = 0;

void Wheelsensor::init()
{
    lasttime = 0;
    totalmiles = 0;
    curspeed = 0;
    rotationcount = 0;
    attachInterrupt(digitalPinToInterrupt(pin_), count_rotation, RISING);
}

void Wheelsensor::count_rotation(void)
{
    rotationcount++;
}

bool Wheelsensor::CheckState()
{
    bool result = false;
    //delay(50);
    noInterrupts();
    long int cycletime = millis() - lasttime;
    int rc = rotationcount; rotationcount = 0;
    interrupts();
    if (rc > 0) {
        //Serial.print("*** Wheelsensor::CheckState(): lasttime = ");
        //Serial.print(lasttime);
        //Serial.print(", time is ");
        //Serial.println(millis());            
        //Serial.print("*** Wheelsensor::CheckState(): cycletime = ");
        //Serial.println(cycletime);
        // Time for next cycle
        lasttime = millis();
        // Compute speed
        curspeed = speed(cycletime / rc);
        //Serial.print("*** Wheelsensor::CheckState(): curspeed is ");
        //Serial.println(curspeed);
        // Accumulate distance
        totalmiles += rc * (FEETPERCYCLE / float(FEETPERMILE));
        //Serial.print("*** Wheelsensor::CheckState(): totalmiles = ");
        //Serial.println(totalmiles);
        result = true;
    }
    else {
        if (speed(cycletime) < 1) {
            curspeed = 0;
            result =  true;
        }
    }
    return result;
}

/** Compute speed in MPH, given a number of ms for a wheel
 *  * rotation,
 *  */
int Wheelsensor::speed(long int cycletime) {
    //Serial.print("*** Wheelsensor::speed(");
    //Serial.print(cycletime);
    //Serial.println(")");
    /* Convert cycle time to feet per second */
    double feetpersecond = (1000.0 / float(cycletime))*FEETPERCYCLE;
    //Serial.print("*** Wheelsensor::speed(): feetpersecond = ");
    //Serial.println(feetpersecond);
    /* Convert feet per second to feet per hour */
    double feetperhour   = feetpersecond * 3600;
    //Serial.print("*** Wheelsensor::speed(): feetperhour = ");
    //Serial.println(feetperhour);
    /* Convert feet per hour to whole miles per hour */
    int    milesperhour  = int((feetperhour / FEETPERMILE)); /* not rounding up */
    //Serial.print("*** Wheelsensor::speed(): milesperhour = ");
    //Serial.println(milesperhour);
    /* Return the MPH */
    return milesperhour;
}


