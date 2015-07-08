/*
VernierBNC (v 201.06)
Reads a Vernier analog (BTA) Ion Selective Electrode (ISE) sensor connected to pin A0 of the Arduino. 
The ISE probes can be tricky to use with the Arduino because the sensor must be 
calibrated before each use and the probe response is not linear, but rather is 
modeled by the Nernst Equation.
The Arduino provides a 10-bit readout (0-1023) from the electrode. The sketch 
maps this to a 0 - 5 V range. The sketch then takes this raw voltage and converts 
that to mV using the slope and intercept associated with the electrode: 

E = 137.55*Voltage -0.1682. 

In order to determine the concentration the mV response must be entered into 
the Nernst equation: 

E = Eo + m(ln C)

The trick with these probes and the Nernst equation is that the sensor is in a 
constant state of deterioration. Every 2 weeks or so the membrane has changed 
enough to require a new calibration. In other words, the values Eo and m have changed.

Determine the values of Eo and m by using 2 known concentration solutions and 
measuring Eo. You can only expect these values to remain valid for a short period 
of time (a week or two) and then you will need to do this again. Enter these values in the sketch.

This sketch displays the raw voltage, the electrode response, and the concentration.
The concentration will be accurate only if the values of Eo and m are calibrated for the
specific sensor you are using. As written, the reading will be displayed every half second
on the serial monitor. Change the variable TimeBetweenReadings to change the rate.
  
See www.vernier.com/arduino for more information.
 */
#include <math.h>
int TimeBetweenReadings = 500;
float Eo = 252.72; //Enter the values from your calibration here
float m = -7.59; // Enter the values from your calibration here
int ReadingNumber=0;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Time (s)    Voltage(V)     Electrode(mV)     Concentration (mg/l)");
  
}
void loop() {
float Count;
float Voltage;
float SensorReading;
float ElectrodeReading;
float Concentration;

float Time;  //the print below does the division first to avoid overflows
  Serial.print(ReadingNumber/1000.0*TimeBetweenReadings); 
  Count = analogRead(A0);
  Voltage = Count / 1023 * 5.0;  // convert from count to raw voltage
  ElectrodeReading = 137.55*Voltage -0.1682; // converts raw voltage to mV from electrode
  double(val) = ((ElectrodeReading-Eo)/m); // calculates the value to be entered into exp func.
  Concentration = exp(val); // converts mV value to concentration
  Serial.print("\t");
  Serial.print("\t");
  Serial.print(Voltage);
  Serial.print("\t");
  Serial.print("\t");  
  Serial.print(ElectrodeReading);
  Serial.print("\t");
  Serial.print("\t");
  Serial.println(Concentration);
  delay(TimeBetweenReadings);// delay in between reads for stability
  ReadingNumber++;
}
