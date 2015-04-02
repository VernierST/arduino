/*
VernierServo (v 2015.04)
Reads a Vernier analog (BTA) sensor connnected to pin A0 of the Arduino or the 
BTA 1 connector. This sketch controls the arm on a servo motor in response to 
this voltage from the sensor. The servo motor is assumed to be connected to 
pin D9 of the Arduino. You can also connect to this line using the BTD 2 connector.
This is an exception to our convention of reserving the BTD 2 connector for the 
digital readout. The servo motors must be controlled by pin 9 on the RedBoard/Arduino.
  
This sketch is a very slight modification of the Knob sketch included with the 
Arduino 1.0.4 Examples.
  
Note that if you have wired the BTD2 connector as we recommend on 
www.vernier.com/arduino or if you are using a SparkFun Vernier Interface Shield,
you can make the connection to proper line for the servo motor from DIO4 (pin 9) of the
BTD2 connector (the pin closest to the tab). 

If you connect the servo motor (3-pin) connector on a DCU and connect the DCU to
the BTD2 connector on the Arduino Interface Shield, this (2015.04) version will work. 
The DCU will require an external power supply as the power received from the Redboard/
Arduino is not sufficient to drive the servo.

*/

#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 
int Sensorpin = 0;  // analog pin used to connect the potentiomete


void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 

/* the 6 lines below are only needed if you plan to use this sketch with a servo motor
connected to the 3-pin connector on a DCU. The reason this is required is that the DCU 
has internal electronics to only allow its 4th pin to be turned on when the other first
three lines are off.
*/

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}
void loop() 
{ 
  int val = analogRead(Sensorpin);     // reads the voltage (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  delay(15);                           // waits for the servo to get there 
} 
