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
//  Last Modified : <221017.1655>
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
#include <Fonts/FreeMono12pt7b.h> // 13x14 (WxH) Spacing: 14 NL: 24 -- 17c x 5l
#include <Fonts/FreeMono24pt7b.h> // 26x26 (WxH) Spacing: 28 NL: 47 -- 8c  x 2l
#include <HardwareSerial.h>
#include <FS.h>
#include <SPIFFS.h>
#include "Wheelsensor.h"
#include "Button.h"
#include "Modes.h"
#include "SerialCLI.h"
#include "PersistentTripDatabase.h"
#include "BikeNVS.h"

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

PersistentTripDatabase tripdatabase(&GPS);

constexpr float VScale = 14.0/4095.0;

static     char buffer[40];

BikeNVS NVS;

static int TickCounter = 0;

void setup() {
    // put your setup code here, to run once:
    // Initialize serial first (so we can debug stuff)
    Serial.begin(115200);
    NVS.begin();
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    Serial1.begin(9600,SERIAL_8N1,RX,TX);
    delay(10);
    //GPS.begin(9600);
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // Set the update rate
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
    GPS.sendCommand(PGCMD_ANTENNA);
    delay(1000);
    GPS.println(PMTK_Q_RELEASE);
    WheelSensor.init(NVS.Miles());
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
    tripdatabase.begin(WheelSensor.Miles(),NVS.TZOffset());
    Serial.println("BikeComputer 0.0");
    Serial.print(">>");
    Serial.flush();
    TickCounter = 0;
}

void loop() {
    // put your main code here, to run repeatedly:
    // Get battery Voltage:
    float VBatt = analogRead(BATTERY) * VScale;
    int   VBint = (int)VBatt;
    int   VBfract = (int)((VBatt - VBint)*10);
    // Process any Serial CLI requests.
    if (Serial.available() > 0) {
        ProcessSerialCLI(&tripdatabase,&NVS,&WheelSensor);
    }
    // Read Wheel sensor and compute MPH
    int speed = 0;
    if (WheelSensor.CheckState()) {
        speed = WheelSensor.CurrentSpeed();
    }
    DisplayMode mode = TimeAndSpeed;
    //DisplayMode mode = b1.CheckModeButton();
    bool newtrip = false;
    // if (b2.Pressed()) newtrip = true;
    bool final   = false;
    // if (b3.Pressed()) final = true;
    tripdatabase.UpdateTripRecord(WheelSensor.Miles(),newtrip,final);
    canvas.fillScreen(ST77XX_BLACK);
    canvas.setFont(&FreeMono12pt7b);
    canvas.setCursor(0, 18);
    canvas.setTextColor(ST77XX_RED);
    canvas.println(tripdatabase.TimeHeader());
    canvas.setTextColor(ST77XX_YELLOW);
    canvas.println(tripdatabase.Location());
    canvas.println(tripdatabase.Heading());
    canvas.setTextColor(ST77XX_WHITE);
    canvas.setFont(&FreeMono24pt7b);
    canvas.setCursor(0, 104);
    //Serial.print("*** loop(): mode = "); Serial.println(mode);
    switch (mode)
    {
    case TimeAndSpeed:
        snprintf(buffer,sizeof(buffer),"%2d %4.1fV",speed,VBatt);
        break;
    case TimeAndDistance:
        snprintf(buffer,sizeof(buffer),"%6dM",WheelSensor.Miles());
        break;
    }
    canvas.println(buffer);
    //Serial.println(buffer);
    tft.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
    TickCounter++;
    if ((TickCounter % 1000) == 0)
    {
        NVS.SetMiles(WheelSensor.Miles());
        NVS.commit();
    }
    delay(100);
}    
