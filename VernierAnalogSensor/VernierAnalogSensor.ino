/*
VernierAnalogSensor (v 2013.11)
Reads a Vernier analog (BTA) Sensor connected to pin A0 of the Arduino. 
This sketch displays the time and sensor readings on the Serial Monitor. 
As written, the readings will be displayed every half second. 
Change the variable TimeBetweenReadings to change the rate.

 See www.vernier.com/arduino for more information.
*/
float Count;
float Voltage;
float SensorReading;
int TimeBetweenReadings = 500; // in ms
int ReadingNumber=0;
float Time;
////////////////////////////////////////
// This is the information on the sensor being used. 
//See the www.vernier.com/products/sensors.
char Sensor[]="Hand Dynamometer";
float Intercept = -19.295;
float Slope = 175.416;
/////////////////////////////////////////
void setup() 
{
  Serial.begin(9600); //initialize serial communication at 9600 baud
  Serial.println("Vernier Format 2");
  Serial.print(Sensor);
  Serial.print(" ");
  Serial.println("Readings taken using Ardunio");
  Serial.println("Data Set");
  Serial.print("Time");
  Serial.print("\t"); //tab character
  Serial.println ("Force"); //change to match sensor
  Serial.print("seconds");
  Serial.print("\t"); // tab character
  Serial.println ("newtons"); //change to match sensor
}
void loop() 
{
  //the print below does the division first to avoid overflows
  Serial.print(ReadingNumber/1000.0*TimeBetweenReadings); 
  Count = analogRead(A0);
  Voltage = Count / 1024 * 5.0;// convert from count to raw voltage
  SensorReading= Intercept + Voltage * Slope;
  Serial.print("\t"); // tab character
  Serial.println(SensorReading);
  delay(TimeBetweenReadings);// delay in between reads for stability
  ReadingNumber++;
}

