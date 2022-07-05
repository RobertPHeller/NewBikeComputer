# New Bike Computer.

Based on the Adafruit ESP32-S3 TFT Feather, with an Adafruit Ultimate GPS
FeatherWing. Displays date/time, Lat/Lon, Heading, Alt., MPH/Miles, Battery
voltage. Saves trip info on SPIFFS, which is uploadable via USB.

This is an update to my older Ardunio Uno based unit. Improvements include the
GPS for location, time, heading, and altitude.  Using a 32-bit MCU and using 
FLASH SPIFFS to store trip information, as a CSV file, which can be uploaded
to a host computer via USB.  This version will be better weatherproofed (the 
old one's LCD display has been partially water damaged).

