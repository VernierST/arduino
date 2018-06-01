#include "VernierLib.h" 
VernierLib Vernier;
float distance = 0;// distqnce in cm 
void setup()
{
  Serial.begin(9600);
  Serial.print("Motion Detector reading...");
}

void loop()
{
  distance = Vernier.readMotionDetector();
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);//delay a tenth of a second
}
