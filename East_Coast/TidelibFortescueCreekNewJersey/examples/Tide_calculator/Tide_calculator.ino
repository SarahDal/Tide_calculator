/*  Tide_calculator.ino 
 Copyright (c) 2019 Luke Miller
This code calculates the current tide height for the 
pre-programmed site. It requires a real time clock
(DS1307 or DS3231 chips) to generate a time for the calculation.
The site is set by the name of the included library (see line 44 below)

Written under version 1.6.4 of the Arduino IDE.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or 
(at your option) any later version.

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see http://www.gnu.org/licenses/

 The harmonic constituents used here were originally derived from 
 the Center for Operational Oceanic Products and Services (CO-OPS),
 National Ocean Service (NOS), National Oceanic and Atmospheric 
 Administration, U.S.A.
 The data were originally processed by David Flater for use with XTide,
 available at http://www.flaterco.com/xtide/files.html
As with XTide, the predictions generated by this program should 
NOT be used for navigation, and no accuracy or warranty is given
or implied for these tide predictions. The chances are pretty good
that the tide predictions generated here are completely wrong.
It is highly recommended that you verify the output of these predictions
against the relevant NOAA tide predictions online.
*/ 
//--------------------------------------------------------------
//Initial setup
//Header files for talking to real time clock
#include <Wire.h> // Required for RTClib
#include <SPI.h> // Required for RTClib to compile properly
#include <RTClib.h> // From https://github.com/millerlp/RTClib
// Real Time Clock setup
RTC_DS1307 RTC;  // Uncomment when using this chip
// RTC_DS3231 RTC; // Uncomment when using this chip

// Tide calculation library setup.
// Change the library name here to predict for a different site.
#include "TidelibFortescueCreekNewJersey.h"
// Other sites available at http://github.com/millerlp/Tide_calculator
TideCalc myTideCalc; // Create TideCalc object called myTideCalc

int currMinute; // Keep track of current minute value in main loop
float results; // results holds the output from the tide calc. Units = ft.
//*******************************************************************
// Welcome to the setup loop
void setup(void)
{
  Wire.begin();
  RTC.begin();

  // For debugging output to serial monitor
  Serial.begin(57600); // Set baud rate to 57600 in serial monitor
  //*************************************
  DateTime now = RTC.now(); // Get current time from clock
  currMinute = now.minute(); // Store current minute value
  printTime(now);  // Call printTime function to print date/time to serial
  Serial.println("Calculating tides for: ");
  Serial.print(myTideCalc.returnStationID());
  Serial.print(" ");
  Serial.println(myTideCalc.returnStationIDnumber());

  // Calculate new tide height based on current time
  results = myTideCalc.currentTide(now);

  //*****************************************
  // For debugging
  Serial.print("Tide height: ");
  Serial.print(results, 3);
  Serial.println(" ft.");
  Serial.println(); // blank line

  delay(2000);
}  // End of setup loop

//********************************************
// Welcome to the main loop
void loop(void)
{
  // Get current time, store in object "now"
   DateTime now = RTC.now();
  // If it is the start of a new minute, calculate new tide height
  if (now.minute() != currMinute) { 
  // If now.minute doesn't equal currMinute, a new minute has turned
  // over, so it's time to update the tide height. We only want to do
  // this once per minute.
  currMinute = now.minute(); // update currMinute
  Serial.println();
  printTime(now);

  // Calculate new tide height based on current time
  results = myTideCalc.currentTide(now);

  //*****************************************
  // For debugging
  Serial.print("Tide height: ");
  Serial.print(results, 3);
  Serial.println(" ft.");
  Serial.println(); // blank line

  }  // End of if (now.minute() != currMinute) statement 
} // End of main loop 


//*******************************************
// Function for printing the current date/time to the
// serial port in a nicely formatted layout.
void printTime(DateTime now) {
  Serial.print(now.year(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC); 
  Serial.print("/");
  Serial.print(now.day(), DEC); 
  Serial.print("  ");
   Serial.print(now.hour(), DEC); 
  Serial.print(":");
  if (now.minute() < 10) {
    Serial.print("0");
    Serial.print(now.minute());
   }
  else if (now.minute() >= 10) {
    Serial.print(now.minute());
  }
  Serial.print(":");
  if (now.second() < 10) {
    Serial.print("0");
    Serial.println(now.second());
  }
  else if (now.second() >= 10) {
    Serial.println(now.second());
  }
} // End of printTime function
//*************************************
