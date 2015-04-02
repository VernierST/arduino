/*
VernierThermistorDisplay (v 2013.12)
Reads the temperature from a Vernier Stainless Steel Temperature Probe (TMP-BTA)
or Surface Temperature Sensor (STS-BTA) connected to the BTA connector. 
As written, the readings will be displayed every half second. Change the variable 
TimeBetweenReadings to change the rate.

We use the Steinhart-Hart equation (in the function Thermistor) to determine temperature 
from the raw A/D converter reading. Because of the use of log functions, in the 
Steinhart-Hart equation, this sketch requires the math.h library. 

See www.vernier.com/engineering/stem/sensors/temperature-sensor/
for more information on how thermistors are read.

This sketch uses a 4-character, 7-Segment Serial Display to temperature
readings in Celsius. The code for controlling the display was based on
the Serial 7-Segment Display Example Code, Serial Mode Stopwatch by Jim Lindblom
of SparkFun Electronics

The print function is used with the SoftwareSerial library
to send display data to the S7S.
Here is the wiring for display with a UART serial connection:
   Arduino -------------- Serial 7-Segment
     5V   --------------------  VCC
     GND  --------------------  GND
     13   --------------------  RX
 */
#include <math.h>
#include <SoftwareSerial.h>
// These are the Arduino pins required to create a software seiral
//  instance. We'll actually only use the TX pin.
const int softwareTx = 9;
const int softwareRx = 7;
SoftwareSerial s7s(softwareRx, softwareTx);
char tempString[10];  // Will be used with sprintf to create strings	
int ThermistorPIN =0;// Analog Pin 0
float Time;
int Count; //reading from the A/D converter (10-bit)
float Temp;
unsigned int Temp100;
int TimeBetweenReadings = 500; // in ms
int ReadingNumber=0;
void setup()
{
  s7s.begin(9600);
  // Clear the display, and then turn on all segments and decimals
  clearDisplay();  // Clears display, resets cursor
  setDecimals(0b000000);  // Turn on all decimals, colon, apos
  s7s.print("DEG.");  // Displays Deg and then Cels on display
  delay (2000);
  s7s.print("CELS"); 
  setBrightness(127);  // Medium brightness
  delay(1500);
  setBrightness(255);  // High brightness
  delay(1500);
  // Clear the display before jumping into loop
  clearDisplay();  
}

void loop()
{
  Count=analogRead(ThermistorPIN);       // read count from the A/D converter 
  Temp=Thermistor(Count);       // and  convert it to Celsius
  Temp100 =Temp*100;
  // Magical sprintf creates a string for us to send to the s7s.
  //  The %4d option creates a 4-digit integer.
  sprintf(tempString, "%4d", Temp100);
  // This will output the tempString to the S7S
  s7s.print(tempString);
  setDecimals(0b0000010);  // Sets digit 3 decimal on
  delay(TimeBetweenReadings);  // This will make the display update at about every half second
}

void clearDisplay()
{
  s7s.write(0x76);  // Clear display command
}

// Set the displays brightness. Should receive byte with the value
//  to set the brightness to
//  dimmest------------->brightest
//     0--------127--------255

void setBrightness(byte value)
{
  s7s.write(0x7A);  // Set brightness command byte
  s7s.write(value);  // brightness data byte
}

// Turn on any, none, or all of the decimals.
//  The six lowest bits in the decimals parameter sets a decimal 
//  (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
//  [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1)
void setDecimals(byte decimals)
{
  s7s.write(0x77);
  s7s.write(decimals);
}

float Thermistor(int Raw) //This function calculates temperature from ADC count
{
 /* Inputs ADC count from Thermistor and outputs Temperature in Celsius
 *  requires: include <math.h>
 * There is a huge amount of information on the web about using thermistors with the Arduino.
 * Here we are concerned about using the Vernier Stainless Steel Temperature Probe TMP-BTA and the 
 * Vernier Surface Temperature Probe STS-BTA, but the general principles are easy to extend to other
 * thermistors.
 * This version utilizes the Steinhart-Hart Thermistor Equation:
 *    Temperature in Kelvin = 1 / {A + B[ln(R)] + C[ln(R)]3}
 *   for the themistor in the Vernier TMP-BTA probe:
 *    A =0.00102119 , B = 0.000222468 and C = 1.33342E-7
 *    Using these values should get agreement within 1 degree C to the same probe used with one
 *    of the Vernier interfaces
 * 
 * Schematic:
 *   [Ground] -- [thermistor] -------- | -- [15,000 ohm bridge resistor] --[Vcc (5v)]
 *                                     |
 *                                Analog Pin 0
 *
 For the circuit above:
 * Resistance = ( Count*RawADC /(1024-Count))
 */
 long Resistance; 
 float Resistor = 15000; //brige resistor
// the measured resistance of your particular bridge resistor in
//the Vernier BTA-ELV this is a precision 15K resisitor 
  float Temp;  // Dual-Purpose variable to save space.
  Resistance=( Resistor*Raw /(1024-Raw)); 
  Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  Temp = 1 / (0.00102119 + (0.000222468 * Temp) + (0.000000133342 * Temp * Temp * Temp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
  return Temp;                                      // Return the Temperature
}







