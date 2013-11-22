/*VernierLaserTracker  (v 2013.11)
Monitors the position of an object using a Vernier Motion Detector
and then aims a laser pointer mounted on a servo motor at the object.

Because of the use of a trig function (arctangent) in the calculations, 
this sketch requires the math.h library. It also requires the servo library.

See www.vernier.com/arduino for more information.
 */
#include <math.h>
#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 
long time;// clock reading in microseconds
long Duration; // time it take echo to return
const int SpeedOfSound = 344; //in m/s
double Distance;// in centimeters
int val = 0;
const int TriggerPin = 4; //trigger pin
const int EchoPin = 2;// echo pin
int Range=100; //distance in cm from Laser Pointer/Servo motor to Motion Detector
void setup() 
{
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  // initialize the Ping pin as an output:
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT); //this is the pin that goes high when an echo is received
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println(" ");
  Serial.print("Distance");
  Serial.print("\t"); //tab character
  Serial.print ("Angle"); 
    Serial.print("\t"); //tab character
  Serial.println ("Angle"); 
  Serial.print("centimeters");
  Serial.print("\t"); // tab character
  Serial.print ("radians"); 
  Serial.print("\t"); // tab character
  Serial.println ("degrees"); 
}
void loop() 
{
  float ArcTan=0;
  double Angle =0;
  float Degrees;
  int ServoSetting;
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
  Serial.print(Distance);
  Serial.print("\t"); // tab character
  ArcTan = atan(Distance/Range); 
  Serial.print(ArcTan);
  Serial.print("\t"); // tab character
  Degrees= ArcTan*57.29578; //convert radians to degrees
  Serial.println(Degrees);
  ServoSetting =Degrees;
  myservo.write(ServoSetting); // sets the servo position according to the scaled value                           
  delay(50); //delay a second
}



