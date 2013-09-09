/*
VernierAnalogVoltage
Reads a Vernier analog (BTA) sensor connected to pin A0 of the Arduino. This
sketch displays the raw voltage reading on the Serial Monitor. As written, the 
reading will be displayed every half second. Change the variable TimeBetweenReadings
to change the rate.
  
See www.vernier.com/arduino for more information.
 */
float Count;
float Voltage;
float SensorReading;
int TimeBetweenReadings = 500;
int ReadingNumber=0;
float Time;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Time (s)    Voltage Readings from A0 (N)");
}
void loop() {
  Time = (ReadingNumber*TimeBetweenReadings);// in ms
  Serial.print(Time/1000); //display in seconds, not milliseconds
  Count = analogRead(A0);
  Voltage = Count / 1024 * 5.0;// convert from count to raw voltage
  Serial.print("            ");
  Serial.println(Voltage);
  delay(TimeBetweenReadings);// delay in between reads for stability
  ReadingNumber++;
}
