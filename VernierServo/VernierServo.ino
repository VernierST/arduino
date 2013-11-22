/*
VernierServo (v 2013.11)
Reads a Vernier analog (BTA) sensor connnected to pin A0 of the Arduino. This
sketch controls the arm on a servo motor in response to this voltage from this 
sensor. The servo motor is assumed to be connected to pin D9 of the Arduino.
  
This sketch is a very slight modification of the Knob sketch included with the Arduino 1.0.4 Examples.
  
Note that if you have wired the BTD2 connector as we recommend on 
www.vernier.com/arduino or if you are using a SparkFun Vernier Interface Shield,
you can make the connection to proper line for the servo motor from DIO3 of the
BTD2 connector (the pin closest to the tab). 
*/

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
 
int Sensorpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin 
 
void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
} 
 
void loop() 
{ 
  val = analogRead(Sensorpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  delay(15);                           // waits for the servo to get there 
} 
