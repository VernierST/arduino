/*
VernierHandControl1(v 2018.05)

This sketch was written to control one finger of the  hand described in the Microsoft Hacking STEM hand project. 

This version reads a Vernier Low-G Accelerometers (LGA-BTA) sensors connected to the Analog 1 connnector of the Vernier
Arduino Shield. It is read as pin A0 of the Arduino. 

This project requires the following hardware:
   - An Arduio Uno or Sparkfun Redboard
   - A Vernier Arduino Shield
   - a Vernier Digital Control Units (DCU-BTA)
   - a 6-volt power supplies such as Vernier LabPro power supplies or CBL2 Power supplies
   - tape for attaching the accelerometer to your finger
   
The voltage is scaled using the handy Arduino Map function and then the servo motor is controlled.
This version uses Analog 1 (Arduino line A0) to control just one fingers.
There are more complex versions which requires more hardware. 

The servo motor ise assumed to be connected to the servo connector of the DCU in the Digital 2 connector of the shield. It is
controlled by Arduino line D9.

 The DCU needs to be connected to 5- volt, LabQues  power suppplY (LQ-PS) to supply the power for the servos. 

 This version sets the finger to the up position at the start.
 It has you press the D12 button to get things started. This allows people to 
 get the accelerometer in the up position before starting to move 
 the servos. This will help avoid wild bending of the straw, which can damage them.
 */

#include <Servo.h> 
//only two declared servos needed in this version. One is used for thumb and the other for all fingers
Servo myservo0;  // create servo object to control a servo  for thumb

int fingerStart= 580; //default LGA reading for starting (up) position of the finger
int fingerStop= 450; //default LGA reading for down position of the finger
int fingerSetting;
int fingerSettingAverage=fingerStart ;
//for button control:
int buttonPin= 12; // analog input pin to use as a digital input
int ledPin= 13; // digital output pin for LED 1 indicator
int buttonVal = 0; // value read from button
boolean buttonHasBeenPressed=LOW;

void setup() 
{ 
  Serial.begin(9600);
  pinMode( 6, OUTPUT);
  pinMode( 7, OUTPUT);
  pinMode( 8, OUTPUT);
  pinMode( 9, OUTPUT);
  myservo0.attach(9);// attaches the servo object to a pin. This one uses the servo connector on DCU1, used for thumb

  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);;//set unused DCU 2 lines low (to avoid problems caused by the DCU output patters for 13, 14, and 15
  
  // Set button input pin
  pinMode(buttonPin, INPUT);//pin 12
  digitalWrite(buttonPin, HIGH);//initialize button state
  pinMode(ledPin, OUTPUT);// Set LED output (pin 13)
}

void loop() 
{
  if (buttonHasBeenPressed == HIGH) //we have already dealt with the button
    { //This is the code to control the fingers 
      fingerSetting= analogRead(0);// note channel A0 is for the finger

      float alpha= 0.7;//factor used in calculating a running averageing
      fingerSettingAverage=alpha*fingerSetting + (1-alpha)*fingerSettingAverage;// calculate running average
 
      Serial.println("Finger Setting: ");
      Serial.print(fingerSetting); 
      Serial.print(" ");
      Serial.println(fingerSettingAverage); 
      //delay (300);//slow down so you can read things
      
      setServos();
     } // end of if

     else
    { //put hand in up position and read the state of the button
      fingerSetting= fingerStart;
      setServos(); //call subroutine to set servos.
      Serial.println("Press D12 button to start hand control. Finger is now up:");
      delay (100);
      //now test for button press:
      buttonVal = digitalRead(buttonPin);// Test for button pressed, button down is low
      if (buttonVal== LOW) buttonHasBeenPressed = HIGH; //button pressed
     }//end of else
} //end of loop

   void setServos()//set servo.  This assumes that the fingerSetting[]  
    {                          //is correct and used as a global variable.
     
      int servoSetting0;//used for finger position
      servoSetting0 = map(fingerSettingAverage,fingerStart, fingerStop, 11, 179);// scale it to match the index finger
      //the last two numbers in the statement above control the range of motion of the servo motor
      myservo0.write (servoSetting0);// set finger position  

      /*  //print these out if you want to see servo setting:
      Serial.println("ServoSetting: ");// print these out if you need to for testing.
      Serial.println(servoSetting0);
      delay (300);
      */
    }// end of setServo subroutine

   
