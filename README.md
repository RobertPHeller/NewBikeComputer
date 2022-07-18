# New Bike Computer.

Based on the Adafruit ESP32-S3 TFT Feather, with an Adafruit Ultimate GPS
FeatherWing. Displays date/time, Lat/Lon, Heading, Alt., MPH/Miles, Battery
voltage. Saves trip info on SPIFFS, which is uploadable via USB.

This is an update to my older Ardunio Uno based unit. Improvements include the
GPS for location, time, heading, and altitude.  Using a 32-bit MCU and using 
FLASH SPIFFS to store trip information, as a CSV file, which can be uploaded
to a host computer via USB.  This version will be better weatherproofed (the 
old one's LCD display has been partially water damaged).

Files in this project:

NewBikeComputer.fzz -- this is the Fritzing for the circuit breadboard.  The 
circuit is pretty simple and straightforward.  If you don't have Fritzing, 
there is an image (NewBikeComputer_bb.png) of the breadboard and a PDF 
(NewBikeComputer_Schematic.pdf) of the schematic.  Parts placement is not 
critical.  Oh, the 100K resistor should be 107K: it and the 33K are 1% 
resistors that form a voltage divider to measure the battery voltage.  My 
trike has a 12V gell cell battery for the lights (headlights, marker lights, 
brake lights, and turn turn signals), that I am also using to power the bike 
computer.  If you are doing something else, feel free to modify things to 
suit.  If you use a LiPo, you can use the built-in JST connector and I believe 
there is a built in battery monitor circuit, so you should change that part of 
the code and leave off the 12V power supply, DC power jack, and the voltage 
divider resistors.

NewBikeComputer_bb.png -- Breadboard image

NewBikeComputer_Schematic.pdf -- Schematic

NewBikeComputer/ -- this sub-directorty contains the Arduino code:

- NewBikeComputer.ino This is the main program
- BikeNVS.h This is the NVS (EEProm) handling
- Button.cpp Button class
- Button.h   Button class
- Modes.cpp  Mode Button class
- Modes.h    Mode Button class
- PersistentTripDatabase.cpp Trip file handling
- PersistentTripDatabase.h   Trip file handling
- SerialCLI.cpp Serial CLI handling
- SerialCLI.h   Serial CLI handling
- Wheelsensor.cpp Wheel sensor
- Wheelsensor.h   Wheel sensor

Libraries needed: the Adafruit ST7735 and ST7789 library, the Adafruit GPS
library, the Adafruit BusIO library, and the Adafruit GFX (the last two will 
be pulled in by the first two).  If you use a LiPo battery and want to use the 
internal battery monitor, you will need to modify the code and add the  
Adafruit LC709203F library as well.

BikeHost/ -- this sub-directorty contains the host code.  This is a Tcl/Tk 
program that runs on a host computer to allow for some maintainance tasks and 
to upload the trip file.  It is set up to build under Linux for a Linux 
target. Note: the Arduino Serial Monitor could also be used for these 
functions. 


