/*
VernierWindOptServo (v 2015.04)
This is a proof of concept sketch and is based on the Vernier Servo 
sketch. The physical connection with a KidWind turbine for testing 
will require some additional engineering.

Reads current (A0) and voltage (A2) from a Vernier Energy Sensor while adjusting 
the pitch of blades on a KidWind Turbine in 5 degree increments from 10 
to 80 degrees. Serial printer posts time, servo angle, current, voltage and power. 
Sketch holds angle for 10 seconds before advancing to next setting. 
Data from this sketch can be used to determine optimum pitch for various wind
conditions. 

The servo motor must be connected to pin D9 of the Arduino. You can also 
connect to this line using the BTD 2 connector. This is an exception to 
our convention of reserving the BTD 2 connector for the digital readout. 
  
Note that if you have wired the BTD2 connector as we recommend on 
www.vernier.com/arduino or if you are using a SparkFun Vernier Interface Shield,
you can make the connection to proper line for the servo motor from DIO4 (pin 9) of the
BTD2 connector (the pin closest to the tab). 

If you connect the servo motor (3-pin) connector on a DCU and connect the DCU to
the BTD2 connector on the Arduino Interface Shield, this sketch will work. The 
DCU will require an external power supply.
*/

#include <Servo.h>
Servo myservo;  // create servo object to control a servo 
int CRaw = 0;  // pin used to connect the current sensor
int VRaw = 2; // pin used to connect voltage sensor
/* 
   Following information on the sensors being used. 
   See the www.vernier.com/products/sensors.
*/
float CIntercept = 1055.25;
float CSlope = -422.09;
float VIntercept = 31.315;
float VSlope = -12.526;

int TimeBetweenReadings = 500; // in ms
int ReadingNumber = 0;
int Angle = (10); // initial pitch in degrees 
int i = 0; //integer tracks time to change pitch
int val;

void setup()
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 

/* the 6 lines below are only needed if you plan to use this sketch with a servo motor
connected to the 3-pin connector on a DCU. The reason this is required is that the DCU 
has internal electronics to only allow its 4th pin to be turned on when the other first
three lines are off.
*/

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  myservo.write(Angle*255/180);
  
  Serial.begin(9600); //initialize serial communication at 9600 baud
  Serial.println("Vernier Format");
  
  Serial.println("Readings taken using Ardunio");
  Serial.println("Data Set");
  Serial.print("Time (s)");//long name
  Serial.print("\t"); //tab character
  Serial.print ("Pitch (degrees)");
  Serial.print("\t"); 
  Serial.print("Current (mA)");
  Serial.print("\t"); 
  Serial.print("Voltage (V)");
  Serial.print("\t");
  Serial.println ("Power (mW)");

}
void loop() 
{ 
 
 
  Serial.print(ReadingNumber/1000.0*TimeBetweenReadings); //division first to avoid overflows
  float CCount = analogRead(CRaw);
  float CVoltage = CCount / 1023 * 5.0;// convert from count to raw voltage
  float SensorCurrent = CIntercept + CVoltage * CSlope; //converts raw voltage to sensor reading
  float VCount = analogRead (VRaw);
  float VVoltage = VCount / 1023 * 5.0;// convert from count to raw voltage
  float SensorVoltage = VIntercept + VVoltage * VSlope; //converts raw voltage to sensor reading
  float Power = SensorCurrent * SensorVoltage; //calculates power in mW
  Serial.print("\t"); // tab character
  Serial.print("\t");
  Serial.print (Angle); // in degrees
  Serial.print ("\t");
  Serial.print("\t");
  Serial.print(SensorCurrent);
  Serial.print ("\t");
  Serial.print("\t");
  Serial.print (SensorVoltage);
  Serial.print ("\t");
  Serial.print("\t");
  Serial.println (Power);
  delay(TimeBetweenReadings);// delay in between reads for stability
  ReadingNumber++;
  
  
  i = ReadingNumber % 20;
    if (i == 0)
  {
    Angle = Angle + 5; //Move servo in 5 degree increments
    if (Angle == 85)
    {
      Angle = (10);
    }
    val = map(Angle, 0, 179, 0, 255);     // scale it to use it with the servo (value between 0 and 180) 
   myservo.write(val);                  // sets the servo position according to the scaled value 
   delay(15);                           // waits for the servo to move. Introduces slight time error. 
  }
  
} 
