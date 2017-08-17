#include "VernierLib.h" 
VernierLib Vernier;
float sensorReading;
void setup()
{
  Serial.begin(9600);
  Vernier.autoID();// this is the routine to do the autoID
}
void loop()
{
  sensorReading =Vernier.readSensor();
  Serial.print(sensorReading);
  Serial.print(" ");
  Serial.println(Vernier.sensorUnits());
  delay(500);//half a second
} 

