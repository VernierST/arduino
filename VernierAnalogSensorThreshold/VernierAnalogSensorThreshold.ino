/*
VernierAnalogSensorThreshold (v. 2018.05)
Reads a Vernier analog (BTA) sensor connnected to pin A0 of the Arduino, Analog 1 on
the Vernier Interface Shield. This sketch displays the sensor reading on the Serial Monitor.
As written, the reading will be displayed every half second. Change the variable 
timeBetweenReadings to change the rate.

This has code added to turn on line D13 if the sensor reading exceeds a
threshold.

See www.vernier.com/arduino for more information.

*/
#include "VernierLib.h" 
VernierLib Vernier;
float sensorReading;
int readingNumber;
float threshold =100 ;// this is the limit you are setting (here we are using 100)
////////////////////////////////////////
int timeBetweenReadings = 500;
/////////////////////////////////////////
void setup() 
{
  Serial.begin(9600); //initialize serial communication at 9600 bits per second
  pinMode(13, OUTPUT); // This specifies that digital line D13 will be used for output (not input, which is the default)
  Vernier.autoID();// this is the routine to do the autoID Serial.println("Vernier Format 2");
  Serial.println(Vernier.sensorName());
  Serial.print(" ");
  Serial.println("Readings taken using Ardunio");
  Serial.println("Data Set");
  Serial.print("Time");//long name
  Serial.print("\t"); //tab character
  Serial.println(Vernier.sensorName());
  Serial.print ("t"); //short name 
  Serial.print("\t"); //tab character
  Serial.println(Vernier.shortName());
  Serial.print("seconds");//units
  Serial.print("\t"); // tab character
  Serial.println(Vernier.sensorUnits());
}
void loop() 
{
   sensorReading =Vernier.readSensor();
  Serial.print(readingNumber/1000.0*timeBetweenReadings); 
  Serial.print ("\t");
  Serial.println(sensorReading);

  delay(timeBetweenReadings);// delay in between reads for stability
  if (sensorReading>threshold){
    digitalWrite(13, HIGH);
    }// This turns on the LED 
  else 
    {
    digitalWrite(13,LOW);
    }
  readingNumber++;
}





