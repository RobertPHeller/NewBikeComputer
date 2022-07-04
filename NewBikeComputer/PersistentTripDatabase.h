// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Jul 3 15:06:18 2022
//  Last Modified : <220704.1122>
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

#ifndef __PERSISTENTTRIPDATABASE_H
#define __PERSISTENTTRIPDATABASE_H

#include <Arduino.h>
#include <stdio.h>
#include <SPI.h>
#include <Adafruit_GPS.h>
#include <HardwareSerial.h>
#include <FS.h>
#include <SPIFFS.h>

class PersistentTripDatabase
{
private:
    static constexpr char tripFORMAT[] = "%2d,%2d,%4d,%02d,%02d,%02d,%3d,%4.1f,%c,%3d,%4.1f,%c,%5.1f,%9.1f,%10.2f";
    static constexpr char timeHeaderFORMAT[] = "%2d/%2d/%4d %02d:%02d:%02d";
    static constexpr char locFORMAT[] = "Loc: %3d %4.1f%c, %3d %4.1f%c";
    static constexpr char headingFORMAT[] = "Heading: %5.1f Alt: %9.1f";
public:
    PersistentTripDatabase(Adafruit_GPS *gps) : gps_(gps)
    {
    }
    void begin(float miles,int tzoffset = 0);
    void UpdateTripRecord(float miles,bool newtrip = false);
    const char *TimeHeader();
    const char *Location();
    const char *Heading();
    void SetTZOffset(int tzoffset)
    {
        tzoffset_ = tzoffset;
    }
    int TZOffset() const {return tzoffset_;}
    void ZeroFile(float miles);
    void UploadFile();
private:
    int DaysInMonth(int month,int year)
    {
        int result = 0;
        switch (month)
        {
        case 1: //  Jan
        case 3: //  Mar
        case 5: //  May
        case 7: //  Jul
        case 8: //  Aug
        case 10: // Oct
        case 12: // Dec
            { result = 31; break; }
        case 4: //  Apr
        case 6: //  Jun
        case 9: //  Sep
        case 11: // Nov
            { result = 30; break; }
        case 2: //  Feb
            if ((year % 4) != 0) {
                result = 28;
            } else if ((year % 400) != 0) {
                result = 28;
            } else {
                result = 29;
            }
        }
        return result;
    }
    Adafruit_GPS *gps_;
    int tzoffset_;
    int hours_;
    int minutes_;
    int seconds_;
    byte month_;
    byte date_;
    short year_;
    struct {
        int degrees;
        float minutes;
        char direction;
    } latitude_, longitude_;
    float angle_;
    float alt_;
    char buffer_[256];
    fs::File tripfile_;
    size_t lastline_;
    bool mounted_;
};


#endif // __PERSISTENTTRIPDATABASE_H

