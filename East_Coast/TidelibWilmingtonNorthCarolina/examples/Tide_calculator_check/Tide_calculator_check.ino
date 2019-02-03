/*  Tide_calculator_check.ino 
 Copyright (c) 2019 Luke Miller
This code calculates the tide height for the 
pre-programmed site based on user input date and time. You should not
need a functional clock chip to run this example, just use the Serial Monitor.

 Just open the Serial Monitor and input a date and time using the 
format: YYYY MM DD HH MM SS and send a newline at the end.
The site is set by the name of the included library (see line 44 below)

Written under version 1.8.8 of the Arduino IDE.

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
// Declare variables to hold user input
long myyear;
long mymonth;
long myday;
long myhour;
long myminute;
long mysec;
DateTime myTime;
// Tide calculation library setup.
// Change the library name here to predict for a different site.
#include "TidelibWilmingtonNorthCarolina.h"
// Other sites available at http://github.com/millerlp/Tide_calculator
TideCalc myTideCalc; // Create TideCalc object called myTideCalc

int currMinute; // Keep track of current minute value in main loop
float results; // results holds the output from the tide calc. Units = ft.
//*******************************************************************
//*******************************************************************
// Welcome to the setup loop
void setup(void)
{
  // For debugging output to serial monitor
  Serial.begin(57600); // Set baud rate to 57600 in serial monitor for slow 8MHz micros
  //*************************************
  Serial.println("Calculating tides for: ");
  Serial.print(myTideCalc.returnStationID());
  Serial.print(" ");
  Serial.println(myTideCalc.returnStationIDnumber());

  Serial.println("Enter date and time in the format:");
  Serial.println("   YYYY MM DD HH MM");
  Serial.println("For example, noon on Jan 1 2019: 2019 1 1 12 00");
  myTime = DateTime(2019,1,1,12,0,0);
  results = myTideCalc.currentTide(myTime);
  Serial.print("Tide height: ");
  Serial.print(results,3);
  Serial.println(" ft.");
  delay(2000);
}  // End of setup loop

//********************************************
// Welcome to the main loop
void loop(void)
{
// When the user has entered a date and time value in the serial
// monitor and hit enter, the following section will execute.
while (Serial.available() > 0) {
  // Expect the year first
  myyear = Serial.parseInt();
  // Expect month next 
  mymonth = Serial.parseInt();
  // Expect day next
  myday = Serial.parseInt();
  // Expect hour next
  myhour = Serial.parseInt();
  // Expect minute next
  myminute = Serial.parseInt();

  // When the enter symbol newline comes along, convert the 
  // values to a DateTime object and set the clock
  if (Serial.read() == '\n'){
    myTime = DateTime(myyear,mymonth,myday,myhour,myminute,0);
    printTime(myTime);
    // Calculate new tide height based on current time
    results = myTideCalc.currentTide(myTime);
    Serial.print("Tide height: ");
    Serial.print(results,3);
    Serial.println(" ft.");

    }
  }  // end of while loop
}  // end of main loop
//*******************************************
// Function for printing the current date/time to the
// serial port in a nicely formatted layout.
void printTime(DateTime now) {
  Serial.print(now.year(), DEC);
  Serial.print("-");
  Serial.print(now.month(), DEC); 
  Serial.print("-");
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
