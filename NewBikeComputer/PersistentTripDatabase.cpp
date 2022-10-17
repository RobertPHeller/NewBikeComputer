// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Jul 3 15:27:28 2022
//  Last Modified : <221017.1748>
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
#include <SPI.h>
#include <Adafruit_GPS.h>
#include <HardwareSerial.h>
#include <FS.h>
#include <SPIFFS.h>
#include "PersistentTripDatabase.h"

void PersistentTripDatabase::begin(float miles,int tzoffset)
{
    tzoffset_ = tzoffset;
    mounted_ = SPIFFS.begin(true);
    if (mounted_)
    {
        Serial.println("/spiffs Mounted");
    }
    else
    {
        Serial.println("/spiffs not Mounted");
    }
    // read data from the GPS
    char c = gps_->read();
    while (c && c != '\n')
    {
        c = gps_->read();
    }
    //Serial.print("*** PersistentTripDatabase::begin(): gps_->read() returns ");
    //Serial.println(c);
    // if a sentence is received, we can check the checksum, parse it...
    unsigned MAX_LOOP = 100;
    //while (!gps_->newNMEAreceived()&&MAX_LOOP-->0) {
    //    //Serial.println("*** PersistentTripDatabase::begin(): newNMEAreceived() loop");
    //    delay(100);
    //}
    if (gps_->newNMEAreceived())
    {
        Serial.println(gps_->lastNMEA());
        if (!gps_->parse(gps_->lastNMEA()))
        {
            return;
        }
        hours_ = gps_->hour + tzoffset_;
        minutes_ = gps_->minute;
        seconds_ = gps_->seconds;
        month_ = gps_->month;
        date_ = gps_->day;
        year_ = 2000+gps_->year;
        if (hours_ < 0)
        {
            hours_ += 24;
            date_ -= 1;
            if (date_ < 1)
            {
                month_ -= 1;
                if (month_ < 1)
                {
                    year_ -= 1;
                    month_ = 12;
                }
                date_ = DaysInMonth(month_,year_);
            }
        } else if (hours_ >= 24)
        {
            hours_ -= 24;
            date_ += 1;
            if (date_ > DaysInMonth(month_,year_))
            {
                month_ += 1;
                date_ = 1;
                if (month_ > 12)
                {
                    year_ += 1;
                    month_ = 1;
                }
            }
        }
        
        if (gps_->fix)
        {
            nmea_float_t lat = gps_->latitude;
            latitude_.degrees = lat / 100;
            latitude_.minutes = lat - (latitude_.degrees*100);
            latitude_.direction = gps_->lat;
            nmea_float_t lon = gps_->longitude;
            longitude_.degrees = lon / 100;
            longitude_.minutes = lon - (longitude_.degrees*100);
            longitude_.direction = gps_->lon;
            angle_ = gps_->angle;
            alt_   = gps_->altitude;
        }
    }
    if (mounted_)
    {
        tripfile_ = SPIFFS.open("/spiffs/trips.list","a+",true);
        snprintf(buffer_,sizeof(buffer_),tripFORMAT,
                 month_,date_,hours_,minutes_,seconds_,latitude_.degrees,
                 latitude_.minutes,latitude_.direction,longitude_.degrees,
                 longitude_.minutes,longitude_.direction,angle_,alt_,
                 miles);
        tripfile_.println(buffer_);
        tripfile_.flush();
        lastline_ = tripfile_.position();
    }
}

