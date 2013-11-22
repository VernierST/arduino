/*
VernierRotary (v 2013.11)

This sketch read a Vernier Rotary Motion Sensor connected to BTD connector.
This sketch uses interrupts. It is very difficult to read the pulses from
the Rotary Motion Sensor without interrupts.

As written, the readings will be displayed every half second. Change the variable 
TimeBetweenReadings to change the rate.

See www.vernier.com/arduino for details.

*/
float Time;
int ReadingNumber=0;
int TimeBetweenReadings = 500; // in ms
const int encoderPinCCW = 2;
const int encoderPinCW = 3;
const int XR = 8;
boolean HighResOn = false;
float Pos, oldPos;
volatile int encoderPos = 0; // variables changed within interrupts are volatile
float res = 1;
void setup()
{
  pinMode(encoderPinCCW, INPUT);
  pinMode(encoderPinCW, INPUT);
  pinMode (XR, OUTPUT);
  digitalWrite(XR,HighResOn);
  digitalWrite(encoderPinCCW, HIGH);
  digitalWrite(encoderPinCW, HIGH);
  Serial.begin(9600);
  attachInterrupt(0, doEncoderCCW, RISING); // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(1, doEncoderCW, FALLING); // encoder pin on interrupt 1 (pin 3)
  if(HighResOn) res = .25;
  else res =1;
  Serial.println("Vernier Format 2");
  Serial.println("Rotary Motion Readings taken using Ardunio");
  Serial.println("Data Set");
  Serial.print("Time");
  Serial.print("\t"); //tab character
  Serial.println ("Angle"); 
  Serial.print("seconds");
  Serial.print("\t"); // tab character
  Serial.println ("degrees"); 
 }
void loop()
{
  uint8_t oldSREG = SREG;
  cli();
  Pos = encoderPos*res;
  SREG = oldSREG;
  Time = (ReadingNumber/1000.0*TimeBetweenReadings);
  Serial.print(Time); // print time in seconds
  Serial.print("\t"); //tab character
  Serial.println(encoderPos*res);   // display temperature to one digit                                
  delay(TimeBetweenReadings); 
  ReadingNumber++;  // Delay a bit... 
}

void doEncoderCCW()
{
    encoderPos++;    // count UP
}
void doEncoderCW()
{
    encoderPos--;    // count DOWN 
}     
    
