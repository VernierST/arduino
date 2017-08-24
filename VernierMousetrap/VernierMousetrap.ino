/*
  VernierMousetrap (v 2017.07)

  A Vernier Photogate should also be connnected to the second BTD 1 connector.
  The sketch monitors this photogate and Controls a stepper motor (unipolar or
  bipolar) via a Vernier Digital Control Unit (DCU) connected to the BTD 2
  connector.

  This sketch will continuously check the photogate and start the stepper
  motor rotating when the photogate is blocked. External power must be supplied
  to the DCU to drive the stepper motor.

  This version uses the VernierLib library

  See www.vernier.com/arduino for more information.
*/

#include "VernierLib.h"
VernierLib Vernier;
int photogateStatus = HIGH;
int stepDelay = 30; //step time in ms
int stepCount = 55; //number of stepCount to take
int stepDirection = 0; //stepDirection 0 =CW
int photogate = 2; //This is the input for a photogate on the BTD 1 connector
int LEDpin = 13; /// line for LED to turn on when photogate is blocked

void setup()
{
  Serial.begin(9600); // set up Serial library at 9600 bps
}// end of setup

void loop ()
{
  Serial.println(" starting loop, waiting for mouse... ");
  photogateStatus = digitalRead(photogate);//low when blocked
  if (photogateStatus == LOW)
  {
    digitalWrite(LEDpin, HIGH);// turn on LED
    Serial.print(photogateStatus);
    Serial.println("  Blocked ");
    Vernier.DCUstep(stepCount,stepDirection, stepDelay);// use stepper motor to lower door
    Vernier.DCU(0);//Turn off all lines
    delay (10000); //wait 10 seconds, note that the Arduino/shield LED will stay on
    digitalWrite(LEDpin, LOW);// turn off Arduino/shield LED  when ready to trap again
  }
  else
  {
    Vernier.DCU(0);//Turn off all lines
    digitalWrite(LEDpin, LOW);// turn off LED
  }
} ;// end of loop


