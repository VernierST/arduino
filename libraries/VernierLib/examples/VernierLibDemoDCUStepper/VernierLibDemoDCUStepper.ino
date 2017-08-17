#include "VernierLib.h" 
VernierLib Vernier;
int stepDelay=30; //step time in ms. If this value is too low for your motor 
int stepCount=48;
int stepDirection =0;//direction 0 =CW, 1 = CCW

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Vernier.DCUStep(stepCount,stepDirection, stepDelay);
  Vernier.DCU(0); // turn off all lines
  delay(500);//delay a half second
  stepDirection ++;
  if (stepDirection>1) stepDirection =0;
}
