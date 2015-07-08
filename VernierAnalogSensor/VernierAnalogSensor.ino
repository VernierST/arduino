/*
VernierAnalogSensor (v 2014.09)
Reads a Vernier analog (BTA) Sensor connected to pin A0 of the Arduino. 
This sketch displays the time and sensor readings on the Serial Monitor. 
As written, the readings will be displayed every half second. 
Change the variable TimeBetweenReadings to change the rate.

 See www.vernier.com/arduino for more information.
*/

////////////////////////////////////////
// This is the information on the sensor being used. 
//See the www.vernier.com/products/sensors.
char Sensor[]="Hand Dynamometer";
float Intercept = -19.295;
float Slope = 175.416;
int TimeBetweenReadings = 500; // in ms
int ReadingNumber=0;

/////////////////////////////////////////
void setup() 
{
  Serial.begin(9600); //initialize serial communication at 9600 baud
  Serial.println("Vernier Format 2");
  Serial.print(Sensor);
  Serial.print(" ");
  Serial.println("Readings taken using Ardunio");
  Serial.println("Data Set");
  Serial.print("Time");//long name
  Serial.print("\t"); //tab character
  Serial.println ("Force"); //change to match sensor
  Serial.print("t");//short name
  Serial.print("\t"); //tab character
  Serial.println ("F"); //short name, change to match sensor 
  Serial.print("seconds");//units
  Serial.print("\t"); // tab character
  Serial.println ("newtons"); //change to match sensor
}
void loop() 
{
float Time;

//the print below does the division first to avoid overflows
  Serial.print(ReadingNumber/1000.0*TimeBetweenReadings); 
  float Count = analogRead(A0);
  float Voltage = Count / 1023 * 5.0;// convert from count to raw voltage
  float SensorReading= Intercept + Voltage * Slope; //converts voltage to sensor reading
  Serial.print("\t"); // tab character
  Serial.println(SensorReading);
  delay(TimeBetweenReadings);// delay in between reads for stability
  ReadingNumber++;
}

