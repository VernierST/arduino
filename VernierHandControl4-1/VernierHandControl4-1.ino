/*
VernierHandControl4-2(v 2018.05)

 This sketch was written to control the thumb. ring, and smallest finger of the hand described in the Microsoft Hacking STEM hand project. 

 The DCUs are connected to two 5- volt, LabQuest power suppplies (Vernier order code LQ-PS) to supply the power for the servos. 
 The system sometime does not not work well with a 5-volt power supply connected to the DCU.

 This version sets the fingers to the up position at the start.
 It has you press the D12 button to get things started. This allows people to 
 get the controlling glove in the up position before starting to move 
 the servos. This will help avoid wild bending of the straws, which can damage them.
 */

#include <Servo.h> 
Servo myservo0;  // create servo object to control a servo  for thumb
Servo myservo1; // create servo object to control a servo for smallest finger
Servo myservo2; // create servo object to control a servo for ring finger
int thumbStart=  605; //default LGA reading for thumb in up position
int fingerStart= 580; //default LGA reading for starting position of ring and smallest fingers 
int thumbStop=  470; //default LGA reading for thumb in up position
int fingerStop= 450; //default LGA reading for starting position of ring and smallest fingers 
int fingerSetting;
int thumbSetting;
int fingerSettingAverage=fingerStart;
int thumbSettingAverage= thumbStart;
//for button control:
int buttonPin= 12; // analog input pin to use as a digital input
int ledPin= 13; // digital output pin for LED 1 indicator
int buttonVal = 0; // value read from button
boolean buttonHasBeenPressed=LOW;

void setup() 
{ 
  Serial.begin(9600);
  pinMode( 2, OUTPUT);//set all DCU lines for output:
  pinMode( 3, OUTPUT);
  pinMode( 4, OUTPUT);
  pinMode( 5, OUTPUT);
  pinMode( 6, OUTPUT);
  pinMode( 7, OUTPUT);
  pinMode( 8, OUTPUT);
  pinMode( 9, OUTPUT);
  myservo0.attach(5);// attaches the servo object to a pin. This one uses the servo connector on DCU1, used for thumb
  myservo1.attach(2);// servo connector on DCU2, used for smallest finger
  myservo2.attach(3);// screw terminal 1 on DCU connected to Digital 1 , used for ring finger
  digitalWrite(4, LOW);//set unused DCU 1 lines low (to avoid problems caused by the DCU output patters for 13, 14, and 15
  digitalWrite(8, LOW);//set unused DCU 2 lines low (to avoid problems caused by the DCU output patters for 13, 14, and 15
  
  // Set button input pin
  pinMode(buttonPin, INPUT);//pin 12
  digitalWrite(buttonPin, HIGH);//initialize button state
  pinMode(ledPin, OUTPUT);// Set LED output (pin 13)
 
}

void loop() 
{
  if (buttonHasBeenPressed == HIGH) //we have already dealt with the button
    { //This is the code to control the fingers 
      thumbSetting= analogRead(0);// note channel A0 is for thumb
      fingerSetting= analogRead(2);// note channel A2 is for ring and smallest finger

      float alpha= 0.7;//factor used in calculating a running averageing
      thumbSettingAverage=alpha*thumbSetting + (1-alpha)*thumbSettingAverage;// calculate running average
      fingerSettingAverage=alpha*fingerSetting + (1-alpha)*fingerSettingAverage;// calculate running average
 
      Serial.println("Thumb and Finger Settings: ");
      Serial.print(thumbSetting); 
      Serial.print(" ");
      Serial.print(thumbSettingAverage);
      Serial.print(" ");
      Serial.print(fingerSetting); 
      Serial.print(" ");
      Serial.println(fingerSettingAverage); 
      //delay (300);//slow down so you can read things
      
      setServos();
     } // end of if

     else
    { //put hand in up position and read the state of the button
      thumbSetting= thumbStart;
      fingerSetting= fingerStart;
      setServos(); //call subroutine to set servos.
      Serial.println("Press D12 button to start hand control. All fingers are now up:");
      delay (100);
      //now test for button press:
      buttonVal = digitalRead(buttonPin);// Test for button pressed, button down is low
      if (buttonVal== LOW) buttonHasBeenPressed = HIGH; //button pressed
     }//end of else
} //end of loop

   void setServos()//set servo.  This assumes that the fingerSetting[]  
    {                          //is correct and used as a global variable.
     
      int servoSetting0;//used for thumb
      int servoSetting1;//used for middle and smallest fingers

      servoSetting0 = map(thumbSettingAverage,thumbStart, thumbStop, 11, 179);// scale it to match the thumb
      //the last two numbers in the statement above control the range of motion of the servo motor
      servoSetting1 = map(fingerSettingAverage,fingerStart, fingerStop, 11, 179);// scale it to match the fingers
      //the last two numbers in the statement above control the range of motion of the servo motor
      //servoSetting0 = map(thumbSetting,thumbStart, thumbStop, 11, 179); //thumb (Has its own control)
      // servoSetting1 = map(fingerSetting,fingerStart, fingerStop, 11, 179);// scale it to match the ring and smallest finger   s  
      myservo0.write (servoSetting0);// set thumb position  
      myservo1.write (servoSetting1); //set smallest finger position  
      myservo2.write (servoSetting1); //set ring finger position  
      /*  //print these out if you want to see servo settings:
      Serial.print("ServoSettings: ");// print these out if you need to for testing.
      Serial.print(servoSetting0);
      Serial.print (" ");
      Serial.println(servoSetting1);
      delay (300);
      */
    }// end of setServo subroutine

   
