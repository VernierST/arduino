/*
VernierFreq_Motion (v 2015.1)

This sketch produces a tone on a speaker connected to pin D9 of
the Arduino. The frequency of the tone will vary depending on how
far from the motion detector (connected to Digital 1 of the Vernier 
Shield) an object is place. It was designed to be used as an 
audible aid for an Engineering Design Challenge at the Museum of 
Science (Boston). It is based on the Tone 2 tutorial.
(see http://arduino.cc/en/Tutorial/Tone2).

If you want to change the range of frequencies, change the last two
numbers in the line:  
int thisPitch = map(sensorReading, 200, 15, 120, 1500)
They are the minimum and maximum frequencies produced.
If you want to change the sensitiviy of the system, change the first
two numbers of the same line. They are the range of expected readings 
from the sensor.

See www.vernier.com/arduino for details.
*/
int OutputPin=9;
long time;// clock reading in microseconds
long Duration; // time it take echo to return
const int SpeedOfSound = 344; //in m/s
double Distance;// in centimeters
int val = 0;
const int TriggerPin = 3; //trigger pin
const int EchoPin = 2;// echo pin
int Range=200; //distance in cm from Laser Pointer/Servo motor to Motion Detector
void setup() 
{
     // initialize the Ping pin as an output:
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT); //this is the pin that goes high when an echo is received
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println(" ");
  Serial.print("Distance");
  Serial.print("Frequency");
  Serial.println(" ");
}

void loop() {
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
  /* The speed of sound is 344 m/s.
  The ultrasound travels out and back, so to find the distance of the
  object we take half of the distance traveled.*/
  Distance= Duration *SpeedOfSound/2/10000 ;// note convert to cm
  Serial.println(Distance);
  
  delay(50); //delay a bit
  // read the sensor:
  float sensorReading = Distance;
  // print the sensor reading so you know its range

  /* map the distance input range (in this case, 200-15 (cm) from the motion detector)
  to the output pitch range (120 - 1500Hz).  I wanted the pitch to increase with height
  and the Motion Detector sits atop the air tube, so the "measured distance" is relative 
  to the top of the tube */
  int thisPitch = map(sensorReading, 200, 15, 120, 1500);
  Serial.print(sensorReading);
  Serial.print ("/t");
  Serial.print(thisPitch);
  // play the pitch:
  tone(OutputPin, thisPitch, 50);
  delay(50);        // delay in between reads for stability
}

