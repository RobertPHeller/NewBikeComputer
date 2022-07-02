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
//  Last Modified : <220702.0937>
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
//#include <SoftwareSerial.h>
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

//SoftwareSerial mySerial(RX,TX);
Adafruit_GPS GPS(&Serial0);


ModeButton b1(MODE);
Button b2(BUTTON2);
Button b3(BUTTON3);
Wheelsensor WheelSensor(WHEEL);

// Use dedicated hardware SPI pins
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const float VScale = (3.3*14.0)/4095.0;

void setup() {
    // put your setup code here, to run once:
    // Initialize serial first (so we can debug stuff)
    Serial.begin(115200);
    Serial.println("BikeComputer 0.0");
    Serial.print(">>");
    Serial.flush();
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    GPS.begin(9600);
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
}
                
void loop() {
    float VBatt = analogRead(BATTERY) * VScale;
    int   VBint = (int)VBatt;
    int   VBfract = (int)((VBatt - VBint)*10);
    if (Serial.available() > 0) {
        ProcessSerialCLI();
    }
    // put your main code here, to run repeatedly:
    if (WheelSensor.CheckState()) {
        int speed = WheelSensor.CurrentSpeed();
    }
}    
