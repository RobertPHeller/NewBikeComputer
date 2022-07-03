// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Sep 15 17:56:32 2013
//  Last Modified : <220703.1735>
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
#include <stdio.h>
#include "SerialCLI.h"
#include <ctype.h>
#include "PersistentTripDatabase.h"
#include "BikeNVS.h"
#include "Wheelsensor.h"

static const char *HelpText[] = {
    "New Bike Computer 1.0",
    "",
    "Commands:",
    "",
    "S offset",
    "  Set TZ offset",
    "U",
    "  Upload tripfile",
    "C",
    "  Clear tripfile",
    "Z",
    "  Zero NVS (miles, tzoffset)", 
    "H",
    "  Print this help",
    "",
    NULL
};

void ProcessSerialCLI(PersistentTripDatabase *PTD,BikeNVS *NVS,
                      Wheelsensor *wheel)
{
    char buffer[256];
    int len;
    
    if (Serial.available() > 0) {
        len = Serial.readBytesUntil('\r',buffer,sizeof(buffer)-1);
        if (len == 0) return;
        buffer[len] = '\0';
        switch ((Commands) (toupper(buffer[0]))) {
        case SET:
            {
                break;
            }
        case CLEAR:
            {
                break;
            }
        case UPLOAD:
            {
                break;
            }
        case ZERO:
            {
                break;
            }
        case HELP:
            {
                int i, n = sizeof(HelpText) / sizeof(HelpText[0]);
                for (i = 0; i < n && HelpText[i]; i++) {
                    Serial.println(HelpText[i]);
                }
                break;
            }
        default:
            Serial.println("");
            Serial.println("Unknown Command.");
        }
        Serial.print(">>");
        Serial.flush();
    }
}               
            
