/*
VernierAnalogStore (v 2014.09)
Reads a Vernier analog (BTA) Sensor connected to pin A0 of the Arduino 
and and stores  the data in the non-volatile EEPROM memory of the Arduino.
This sketch displays the time and sensor readings on the Serial Monitor. 
As written, the readings will be taken every second. You can stop the
data collection by pressing the button (after three points are collected).

Change the variable TimeBetweenReadings to change the rate.
Change the variable NumberOfReadings to change the number of data points to take.
Change the info between //////// lines to change sensors.

See www.vernier.com/arduino for more information, especially for information 
on how this sketch can be used to collect remote data with the Arduino away 
from a computer.
 
Thanks to Brian Huang, for help on this program, especially the structure of a 
program to store and retrieve data.
*/
int buttonPin= 12; // analog input pin to use as a digital input
int ledPin1= 13; // digital output pin for LED 1 indicator
int debounce= 20; // ms debounce period to prevent flickering when pressing or releasing the button
int holdTime= 2000; // ms hold period: how long to wait for press+hold event
#include <EEPROM.h>
int base = 0;  // the base address in EEPROM for data storage
// note first two bytes store the number of points collected
// Button variables
int buttonVal = 0; // value read from button
int buttonLast = 1; // buffered value of the button's previous state
long btnDnTime; // time the button was pressed down
long btnUpTime; // time the button was released
long ReadingTime; //time at which a reading was taken
long ElapsedTime; //time since last reading
boolean ignoreUp = false; // whether to ignore the button release because the click+hold was triggered
boolean ledVal1 = false; // state of LED 1
int AnalogDataPin = A0; // this may be changed depending on circuit wiring
int Count;
float Voltage;
float SensorReading;
unsigned long Time;
long unsigned int TimeBetweenReadings = 1000; //in ms
int NumberOfPoints= 511;
///////////////////////////////////
String SensorName = "Dual-Range Force Sensor, 10 N";
String Measurement = "Force";
String ShortMeasurement = "F"; // this is a shortened version of the label
String Units = "Newtons";
float Intercept = 12.25;
float Slope = -4.9;
///////////////////////////////////

void setup()
{
  // Set button input pin
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(AnalogDataPin, INPUT);
  pinMode(ledPin1, OUTPUT);// Set LED output pin
  Serial.begin(9600);
  Serial.println("VernierAnalogStore sketch");
  Serial.println("Press and hold to collect new data");
  Serial.println("Press button briefly to read data stored in EEPROM");
  Serial.println("and send it to the Serial Monitor");
  Serial.println(" ");
}

void loop()
{
  digitalWrite(ledPin1, false);
  // Read the state of the button
  buttonVal = digitalRead(buttonPin);// button down is low
  // Test for button pressed and store the down time
  if (buttonVal == LOW && buttonLast == HIGH && (millis() - btnUpTime) > long(debounce))
    { 
      //button pressed
       btnDnTime = millis();// note time of press
     }
  // Test for button release and store the up time
  if (buttonVal == HIGH && buttonLast == LOW && (millis() - btnDnTime) > long(debounce))
    {
       if (ignoreUp == false) ReadEEPROMData();// read data from EEPROM and send to Serial Monitor
       else ignoreUp = false;
       btnUpTime = millis();// note time of button release
     }
  // Test for button held down for longer than the hold time
  if (buttonVal == LOW && (millis() - btnDnTime) > long(holdTime))
    {
      CollectData(); //collect new data
      ignoreUp = true;
      btnDnTime = millis(); // get new button down time
    }
  buttonLast = buttonVal;
}

