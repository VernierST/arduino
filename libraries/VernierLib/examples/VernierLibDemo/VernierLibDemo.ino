#include "VernierLib.h" 
VernierLib Vernier;
float sensorReading;
void setup()
{
  Serial.begin(9600);
  Vernier.autoID();// this is the routine to do the autoID
  printSensorInfo();
}

void loop()
{
  sensorReading =Vernier.readSensor();
  Serial.print(sensorReading);
  Serial.print(" ");
  Serial.println(Vernier.sensorUnits());
  delay(500);//half a second
} 

 void printSensorInfo()
 {
   // print out information about the sensor found:
   Serial.println("Sensor Information:"); 
   Serial.print("Sensor ID number: "); 
   Serial.print("\t");
   Serial.println(Vernier.sensorNumber());
   Serial.print("Sensor Name: ");
   Serial.print("\t");
   Serial.println(Vernier.sensorName());
   Serial.print("Short Name: "); 
   Serial.print("\t");
   Serial.println(Vernier.shortName()); 
   Serial.print("Units: "); 
   Serial.print("\t");
   Serial.println(Vernier.sensorUnits());
   Serial.print("ID voltage level: "); 
   Serial.print("\t");
   Serial.println(Vernier.voltageID());
   Serial.print("Page: ");  
   Serial.print("\t");
   Serial.println(Vernier.page());
   Serial.print("slope: ");
   Serial.print("\t");
   Serial.println(Vernier.slope());
   Serial.print("intercept: ");
   Serial.print("\t");
   Serial.println(Vernier.intercept());
   Serial.print("cFactor:");
   Serial.print("\t");
   Serial.println(Vernier.cFactor());
   Serial.print("calEquationType: ");
   Serial.print("\t");
   Serial.println(Vernier.calEquationType());
 }
