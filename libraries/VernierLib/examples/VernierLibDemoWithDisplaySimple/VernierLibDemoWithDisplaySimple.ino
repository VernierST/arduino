#include "VernierLib.h" 
VernierLib Vernier;
float sensorReading;
///
#include <SoftwareSerial.h> //library used in printing to display
SoftwareSerial mySerial(3,9); //for display, pin 9 = TX, pin 3 = RX (unused)
///

void setup()
{
  Serial.begin(9600);
  Vernier.autoID();// this is the routine to do the autoID
  mySerial.begin(9600); // for sending characters to display
  delay(500); // wait for display to boot up
  mySerial.write(124); // adjust backlight brightness of display
  mySerial.write(150); //max=157, 150=73%, 130=40%,128=off
  delay(500); // wait for display to start
  mySerial.write(254);
  mySerial.write(1); //clear display
  mySerial.print(Vernier.shortName()); // display Short Name
  mySerial.write(254); // cursor to middle of bottom line
  mySerial.write(200);
  mySerial.print(Vernier.sensorUnits()); // display Units
}
void loop()
{
  sensorReading =Vernier.readSensor();
  Serial.print(sensorReading);
  Serial.print(" ");
  Serial.println(Vernier.sensorUnits());
  mySerial.write(254); // command character
  mySerial.write(192); // move to line 2, position 0,
  mySerial.print(sensorReading);
  delay(1000);// a second
  ///
} 

 
