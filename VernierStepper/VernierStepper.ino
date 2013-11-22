/*
 VernierStepper (v 2013.11)
Controls a stepper motor (unipolar or bipolar) connected to a Vernier Digital
Control Unit (DCU) connected to BTD connector.

This sketch causes the stepper motor to rotate 100 steps and then the rotation
pauses for one second and the rotation is repeated in the opposite direction.
See www.vernier.com/arduino for more information.

*/int Duration=30; //step time in ms
int Steps = 4; //number of steps to take
int Direction =0;//direction 0 =CW
int x;
int StepValue;
const int Pin1 = 2;
const int Pin2 = 3; 
const int Pin3 = 4; 
const int Pin4 = 5; 
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
DCU(0);//Turn off all lines
delay (1000);
Step(100,0);//rotate clockwise
delay (1000);
Step(100,1);// rotate counter clockwise
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