//=================================================
// Events to trigger by short click of button
void ReadEEPROMData()// //Send data to Serial Monitor
{
  ledVal1 = !ledVal1;
  digitalWrite(ledPin1, true);
  int NumberOfPointsRead;// local variable
  Serial.println("Send Data to Serial Monitor");
  Serial.println(" ");
  int Sample = 0;// local variables
  int i = 0; 
  int addr = base+ 2; //starting address of memory for data storage
  //note addr is laterincremented
  NumberOfPointsRead = EEPROM.read(base) + EEPROM.read(base+ 1)*256;
  Serial.println( "Vernier Format 2");
  Serial.print(NumberOfPointsRead);
  Serial.print(" readings taken with a Vernier ");
  Serial.println(SensorName);
  Serial.println("Data Set From Arduino");
  Serial.print("Time");//long name
  Serial.print("\t");                // print a tab character  
  Serial.println(Measurement);  
  Serial.print("T");//short name
  Serial.print("\t");                // print a tab character  
  Serial.println(ShortMeasurement);  //short name
  Serial.print("s");//units
  Serial.print("\t");                // print a tab character 
  Serial.println(Units);  
  for (i = 0; i <(NumberOfPointsRead)*2; i+=2)
   {
      Count = EEPROM.read(base+2 + i) | EEPROM.read(base+2 + i + 1) << 8;   
      //the print below does the division first to avoid overflows
      Serial.print(i/2/1000.0*TimeBetweenReadings);  
      Serial.print("\t");                // print a tab character 
      Voltage = Count  * (5.0/ 1023);
      SensorReading= Intercept + Voltage * Slope;
      Serial.println(SensorReading);
   }
  Serial.println(" ");
  Serial.println("Press and hold button to collect new data ");
  Serial.println(" ");
  digitalWrite(ledPin1, false); //turn off LED
  //end of send data to Serial Monitor
}

//=================================================
// Events to trigger by long click of button
void CollectData() //Collect Data
{
  ledVal1 = !ledVal1;
  Serial.begin(9600);
  Serial.println("Collecting Data...");
  Serial.println(" ");
  int Sample = 0;// local variable
  int addr = base+ 2; //starting address of memory for data storage
  //note addr is later incremented
  Serial.println( "Vernier Format 2");
  Serial.print("Data taken with a Vernier ");
  Serial.println(SensorName);
  Serial.println("Data Set From Arduino");
  Serial.print("Reading");
  Serial.print("\t"); 
  Serial.print("Time");
  Serial.print("\t");                // print a tab character  
  Serial.println(Measurement);    
  Serial.print("n");
  Serial.print("\t");
  Serial.print("T");
  Serial.print("\t");                // print a tab character  
  Serial.println(ShortMeasurement);  
  Serial.print("#");
  Serial.print("\t"); 
  Serial.print("s");                // print a tab character  
  Serial.print("\t"); 
  Serial.println(Units);
  
  Sample=0;
  do
  {
     ReadingTime = millis();// note time of reading
     digitalWrite(ledPin1, true);
     Serial.print(Sample);
     Serial.print("\t");
     //the print below does the division first to avoid overflows
     Serial.print(Sample/1000.0*TimeBetweenReadings); 
     Serial.print("\t");
     Count= analogRead(AnalogDataPin);
     Voltage = Count * (5.0/ 1023);
     SensorReading = Intercept + Voltage * Slope;
     Serial.println(SensorReading);
     EEPROM.write(addr, lowByte(Count));
     EEPROM.write(addr + 1, highByte(Count));
     addr +=2;  //increment address pointer twice 
     delay (300);//blink to the LED
     digitalWrite(ledPin1, false);
     do
       {
         buttonVal = digitalRead(buttonPin);// check button status
         ElapsedTime=millis()-ReadingTime;
       }
         while ((buttonVal==HIGH || Sample< 3) && (ElapsedTime<TimeBetweenReadings));
   Sample++ ;
   } 
  while ((buttonVal==HIGH || Sample< 3) && Sample < NumberOfPoints); 
  for (int i=1; i<4;i++)
    {//flash LED to indicate stop
      digitalWrite(ledPin1, true);
      delay (100);
      digitalWrite(ledPin1, false);
      delay (100);
    }
  //the number of points is stored at base and base+1;
  //data starts at the next two bytes (base=2 and base+3, etc);
  EEPROM.write(base+ 0, lowByte(Sample));
  EEPROM.write(base + 1, highByte(Sample));
  Serial.println(" ");
  Serial.println("Press button briefly to send data to Serial Monitor");
  Serial.println("Press and hold to collect new data");
  Serial.println(" ");
  digitalWrite(ledPin1, false);
};//end of Data Collect

