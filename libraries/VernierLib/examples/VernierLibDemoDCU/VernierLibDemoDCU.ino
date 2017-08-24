#include "VernierLib.h" 
VernierLib Vernier;
byte DCUSetting= 0;
float sensorReading;
float threshold = 5.0;

void setup()
{
  Serial.begin(9600);
  Vernier.autoID();// this is the routine to do the autoID
}
void loop()
{
  sensorReading = Vernier.readSensor();
  Serial.print(sensorReading);
  Serial.print(" ");
  Serial.println(Vernier.sensorUnits());
  if (sensorReading >threshold )
  {
    DCUSetting =7;
  }
  else 
  {
    DCUSetting =0;
  }
  Vernier.DCU(DCUSetting);
  delay(500);                            
}
