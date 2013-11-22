/*
VernierMousetrap (v 2013.11)
Controls a stepper motor (unipolar or bipolar) via a Vernier Digital
Control Unit (DCU) connected to the first BTD connector.

A Vernier Photogate should also be connnected to the second BTD connector. 

This sketch will continuously check the photogate and start the stepper
motor rotating when the photogate is blocked.
  
See www.vernier.com/arduino for more information.
 */

int buttonPress = HIGH;
int Duration=30; //step time in ms
int Steps = 55; //number of steps to take
int Direction =0;//direction 0 =CW
int x;
int StepValue;
const int Pin1 = 2; // pin
const int Pin2 = 3; // pin
const int Pin3 = 4; // pin
const int Pin4 = 5; // pin
int photogate =3; //
int output; //number sent to DCU
int DCUStep[4]; //pattern used to drive stepper motor
void setup() 
{
  Serial.begin(9600); // set up Serial library at 9600 bps
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);
  pinMode(Pin4, OUTPUT);
};// end of setup
void loop ()
{
buttonPress = digitalRead(photogate);//low when blocked
if (buttonPress == LOW) 
   { 
     Step(55,0) ;// use stepper motor to lower door
     Serial.print(buttonPress);
     Serial.println("  Blocked ");
   }
DCU(0);//Turn off all lines
delay (10000);
} ;// end of loop
 
void Step(int Steps, int direction)
 {
   if (direction ==0)
     {
      DCUStep[0]=5;//,9,10,6}; //on this order for CW
      DCUStep[1]=9;
      DCUStep[2]=10;
      DCUStep[3]=6; 
     }
    else
     {
       DCUStep[0]=6;//,9,10,6}; //on this order for CW
       DCUStep[1]=10;
       DCUStep[2]=9;
       DCUStep[3]=5; 
   }
  for (x=0; x<=Steps; x++) // set up step pattern
    {
      output = x%4;
      StepValue = DCUStep[output];
      DCU(StepValue);
      Serial.print(output);
      Serial.print("  ");
      Serial.println(StepValue);
      delay (Duration);
    } ;//end of for
};// end of Step
 
void DCU (int output)
{
  switch (output) 
  {
    case 0:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
      break;
    case 1:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
      break;
    case 2:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
    break;   
    case 3:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
      break;
    case 4:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, LOW);
      break;    
    case 5:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, LOW);
      break;
    case 6:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, LOW);
      break;
    case 7:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, LOW);
      break;
    case 8:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, HIGH);
      break;
    case 9:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, HIGH);
      break;
    case 10:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, HIGH);
      break;
    case 11:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, HIGH);
      break;
    case 12:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, HIGH);
      break;
    case 13:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, HIGH);
      break;
    case 14:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, HIGH);
      break;
    case 15:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, HIGH);
      break;
  }
}
