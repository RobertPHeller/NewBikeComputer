// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Sep 15 11:29:23 2013
//  Last Modified : <220703.1738>
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

#ifndef __WHEELSENSOR_H
#define __WHEELSENSOR_H

class Wheelsensor {
public:
    Wheelsensor(uint8_t pin) : pin_(pin) { }
    bool CheckState();
    int CurrentSpeed() {return curspeed;}
    float Miles() {return totalmiles;}
    void init(float miles = 0.0);
    void ZeroMiles() {totalmiles=0.0;}
private:
    volatile static int rotationcount;
    static void count_rotation(void);
    long int lasttime;
    float totalmiles;
    int   curspeed;
    int speed(long int cycletime);
    long lastDebounceTime;
    long debounceDelay;
    int lastButtonState;
    uint8_t pin_;
};

#endif // __WHEELSENSOR_H

