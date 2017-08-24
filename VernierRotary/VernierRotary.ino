/*
VernierRotary (v 2017.07)

This sketch read a Vernier Rotary Motion Sensor connected to the BTD 1 connector.
This sketch uses interrupts. It is very difficult to read the pulses from
the Rotary Motion Sensor without interrupts.

As written, the readings will be displayed every half second. Change the variable 
TimeBetweenReadings to change the rate.

See www.vernier.com/arduino for details.

*/
const int encoderPinCCW = 2;// this assumes you are using the BTD 1 connector
const int encoderPinCW = 3;
const int XR = 5; // this pin controls the resolution of the Rotary Motion Sensor
boolean highResOn = false;// this controls whether you are measuring in 
//high-res mode, to the nearest 1/4 degree. If not, it is to the nearest degree
float res; //angle change for each transition (1 or 0.25 degrees)
int readingNumber=0;
volatile int encoderPos = 0; // variables changed within interrupts are volatile

void setup()
{
  pinMode(encoderPinCCW, INPUT);
  pinMode(encoderPinCW, INPUT);
  pinMode (XR, OUTPUT);
  digitalWrite(XR,highResOn);
  digitalWrite(encoderPinCCW, HIGH);
  digitalWrite(encoderPinCW, HIGH);
  Serial.begin(9600);
  attachInterrupt(0, doEncoderCCW, RISING); // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(1, doEncoderCW, FALLING); // encoder pin on interrupt 1 (pin 3)
  if(highResOn) res = 0.25;
  else res =1;
  Serial.println("Vernier Format 2");
  Serial.println("Rotary Motion Readings taken using Ardunio");
  Serial.println("Data Set");
  Serial.print("Time");//long name
  Serial.print("\t"); //tab character
  Serial.println ("Angle"); //long name
  Serial.print("t");//short name
  Serial.print("\t"); //tab character
  Serial.println ("A"); //short name
  Serial.print("seconds");
  Serial.print("\t"); // tab character
  Serial.println ("degrees"); 
 }
 
void loop()
{
  float readingTime;
  float pos, oldPos;
  int timeBetweenReadings = 500; // in ms
  uint8_t oldSREG = SREG;
  cli();
  pos = encoderPos*res;
  SREG = oldSREG;
  readingTime = (readingNumber/1000.0*timeBetweenReadings);
  Serial.print(readingTime); // print time in seconds
  Serial.print("\t"); //tab character
  Serial.println(encoderPos*res);   // display angle in degrees                               
  delay(timeBetweenReadings); // Delay a bit...
  readingNumber++;   
}

void doEncoderCCW()
{
    encoderPos++;    // count UP
}
void doEncoderCW()
{
    encoderPos--;    // count DOWN 
}     
    
