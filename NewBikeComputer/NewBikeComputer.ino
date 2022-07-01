// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Jun 30 15:03:26 2022
//  Last Modified : <220701.1254>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2022  Robert Heller D/B/A Deepwoods Software
//			51 Locke Hill Road
//			Wendell, MA 01379-9728
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// 
//
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include <Arduino.h>
#include <stdio.h>
#include "Wheelsensor.h"
#include "Button.h"
#include "Modes.h"
//#include "SerialCLI.h"

const uint8_t BUTTON2 = 9;
const uint8_t BUTTON3 = 6;
const uint8_t MODE  = 5;
const uint8_t WHEEL = 10;
const uint8_t BATTERY = A5;

ModeButton b1(MODE);
Button b2(BUTTON2);
Button b3(BUTTON3);
Wheelsensor WheelSensor(WHEEL);

const float VScale = (3.3*14.0)/4095.0;

void setup() {
    // put your setup code here, to run once:
    WheelSensor.init();
    //analogReference(DEFAULT);
}
                
void loop() {
    float VBatt = analogRead(BATTERY) * VScale;
    int   VBint = (int)VBatt;
    int   VBfract = (int)((VBatt - VBint)*10);
    
    // put your main code here, to run repeatedly:
    if (WheelSensor.CheckState()) {
        int speed = WheelSensor.CurrentSpeed();
    }
}    
