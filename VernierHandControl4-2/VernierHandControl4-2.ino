/*
VernierHandControl4-2(v 2018.05)

 This sketch was written to control the index and middle finger of the hand described in the Microsoft Hacking STEM hand project. 

 The DCUs are connected to two 5- volt, LabQuest power suppplies (Vernier order code LQ-PS) to supply the power for the servos. 

 This version sets the fingers to the up position at the start.
 It has you press the D12 button to get things started. This allows people to 
 get the controlling glove in the up position before starting to move 
 the servos. This will help avoid wild bending of the straws, which can damage them.
 */

#include <Servo.h> 
//only two declared servos needed in this version. One is used for index and the other for middle finger
Servo myservo0;  // create servo object to control a servo for index finger
Servo myservo1; // create servo object to control a servo for middle finger

int finger;
int indexStart=  540; //default LGA reading for index in up position
int fingerStart= 570; //default LGA reading for starting position of ring fingers 
int indexStop=  470; //default LGA reading for thumb in up position
int fingerStop= 450; //default LGA reading for starting position of ring and smallest fingers 
int fingerSetting;
int indexSetting;
int fingerSettingAverage=fingerStart ;
int indexSettingAverage= indexStart;
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
  myservo0.attach(9);// attaches the servo object to a pin. This one uses the servo connector on DCU1, used for index
  myservo1.attach(6);// servo connector on DCU2, used for smallest finger
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
      indexSetting= analogRead(0);// note channel A0 is index finger
      fingerSetting= analogRead(2);// note channel A2 is for middle finger

      float alpha= 0.7;//factor used in calculating a running averageing
      indexSettingAverage=alpha*indexSetting + (1-alpha)*indexSettingAverage;// calculate running average
      fingerSettingAverage=alpha*fingerSetting + (1-alpha)*fingerSettingAverage;// calculate running average
 
      Serial.println("Index and Finger Settings: ");
      Serial.print(indexSetting); 
      Serial.print(" ");
      Serial.print(indexSettingAverage);
      Serial.print(" ");
      Serial.print(fingerSetting); 
      Serial.print(" ");
      Serial.println(fingerSettingAverage); 
      //delay (300);//slow down so you can read things
      
      setServos();
     } // end of if

     else
    { //put hand in up position and read the state of the button
      indexSetting= indexStart;
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
     
      int servoSetting0;//used for index finger
      int servoSetting1;//used for middle finger
      servoSetting0 = map(indexSettingAverage,indexStart, indexStop, 11, 179); //index (Has its own control)
      servoSetting1 = map(fingerSettingAverage,fingerStart, fingerStop, 11, 179);// scale it to match the index finger
      //servoSetting0 = map(indexSetting,indexStart, indexStop, 11, 179); //index (Has its own control)
      // servoSetting1 = map(fingerSetting,fingerStart, fingerStop, 11, 179);// scale it to match the middle finger     
      myservo0.write (servoSetting0);// set index position  
      myservo1.write (servoSetting1); //set middle finger position  
      /*  //print these out if you want to see servo settings:
      Serial.print("ServoSettings: ");// print these out if you need to for testing.
      Serial.print(servoSetting0);
      Serial.print (" ");
      Serial.println(servoSetting1);
      delay (300);
      */
    }// end of setServo subroutine

   
