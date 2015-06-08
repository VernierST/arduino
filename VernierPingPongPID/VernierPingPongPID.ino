/*
VernierMotionDetectorPID (v 2015.05)
Takes data from a Vernier Motion Detector connected to Digital 1 connector. 
Uses PID to control fan to elevate ping pong ball to target height. The ball is in
plastic tube 0.6 m tall with a fan at the bottom and a motion detector at the top. 
This version uses a Digital Control Unit (DCU) connected to the Digital 2 port to 
control the fan.

Motion detector portion of this sketch is taken from VernierMotionDetector (v 2014.09) 
It measures the time taken for the ultrasound to return (in microseconds)
and then calculates the corresponding distance (based on the speed of ultrasound
in air) and displays the distance (in cm) on the Serial Monitor. 

Here is how the Vernier Motion Detector works:
- when pin 2 on BTD is pulled high, this triggers the ultrasound pulse
- the program then starts timing but then delays 0.9 ms *(blanking time,
   0.9 seconds is the time it takes ultrasound to travel 15 cm twice (round trip))
- the program then monitors pin 1 on the BTD, waiting for it to go high. 
This happens when an echo is detected.

The PID control is modified from http://playground.arduino.cc/Code/PIDLibaryBasicExample 
and takes the calculated input from the motion detector ("distance") and controls analog PWM
output 6 through the DCU (line 1).You will need to download the PID library from 
http://playground.arduino.cc/Code/PIDLibrary.

As written, the reading will feed to the PID control continuously and data fed to
Serial Print.

This has been used with VPython to create a virtual display of the ping pong ball. 
Some notes in the sketch are there to aid ease of reading data into VPython.
  
See www.vernier.com/arduino for more information.
 */
#include <PID_v1.h>

//Define Variables we'll be connecting to
double Setpoint, Distance, Output;

//Specify the links and initial tuning parameters
PID myPID(&Distance, &Output, &Setpoint, 9,1.0,2.0, REVERSE);

const int TriggerPin = 3; //trigger pin
const int EchoPin = 2;// echo pin

void setup() 
{
  // initialize the Ping pin as an output:
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT); //this is the pin that goes high when an echo is received
    //initialize the variables we're linked to
    Setpoint = 30; // setpoint (cm from top)

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // The following can be grayed out for a VPython serial display
  Serial.println("Vernier Format 2");
  Serial.println("Motion Detector Readings taken using Ardunio");
  Serial.println("Data Set");
  Serial.print("Time for Echo");//long name
  Serial.print("\t"); //tab character
  Serial.print("Set Point");
  Serial.print("\t"); // tab character
  Serial.print ("Distance"); //long name
  Serial.print("\t");
  Serial.println ("Output from PID"); 
  Serial.print("delta t");//short name
  Serial.print("\t"); //tab character
  Serial.print("\t"); //tab character
  Serial.print("SP");
  Serial.print("\t"); // tab character
  Serial.print("\t");
  Serial.print ("D"); //short name
  Serial.print("\t");
  Serial.println("0 - 255");
  Serial.print("micro seconds");//units
  Serial.print("\t"); // tab character
  Serial.print ("centimeters"); //units
  Serial.print("\t"); // tab character
  Serial.print ("centimeters"); //units
  Serial.print("\t");
  Serial.println("#");
  // This would be the end of the grayed out portion if using VPython
}
void loop() 
{
  long time; // clock reading in microseconds
  long Duration; // time it take echo to return
  const float SpeedOfSound = 340; //in m/s
  int val = 0;
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(4000);
  digitalWrite(TriggerPin, HIGH); // start the ultrasound pulse
  time = micros(); //note time
  delayMicroseconds(900); //delay during the blanking time
  do
  {
    val =digitalRead(EchoPin);
    // if no echo, repeat loop and wait:
  }
  while (val == LOW) ;
  Duration =micros() - time;
  
  /* The speed of sound is 340 m/s.
  The ultrasound travels out and back, so to find the distance of the
  object we take half of the distance traveled.*/
  
  Distance= Duration *SpeedOfSound/2/10000 ;// note convert to cm
  
  myPID.Compute();
  analogWrite(6,Output);
  
  //* Grayed out for python connection
  Serial.print(Duration);// print the time it took until the echo
  Serial.print("\t"); // tab character
  Serial.print(Setpoint);
  Serial.print("\t"); // tab character
  // This would be the end of the grayed out portion
  Serial.println(Distance);
  Serial.print("\t"); //tab character
  Serial.println(Output);
  delay (100);
 
}

 


