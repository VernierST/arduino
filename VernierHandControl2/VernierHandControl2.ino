/*
VernierHandControl2(v 2018.05)

This sketch was written to control the hand described in the Microsoft Hacking STEM hand project. 

This version reads 2 Vernier Low-G Accelerometers (LGA-BTA) sensors connected to pins A0 and A2 of the Arduino. 
It requires the following hardware:
   - A Sparkfun Redboard or Arduio Uno
   - A Vernier Arduino Shield
   - (2) Vernier Digital Control Units (DCU-BTA)
   - (2) 5-volt power supplies such as Vernier LabQuest power supplies (Vernier order code LQ-PS)
   - a glove
   - tape, glue or some method of attaching the two accelerometers to the glove
   
The voltage is scaled using the handy Arduino Map function and then the servo motors are controlled.
This version uses Analog 1 (Arduino line A0) to control the thumb and Analog 2 (Arduino line A2) to control the 4 fingers.
There is a more complex version which requires more hardware. That version is controlled with the VernierHandControl5 sketch. 

The five servo motors are assumed to be connected to pin D1, D2, D5, D6, and D9 of the Arduino as follows:
 - The thumb servo is controlled by the DCU in Digital 1, and uses the servo connector (Arduino line D5)
 - The thumb finger servo is controlled by the DCU in Digital 2, and uses the servo connector (Arduino line D9)
 - The smallest finger (pinky) servo is controlled by the DCU in Digital 1, and uses the servo connector (Arduino line D2)
 - The ring finger servo is controlled by the DCU in Digital 1, and uses the servo connector (Arduino line D3)
 - The middle servo is controlled by the DCU in Digital 2, and uses the screw terminal 1 connector (Arduino line D6)

 You may want to use short wires to make the connections between the screw terminals and the servo moter connetors.

 The DCUs are connected to two 5-volt, LabQuest power suppplies (Vernier order code LQ-PS) to supply the power for the servos. 

 This version sets the fingers to the up position at the start.
 It has you press the D12 button to get things started. This allows people to 
 get the controlling glove in the up position before starting to move 
 the servos. This will help avoid wild bending of the straws, which can damage them.

 We have a more complex version of this project, which usese 2 Arduinos (with shields) and 4 accelerometers to give more
 complete control of the fingers.
 */

#include <Servo.h> 
//only two declared servos needed in this version. One is used for thumb and the other for all fingers
Servo myservo0;  // create servo object to control a servo  for thumb
Servo myservo1; // create servo object to control a servo for all fingers
Servo myservo2; // create servo object to control a servo for all fingers
Servo myservo3; // create servo object to control a servo for all fingers
Servo myservo4; // create servo object to control a servo for all fingers

int finger;
int thumbStart=  605; //default LGA reading for thumb in up position
int fingerStart= 580; //default LGA reading for starting position of all fingers in the up position
int thumbStop=  470; //default LGA reading for thumb in the down position
int fingerStop= 450; //default LGA reading for all fingers in the down position
int fingerSetting;
int thumbSetting;
int fingerSettingAverage=fingerStart ;
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
  myservo1.attach(9);// servo connector on DCU2, used for index finger
  myservo2.attach(2);// screw terminal 1 on DCU connected to Digital 1 , used for pinky)
  myservo3.attach(3);// screw terminal 2 on DCU connected to Digital 1 , used for ring finger)     
  myservo4.attach(6);// screw terminal 1 on DCU connected to Digital 2 , used for middle finger)

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
      thumbSetting= analogRead(0);// note channel A0 is thumb
      fingerSetting= analogRead(2);// note channel A2 is for all fingers

      float alpha= 0.7;//factor used in calculating a running averageing
      thumbSettingAverage=alpha*thumbSetting + (1-alpha)*thumbSettingAverage;// calculate running average
      fingerSettingAverage=alpha*fingerSetting + (1-alpha)*fingerSettingAverage;// calculate running average
 
      Serial.println("thumb and Finger Settings: ");
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
     
      int servoSetting0;//used for thumb finger
      int servoSetting1;//used for middle finger
      servoSetting0 = map(thumbSettingAverage,thumbStart, thumbStop, 11, 179);// scale it to match the thumb
      //the last two numbers in the statement above control the range of motion of the servo motor
      servoSetting1 = map(fingerSettingAverage,fingerStart, fingerStop, 11, 179);// scale it to match the fingers
      //the last two numbers in the statement above control the range of motion of the servo motor
      myservo0.write (servoSetting0);// set thumb position  
      myservo1.write (servoSetting1); //set finger position  
      myservo2.write (servoSetting1); //set finger position  
      myservo3.write (servoSetting1); //set finger position  
      myservo4.write (servoSetting1); //set finger position  
  
      /*  //print these out if you want to see servo settings:
      Serial.print("ServoSettings: ");// print these out if you need to for testing.
      Serial.print(servoSetting0);
      Serial.print (" ");
      Serial.println(servoSetting1);
      delay (300);
      */
    }// end of setServo subroutine

   
