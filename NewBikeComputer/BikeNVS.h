// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Jul 3 16:45:26 2022
//  Last Modified : <220703.1737>
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

#ifndef __BIKENVS_H
#define __BIKENVS_H

#include <EEPROM.h>

class BikeNVS : public EEPROMClass
{
public:
    BikeNVS() : EEPROMClass("nvs")
    {
    }
    ~BikeNVS()
    {
        end();
    }
              
    void begin()
    {
        EEPROMClass::begin(sizeof(float)+sizeof(int));
    }
    void Zero() {
        SetMiles(0.0);
        SetTZOffset(0);
    }
    float Miles() {
        float miles;
        get(0,miles);
        return miles;
    }
    int TZOffset() {
        int tzoffset;
        get(sizeof(float),tzoffset);
        return tzoffset;
    }
    void SetMiles(float miles)
    {
        if (Miles() != miles)
        {
            put(0,miles);
        }
    }
    void SetTZOffset(int tzoffset)
    {
        if (TZOffset() != tzoffset)
        {
            put(sizeof(float),tzoffset);
        }
    }
};

#endif // __BIKENVS_H

