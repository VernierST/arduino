/*VernierLaserTracker  (v 2018.5)
Monitors the position of an object using a Vernier Motion Detector
and then aims a laser pointer mounted on a servo motor at the object.

This sketch uses three Arduino libraries. To read the Motion Detector, it uses
the VernierLib library. Because of the use of a trig function (arctangent) in the 
calculations, this sketch requires the math.h library. It also requires the servo 
library to control the servo motor.

See www.vernier.com/arduino for more information.
 */
#include <math.h>
#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 
#include "VernierLib.h" 
VernierLib Vernier;
float distance = 0;// distqnce in cm 
int Range=100; //distance in cm from Laser Pointer/Servo motor to Motion Detector
const int laser = 6;//laser pin, connect laser to pin 6 of the Arduino, which is the first line on Digital 2

void setup() 
{
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println(" ");
  Serial.print("Distance");
  Serial.print("\t"); //tab character
  Serial.print ("Angle"); 
    Serial.print("\t"); //tab character
  Serial.println ("Angle"); 
  Serial.print("centimeters");
  Serial.print("\t"); // tab character
  Serial.print ("radians"); 
  Serial.print("\t"); // tab character
  Serial.println ("degrees"); 
}
void loop() 
{
  float ArcTan=0;
  double Angle =0;
  float Degrees;
  int ServoSetting;
    distance = Vernier.readMotionDetector();
  Serial.print(distance);
  Serial.println(" cm");
  if (distance < 120)
   {
     digitalWrite(laser, HIGH);// if closest item is within 120 cm it turns laser on
   }
  else digitalWrite(laser, LOW);
  delay(100);//delay a tenth of a second
  
  Serial.print(distance);
  Serial.print("\t"); // tab character
  ArcTan = atan(distance/Range); 
  Serial.print(ArcTan);
  Serial.print("\t"); // tab character
  Degrees= ArcTan*57.29578; //convert radians to degrees
  Serial.println(Degrees);
  ServoSetting =Degrees;
  myservo.write(ServoSetting); // sets the servo position according to the scaled value                           
  delay(50); //delay a bit
}



