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
//  Last Modified : <220702.1521>
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
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Adafruit_GPS.h>
#include <Fonts/FreeSans12pt7b.h>
#include <HardwareSerial.h>
#include "Wheelsensor.h"
#include "Button.h"
#include "Modes.h"
#include "SerialCLI.h"

const uint8_t BUTTON2 = 9;
const uint8_t BUTTON3 = 6;
const uint8_t MODE  = 5;
const uint8_t WHEEL = 10;
const uint8_t BATTERY = A5;

Adafruit_GPS GPS(&Serial1);


ModeButton b1(MODE);
Button b2(BUTTON2);
Button b3(BUTTON3);
Wheelsensor WheelSensor(WHEEL);

// Use dedicated hardware SPI pins
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
GFXcanvas16 canvas(240, 135);

const float VScale = (3.3*14.0)/4095.0;

void setup() {
    // put your setup code here, to run once:
    // Initialize serial first (so we can debug stuff)
    Serial.begin(115200);
    Serial.println("BikeComputer 0.0");
    Serial.print(">>");
    Serial.flush();
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    Serial1.begin(9600,SERIAL_8N1,RX,TX);
    delay(10);
    //GPS.begin(9600);
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // Set the update rate
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
    delay(1000);
    Serial0.println(PMTK_Q_RELEASE);
    
    WheelSensor.init();
    //analogReference(DEFAULT);
    // turn on backlite
    pinMode(TFT_BACKLITE, OUTPUT);
    digitalWrite(TFT_BACKLITE, HIGH);
    
    // turn on the TFT / I2C power supply
    pinMode(TFT_I2C_POWER, OUTPUT);
    digitalWrite(TFT_I2C_POWER, HIGH);
    delay(10);
    
    // initialize TFT
    tft.init(135, 240); // Init ST7789 240x135
    tft.setRotation(3);
    tft.fillScreen(ST77XX_BLACK);
    canvas.setFont(&FreeSans12pt7b);
    canvas.setTextColor(ST77XX_WHITE);
}

static int hours = 0;
static int minutes = 0;
static int seconds = 0;
static byte month = 0;
static byte date = 0;
static short year = 0;
static struct {
    int degrees;
    float minutes;
    char direction;
} latitude, longitude;
static float angle = 0.0;
static float alt   = 0.0;

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

void loop() {
    // put your main code here, to run repeatedly:
    // Get battery Voltage:
    float VBatt = analogRead(BATTERY) * VScale;
    int   VBint = (int)VBatt;
    int   VBfract = (int)((VBatt - VBint)*10);
    // Process any Serial CLI requests.
    if (Serial.available() > 0) {
        ProcessSerialCLI();
    }
    // Read Wheel sensor and compute MPH
    int speed = 0;
    if (WheelSensor.CheckState()) {
        speed = WheelSensor.CurrentSpeed();
    }
    
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if a sentence is received, we can check the checksum, parse it...
    if (GPS.newNMEAreceived()) {
        // a tricky thing here is if we print the NMEA sentence, or data
        // we end up not listening and catching other sentences!
        // so be very wary if using OUTPUT_ALLDATA and trying to print out data
        //Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
        if (GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
        {
            hours = GPS.hour + TZOffset;
            minutes = GPS.minute;
            seconds = GPS.seconds;
            month = GPS.month;
            date = GPS.day;
            year = 2000+GPS.year;
            if (hours < 0)
            {
                hours += 24;
                date -= 1;
                if (date < 1)
                {
                    month -= 1;
                    if (month < 1)
                    {
                        year -= 1;
                        month = 12;
                    }
                    date = DaysInMonth(month,year);
                }
            } else if (hours >= 24)
            {
                hours -= 24;
                date += 1;
                if (date > DaysInMonth(month,year))
                {
                    month += 1;
                    date = 1;
                    if (month > 12)
                    {
                        year += 1;
                        month = 1;
                    }
                }
            }
                              
            if (GPS.fix)
            {
                nmea_float_t lat = GPS.latitude;
                latitude.degrees = lat / 100;
                latitude.minutes = lat - (latitude.degrees*100);
                latitude.direction = GPS.lat;
                nmea_float_t lon = GPS.longitude;
                longitude.degrees = lon / 100;
                longitude.minutes = lon - (longitude.degrees*100);
                longitude.direction = GPS.lon;
                angle = GPS.angle;
                alt   = GPS.altitude;
            }
                
        }
    }
    char buffer[40];
    canvas.fillScreen(ST77XX_BLACK);
    canvas.setCursor(0, 25);
    canvas.setTextColor(ST77XX_RED);
    snprintf(buffer,sizeof(buffer),"%2d/%2d/%4d %02d:%02d:%02d",
             month,date,hours,minutes,seconds);
    canvas.println(buffer);
    snprintf(buffer,sizeof(buffer),"Loc: %3d %4.1f%c, %3d %4.1f%c",
             latitude.degrees,latitude.minutes,latitude.direction,
             longitude.degrees,longitude.minutes,longitude.direction);
    canvas.setTextColor(ST77XX_YELLOW);
    canvas.println(buffer);
    snprintf(buffer,sizeof(buffer),"Heading: %5.1f Alt: %9.1f",angle,alt);
    canvas.println(buffer);
    snprintf(buffer,sizeof(buffer),"%2d MPH    %4.1VBatt",speed,VBatt);
    canvas.setTextColor(ST77XX_WHITE);
    canvas.println(buffer);
    tft.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
    delay(100);
}    
