/*
VernierISE (v 2017.09)
Reads a Vernier analog (BTA) Ion Selective Electrode (ISE) sensor connected to Analog 1 
port of the Vernier Sensor. 
The ISE probes can be tricky to use with the Arduino because the sensor must be 
calibrated before each use and the probe response is not linear, but rather is 
modeled by the Nernst Equation.
The Arduino provides a 10-bit readout (0-1023) from the electrode. The sketch 
uses the Vernier Library to read the sensor with units of mV using this standard calibration for the
electrode. 

E = 137.55*Voltage -0.1682. (units of mV)

In order to determine the concentration in mg/l the mV response must be entered into 
the Nernst equation: 

E = Eo + m(ln C)

The trick with these probes and the Nernst equation is that the sensor membrane changes with 
time. It is likely that each time you go to use the sensor it will have changed 
enough to require a new calibration. 

If this is important to your application, determine the values of Eo and m by using 2 known 
concentration solutions and measure the corresponding Eo. You can only expect these values 
to remain valid for a short period of time and then you will need to do this again. 
Enter these values in the sketch.

This sketch displays the the electrode response (in mV), and the concentration (in mg/l)
The concentration will be accurate only if the values of Eo and m are calibrated for the
specific sensor you are using. As written, the reading will be displayed every half second
on the serial monitor. Change the variable TimeBetweenReadings to change the rate.
  
See www.vernier.com/arduino for more information.
 */
#include "VernierLib.h" 
VernierLib Vernier;
float sensorReading;
float Eo = 252.72; //Enter the values from your calibration here
float m = -7.59; // Enter the values from your calibration here
void setup()
{
  Serial.begin(9600);
  Vernier.autoID();// this is the routine to do the autoID
  printSensorInfo();
}

void loop()
{
  sensorReading =Vernier.readSensor();
  double(val) = ((sensorReading-Eo)/m); // calculates the value to be entered into exp func.
  float concentration = exp(val); // converts mV value to concentration
  Serial.print(sensorReading);
  Serial.print(" ");
  Serial.print(Vernier.sensorUnits());
  Serial.print("\t");
  Serial.print(concentration);
  Serial.println("   mg/l");
  delay(500);//half a second
} 
 void printSensorInfo()
 {
   // print out information about the sensor found:
   Serial.println("Sensor Information:"); 
   Serial.print("Sensor ID number: "); 
   Serial.print("\t");
   Serial.println(Vernier.sensorNumber());
   Serial.print("Sensor Name: ");
   Serial.print("\t");
   Serial.println(Vernier.sensorName());
   Serial.print("Short Name: "); 
   Serial.print("\t");
   Serial.println(Vernier.shortName()); 
   Serial.print("Units: "); 
   Serial.print("\t");
   Serial.println(Vernier.sensorUnits());
   Serial.print("ID voltage level: "); 
   Serial.print("\t");
   Serial.println(Vernier.voltageID());
   Serial.print("Page: ");  
   Serial.print("\t");
   Serial.println(Vernier.page());
   Serial.print("slope: ");
   Serial.print("\t");
   Serial.println(Vernier.slope());
   Serial.print("intercept: ");
   Serial.print("\t");
   Serial.println(Vernier.intercept());
   Serial.print("cFactor:");
   Serial.print("\t");
   Serial.println(Vernier.cFactor());
   Serial.print("calEquationType: ");
   Serial.print("\t");
   Serial.println(Vernier.calEquationType());
 }
