#include "VernierLib.h" 
VernierLib Vernier;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
   for (int DCUSetting=0; DCUSetting <= 7; DCUSetting++)
   {
      Serial.println(DCUSetting);
      Vernier.DCU(DCUSetting);
      delay(1000);
   } 
}