void PersistentTripDatabase::UpdateTripRecord(float miles,bool newtrip,
                                              bool final)
{
    // read data from the GPS
    char c = gps_->read();
    while (c && c != '\n')
    {
        c = gps_->read();
    }
    // if a sentence is received, we can check the checksum, parse it...
    if (gps_->newNMEAreceived()) {
        Serial.println(gps_->lastNMEA());
        if (!gps_->parse(gps_->lastNMEA()))
        {
            return;
        }
        hours_ = gps_->hour + tzoffset_;
        minutes_ = gps_->minute;
        seconds_ = gps_->seconds;
        month_ = gps_->month;
        date_ = gps_->day;
        year_ = 2000+gps_->year;
        if (hours_ < 0)
        {
            hours_ += 24;
            date_ -= 1;
            if (date_ < 1)
            {
                month_ -= 1;
                if (month_ < 1)
                {
                    year_ -= 1;
                    month_ = 12;
                }
                date_ = DaysInMonth(month_,year_);
            }
        } else if (hours_ >= 24)
        {
            hours_ -= 24;
            date_ += 1;
            if (date_ > DaysInMonth(month_,year_))
            {
                month_ += 1;
                date_ = 1;
                if (month_ > 12)
                {
                    year_ += 1;
                    month_ = 1;
                }
            }
        }
        
        if (gps_->fix)
        {
            nmea_float_t lat = gps_->latitude;
            latitude_.degrees = lat / 100;
            latitude_.minutes = lat - (latitude_.degrees*100);
            latitude_.direction = gps_->lat;
            nmea_float_t lon = gps_->longitude;
            longitude_.degrees = lon / 100;
            longitude_.minutes = lon - (longitude_.degrees*100);
            longitude_.direction = gps_->lon;
            angle_ = gps_->angle;
            alt_   = gps_->altitude;
        }
    }
    if (!final) return;
    if (!mounted_) return;
    if (newtrip)
    {
        tripfile_.seek(0,SeekEnd);
    }
    else
    {
        auto rc = tripfile_.seek(lastline_,SeekSet);
        //Serial.print("*** PersistentTripDatabase::UpdateTripRecord(): seek returns ");
        //Serial.println(rc);
        //Serial.print("*** PersistentTripDatabase::UpdateTripRecord(): position is now ");
        //Serial.println(tripfile_.position());
    }
    snprintf(buffer_,sizeof(buffer_),tripFORMAT,
             month_,date_,hours_,minutes_,seconds_,latitude_.degrees,
             latitude_.minutes,latitude_.direction,longitude_.degrees,
             longitude_.minutes,longitude_.direction,angle_,alt_,
             miles);
    tripfile_.println(buffer_);
    tripfile_.flush();
    if (newtrip) lastline_ = tripfile_.position();
}

const char *PersistentTripDatabase::TimeHeader()
{
    snprintf(buffer_,sizeof(buffer_),timeHeaderFORMAT,month_,date_,hours_,
             minutes_,seconds_);
    return buffer_;
}

const char *PersistentTripDatabase::Location()
{
    snprintf(buffer_,sizeof(buffer_),locFORMAT,latitude_.degrees,
             latitude_.minutes,latitude_.direction,longitude_.degrees,
             longitude_.minutes,longitude_.direction);
    return buffer_;
}

const char *PersistentTripDatabase::Heading()
{
    snprintf(buffer_,sizeof(buffer_),headingFORMAT,angle_,alt_);
    return buffer_;
}

void PersistentTripDatabase::ZeroFile(float miles)
{
    if (mounted_)
    {
        tripfile_.close();
        SPIFFS.remove("/spiffs/trips.list");
        tripfile_ = SPIFFS.open("/spiffs/trips.list",FILE_APPEND,true);
        UpdateTripRecord(miles,true);
    }
}

void PersistentTripDatabase::UploadFile()
{
    if (mounted_)
    {
        uint8_t buffer[1024];
        size_t bytesRead, remaining, contentLength;
        tripfile_.close();
        tripfile_ = SPIFFS.open("/spiffs/trips.list",FILE_READ,false);
        contentLength = tripfile_.size();
        Serial.print("Length: ");Serial.println(contentLength);
        remaining = contentLength;
        while (remaining > 0) {
            bytesRead = tripfile_.read(buffer,sizeof(buffer));
            Serial.write(buffer,bytesRead);
            remaining -= bytesRead;
        }
        tripfile_.close();
        tripfile_ = SPIFFS.open("/spiffs/trips.list",FILE_APPEND,false);
    }
}


constexpr char PersistentTripDatabase::tripFORMAT[];
constexpr char PersistentTripDatabase::timeHeaderFORMAT[];
constexpr char PersistentTripDatabase::locFORMAT[];
constexpr char PersistentTripDatabase::headingFORMAT[];
