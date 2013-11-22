/*
VernierAnalogSensorThreshold (v. 2013.11)
Reads a Vernier analog (BTA) sensor connnected to pin A0 of the Arduino. This
sketch displays the sensor reading on the Serial Monitor. As written, the 
reading will be displayed every half second. Change the variable 
TimeBetweenReadings to change the rate.

This has code added to turn on line D13 if the sensor reading exceeds a
threshold.

See www.vernier.com/arduino for more information.

*/
float Count;
float Voltage;
float SensorReading;
int TimeBetweenReadings = 500;
int ReadingNumber=0;
float Time;
float Threshold =100 ;// this is the limit you are setting (here we are using 100)
////////////////////////////////////////
// This is the information on the sensor being used. 
//See the www.vernier.com/products/sensors.
char Sensor[]="Hand Dynamometer";
float Intercept = -19.295;
float Slope = 175.416;
/////////////////////////////////////////
void setup() 
{
  Serial.begin(9600); //initialize serial communication at 9600 bits per second
  pinMode(13, OUTPUT); // This specifies that digital line D13 will be used for output (not input, which is the default)
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
  Serial.print("        ");
  Serial.println(SensorReading);
  delay(TimeBetweenReadings);// delay in between reads for stability
  if (SensorReading>Threshold){
    digitalWrite(13, HIGH);
    }// This turns on the LED 
  else 
    {
    digitalWrite(13,LOW);
    }
  ReadingNumber++;
}





