/*
VernierMousetrap (v 2014.09)

A Vernier Photogate should also be connnected to the second BTD 1 connector.
The sketch monitors this photogate and Controls a stepper motor (unipolar or 
bipolar) via a Vernier Digital Control Unit (DCU) connected to the second BTD 
connector.

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
//the lines below are so that you can quickly change this code if you want to
//use the DCU in the BTD1 connector for some reason.
int DCUinBTD2=1;// change this to 0 if you want to use the DCU on BTD 1
const int Pin1 = 2 +DCUinBTD2*4;
const int Pin2 = 3 +DCUinBTD2*4;
const int Pin3 = 4 +DCUinBTD2*4;
const int Pin4 = 5 +DCUinBTD2*4;
int photogate =2; //This is the input for a photogate on the BTD 1 connector
int LEDpin =13;/// line for LED to turn on when photogate is blocked
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
    Serial.println(" starting loop ");
    buttonPress = digitalRead(photogate);//low when blocked
if (buttonPress == LOW) 
   { 
    digitalWrite(LEDpin, HIGH);// turn on LED 
    Serial.print(buttonPress);
    Serial.println("  Blocked ");
    Step(55,0) ;// use stepper motor to lower door
    DCU(0);//Turn off all lines
    delay (10000); //wait 10 seconds, note that the Arduino/shield LED will stay on
    digitalWrite(LEDpin, LOW);// turn off Arduino/shield LED  when ready to trap again
   }
   else
   {
     DCU(0);//Turn off all lines
     digitalWrite(LEDpin, LOW);// turn off LED 
   }
} ;// end of loop
 
void Step(int Steps, int direction)
 {
   if (direction ==0)
     {
      DCUStep[0]=5;// 5,9,10,6}, in this order for CW rotation
      DCUStep[1]=9;
      DCUStep[2]=10;
      DCUStep[3]=6; 
     }
    else
     {
       DCUStep[0]=6;// 6,9,10,6}; //on this order for CCW
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
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
      break;
    case 13:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, LOW);
      break;
    case 14:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, LOW);
      break;
    case 15:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, HIGH);
      break;
  }
}
