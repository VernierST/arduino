/*
 VernierStepper (v 2014.09)
Controls a stepper motor (unipolar or bipolar) connected to a Vernier Digital
Control Unit (DCU) connected to BTD 2 connector.

Stepper motors are unique in that they allow for very exact positioning of the 
drive shaft. Additional guidance related to stepper motors can be found in the 
user manual for the DCU http://www.vernier.com/files/manuals/dcu-btd.pdf 

This sketch causes the stepper motor to rotate 100 steps and then the rotation
pauses for one second and the rotation is repeated in the opposite direction.
See www.vernier.com/arduino for more information.

The sketch makes use of a segment of code for the DCU. 

There are at least two points of note for this sketch. First the DCU must have 
an external power source in order drive the motor. Second, if you try to cycle the 
stepper motor too quickly (by having a small int Duration) the motor may not be 
able to complete its cycle before being forced to start again. In both cases you will 
observe the shaft on the motor vibrating, but not rotating.

*/

int Duration=50; /*step time in ms. If this value is too low for your motor 
it will cause the motor to shake without moving.*/
int Direction =0;//direction 0 =CW, 1 = CCW
//the lines below are so that you can quickly change this code if you want to
//use the DCU in the BTD1 connector for some reason.
int DCUinBTD2=1;// change this to 0 if you want to use the DCU on BTD 1
const int Pin1 = 2 +DCUinBTD2*4;
const int Pin2 = 3 +DCUinBTD2*4;
const int Pin3 = 4 +DCUinBTD2*4;
const int Pin4 = 5 +DCUinBTD2*4;
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
Step(100,0);//rotate clockwise 100 steps
delay (1000);
Step(100,1);// rotate counter clockwise 100 steps
} ;// end of loop
 
void Step(int Steps, int direction)
{
  int x;
  int StepValue;
  if (direction ==0)
    { 
      /* 
      The following sequences are for a "normal" stepper motor.
      5,9,10,6 steps the motor CW
      Reverse the order for CCW      
      */
      DCUStep[0]=5; //CW
      DCUStep[1]=9;
      DCUStep[2]=10;
      DCUStep[3]=6; 
    }
    else
     {
       DCUStep[0]=6; //CCW
       DCUStep[1]=10;
       DCUStep[2]=9;
       DCUStep[3]=5; 
    }
  for (x=0; x<=Steps; x++) // set up step pattern
    {
      output = x%4;// % = modulo: returns the remainder of x divided by the value (4) in the case
      //as "x" increments "output" will progress from 0, 1, 2, 3 repeating
      StepValue = DCUStep[output];
      DCU(StepValue); // This points to the case for the DCU. See void DCU below.
      Serial.print(output);
      Serial.print("  ");
      Serial.println(StepValue);
      delay (Duration);
    } ;//end of for
};// end of Step
 
void DCU (int output)// This segment of code is copied from a DCU sketch as a complete module
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

