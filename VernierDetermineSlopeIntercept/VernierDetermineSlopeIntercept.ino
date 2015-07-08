/*
VernierDetermineSlopeIntercept (v 2015.02)
Reads a Vernier analog (BTA) Sensor connected to pin A0 of the Arduino. 
This sketch displays the time and digital readout from the sensor 
readings on the Serial Monitor. 
As written, the readings will be displayed every half second. 
Change the variable TimeBetweenReadings to change the rate.

 See www.vernier.com/arduino for more information.
*/
/*
////////////////////////////////////////
The data from this sketch will allow you to determine the slope and
intercept of the sensor - assuming a linear relationship. Once this 
is determined another sketch called "VernierAnalog Sensor" can be used
to read values of the sensor.
//See www.vernier.com/products/sensors.
*/
int TimeBetweenReadings = 500; // in ms
float Count;
float Voltage;
float SensorReading;
int ReadingNumber=0;
float Time;
/////////////////////////////////////////

void setup() 
{
  Serial.begin(9600); //initialize serial communication at 9600 baud
  Serial.println("Vernier Format Special");
  Serial.println("Readings taken using Ardunio to determine slope and intercept");
  Serial.println("Data Set");
  Serial.print("Time");//long name
  Serial.print("\t"); //tab character
  Serial.print("(s)");//units
  Serial.print("\t");
  Serial.print("Count");
  Serial.print("\t");
  Serial.print("Voltage");
  Serial.print("\t");
  Serial.println("(V)"); 
}
void loop() 
{

//the print below does the division first to avoid overflows
  Serial.print("\t");
  Serial.print(ReadingNumber/1000.0*TimeBetweenReadings); 
  Serial.print("\t");
  Count = analogRead(A0);
  Voltage = Count / 1023 * 5.0;// convert from count to raw voltage
  Serial.print (Count);
  Serial.print ("\t");
  Serial.println (Voltage);
  delay(TimeBetweenReadings);// delay in between reads for stability
  ReadingNumber++;
}

