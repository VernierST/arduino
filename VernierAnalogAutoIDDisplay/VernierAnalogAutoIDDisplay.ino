/*
VernierAnalogAutoIDTwoLineDisplay (v 2014.12)
Reads the information to AutoID any Vernier BTA sensor connected to the BTA 1 connector
on the Vernier Interface Shield. it will also work with homemade, breadboard connections
if all the lines are connected so AutoID can work. This sketch handles sensors with 
memory chips and those using the I2C lines and those with resistor autoID. 

This sketch supports  (but does not require) the Two-Line Display,
sold by SparkFun as Serial Enabled 16x2 LCD display. See https://www.sparkfun.com/products/9395
The display should be connected as follows:
RX - Arduino Pin 9 (or pin 6 of the BTD 2 connector)
GND -Arduino GND Pin (or pin 5 of the BTD 2 connector)
+5V - Arduino +5V Pin (or pin 4 of the BTD 2 connector)

At startup, the program will list the Name, Units, and Calibration Page
for the sensor. After a two second delay, the Short Name and the senscor 
reading, with units will be shown on the display.

If the D12 button on the Vernier Interface Shield is held down as the program starts, the
Sensor ID Number, equation type, intercept, slope, and c factor for the sensor will be 
displayed briefly before the sensor readings appear.

This sketch also handles the two analog sensors that use the +/-10 Volt input line (VP-BTA and
30V-BTA). 

This version reads the A-to-D converter many times quickly and averages readings to average out
noise. You can change the amount of averaging by changing the integer, numberAveraged, in the loop.

Note that this sketch handles multiple pages of sensor calibrations.

The only BTA (analog) sensors that are not read properly are Ion-Selective Electrodes. For those,
mV output is displayed.

After the AutoID:
As written, this sketch displays the time and sensor readings on the Serial Monitor.
The readings will be displayed every second. 
Change the variable TimeBetweenReadings to change the rate.

When taking data with any sensor, if the D12 button on the Vernier Interface Shield is pressed, 
the display will show the raw voltage instead of the sensor reading. You will also notice that 
the LED will flicker (only on for a short time) while the voltage is being displayed.

See www.vernier.com/arduino for more information.
*/

int ledPin =13;//used to indicate data collection is going on
const int buttonPin=12; //button on Vernier Interface Shield
int buttonState = 0;//variable for reading the pushbutton status
int device=0x50; // for I2C commmunication
float VCC= 5.00;//"5 volt" power supply voltage used in resistor ID section and themistor calibration
int SensorNumber; //integer indicating sensor number
float VoltageID;//voltage read when checking for resistor-ID sensors
String Name="LongName_16_char";//initialized strings, LongName is 16 characters
String ShortName="ShortName12c";//ShortName is 12 characters
String Units = "Units_7";// , Units is 7 characters
float Intercept=0.000;// the three factors used for calibration:
float Slope=1.00;
float cfactor=0; //used only in complex calibrations
int Page;// page of memory where calibration is stored
int CalEquationType;
const int Channel=1; //always BTA Channel 1 in this script
char namestring[17];//character array for printing strings to display
char tempstring[16]; // character array used to create string for displaying numbers
/////////////////////////////////////////
int TimeBetweenReadings = 1000; // in ms (change this to control rate of data collection)
////////////////////////////////////////
#include <math.h> //library used in thermistor calibrations
#include <Wire.h>  //library used in digital autoID
#include <SoftwareSerial.h> //library used in printing to display
SoftwareSerial mySerial(3,9); //for display, pin 9 = TX, pin 3 = RX (unused)
byte sensordata [128];//use in reading digital autoID
byte floatbyte[5];// used in digital autoID
int ReadingNumber=0;

void setup() 
{
  Serial.begin(9600);// for printing to monitor
  mySerial.begin(9600); // for sending characters to display
  delay(500); // wait for display to boot up
  mySerial.write(124); // adjust backlight brightness of display
  mySerial.write(150); //max=157, 150=73%, 130=40%,128=off
  int muxlsb = 10; //low byte of multiplexer
  int muxmsb = 11; //high byte of multiplexer
  pinMode(muxlsb, OUTPUT); 
  pinMode(muxmsb, OUTPUT); 
  device=0x50; // used in digital autoID; for IC2 line
  digitalWrite(muxlsb, LOW); //set multiplexer for BTA1   
  digitalWrite(muxmsb, LOW);  
  pinMode(ledPin, OUTPUT); //LED on SparkFun Vernier Shield
  // Set up button input pin;
  pinMode(buttonPin, INPUT_PULLUP);
  //Read BTA1 Sensor 
  BTAResistorSensorID(Channel);// check for resistor AutoID
  if (SensorNumber==0) //if no resistor AutoID found. check for digital AutoID
        {
          Wire.begin(); //start I2C communication
          pinMode(A4, OUTPUT); //set up I2C lines
          pinMode(A5, OUTPUT); 
          DigitalSensorID(Channel);// check for digital AutoID
          pinMode(A4, INPUT); //turn off I2C lines
          pinMode(A5, INPUT); // if you do not turn these off, some analog readings are affected
        } 
   //Special handling for ISEs, CA, NH4, NO3, or Cl
     if  (SensorNumber>37 && SensorNumber<42) Units ="(mV*)";
          //Special calibration for Potasium ISE:
     if  (SensorNumber==113) Units ="(mV*)";
  PrintSensorInfo();// this line can be commented out if you do not need all this info.
  // The Print statements below form the heading of a table that can be easily  
  // transferred to the Vernier Logger Pro computer program for Mac or Windows.
  Serial.println(" ");    
  Serial.println("Vernier Format 2");
  Serial.println("Readings taken using Ardunio");
  Serial.println("Data Set");
  Serial.print("Time");
  Serial.print("\t"); //tab character
  Serial.println(Name); 
  Serial.print("t");
  Serial.print("\t"); //tab character
  Serial.println(ShortName);      
  Serial.print("seconds");
  Serial.print("\t"); //tab character
  Units.trim();
  Serial.println (Units);
  ///////////////////
  //send characters to 2-line display: (you could get rid of all of this if you do not have a 2-line display) 
  //Display Name, Units, and Page
  mySerial.write(254); // cursor to beginning of first line
  mySerial.write(128);
  mySerial.print("                    "); // clear display for next loop
  mySerial.print("                    ");
  mySerial.write(254); // cursor to beginning of first line (position 128)
  mySerial.write(128);
  Name.toCharArray(namestring,17);
  mySerial.print(namestring); // display name on first line
  mySerial.write(254); // cursor to beginning of second line (position 196)
  mySerial.write(196); 
  Units.toCharArray(namestring,17);
  mySerial.print(namestring);  // display units on second line
  mySerial.write(254);// cursor to the end of second line
  mySerial.write(204);
  mySerial.print("p=");// display page at end of second line (-1 if resistor ID)
  mySerial.print(Page); 
  delay(2000); // short delay
  //Special section to display equation type, slope, intercept, and c, if button is pressed:
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == LOW) 
   {
     digitalWrite(ledPin, HIGH);  
      //Display SensorNumber, cal equation type, and a (intercept):
     mySerial.print("                    "); // clear display for next loop
     mySerial.print("                    ");
     mySerial.write(254); // cursor to beginning of first line
     mySerial.write(128);
     mySerial.print("#"); 
     mySerial.print(SensorNumber); // Sensor ID number
     mySerial.write(254);// cursor to end of first line
     mySerial.write(134); 
     mySerial.print(" cal eq="); // // display cal eq type
     mySerial.print(CalEquationType); // // display page at end of second line   
     mySerial.write(254); // cursor to beginning of 2nd line
     mySerial.write(192);
     mySerial.print("a(int)="); // one space
     mySerial.print(Intercept); //     
     delay (2000);
     //Display b(slope), and c (cfactor):
     mySerial.print("                    "); // clear display for next loop
     mySerial.print("                    ");
     mySerial.write(254); // cursor to beginning of first line
     mySerial.write(128);
     mySerial.print("b(sl)="); // one space
     mySerial.print(Slope); //     
     mySerial.write(254); // cursor to beginning of second line
     mySerial.write(192);
     mySerial.print("c="); 
     mySerial.print(cfactor); //     
     delay (2000);
     digitalWrite(ledPin, LOW);  // turn LED off:
   }// end of special display done if button is down
  //set up screen for displaying data:
  mySerial.write(254); // cursor to beginning of first line
  mySerial.write(128);
  mySerial.print("                    "); // clear display for next loop
  mySerial.print("                    ");
  mySerial.write(254); // cursor to beginning of first line
  mySerial.write(128);
  ShortName.toCharArray(namestring,17);
  mySerial.print(namestring); // display Short Name
  mySerial.write(138);
  Units.toCharArray(namestring,17);
  mySerial.print(namestring); // display Units
 }

void loop() 
  {
    int Count; //reading from 0 to 5 volt input
    int AltCount ;//reading from -10 to +10V input
    float Voltage;
    float SensorReading=0.00;
    float Time;
    float sum = 0;
    float numberAveraged=100;//to average out noise, we take a lot of readings quickly and average, you can change this
    digitalWrite(ledPin, HIGH); //turn on LED
    Serial.print(ReadingNumber/1000.0*TimeBetweenReadings); 
    Serial.print("\t"); //tab character
    if (Name.startsWith("Voltage +/-"))// one of two sensors using the +/- 10 volt line
      {
         for (int i=0 ;i<numberAveraged;i++)
           {
             AltCount = analogRead(A1); //read  +/- 10 volt lin
             sum = sum+ AltCount;
           }
          Voltage = sum/numberAveraged / 1024.0 * 5.0 ;   // convert from count to raw voltage if using 10 volt range:      
      }
     else
      {        
         for (int i=0 ;i<numberAveraged;i++)
           {
             Count = analogRead(A0); //read 0 to 5 volt analog lines
               sum = sum+ Count;
           }
          Voltage =sum/numberAveraged/1024.0 * 5.0; ; //convert average count to voltage (0 to 5 volt input)
       }      
     SensorReading= Intercept + Voltage * Slope;//for all linear sensors
     //special calibration for thermistor temperture probe:
     if (SensorNumber==10) SensorReading=Thermistor(Count);//TMP or STS sensors
     //Special calibration for Wide Range Tempeature Sensor:
     if  (SensorNumber==78) SensorReading=Intercept+Voltage*Slope+cfactor*Voltage*Voltage;
     //Special calibration for Ethanol Sensor:
      if  (SensorNumber==97) SensorReading=Intercept*pow(Voltage,Slope);
     //Special calibration for Melt Station:
     if  (SensorNumber==92) SensorReading=Intercept+Voltage*Slope+cfactor*Voltage*Voltage;         
     //Special calibration for ISEs, CA, NH4, NO3, Cl:
     if  (SensorNumber>37 && SensorNumber<42) SensorReading=(137.55*Voltage-0.1682);
          //Special calibration for Potasium ISE:
     if  (SensorNumber==113) SensorReading=(137.55*Voltage-0.1682);
     mySerial.write(254); // command character
     mySerial.write(192); // move to line 2, position 0,
     mySerial.print("   "); // print some text
     buttonState = digitalRead(buttonPin);
     // check if the pushbutton is pressed.
     // if it is, the buttonState is HIGH:
     if (buttonState == LOW) // button down
       {
         digitalWrite(ledPin, LOW);  
         Serial.print("V= ");
         Serial.println(Voltage,1);   // display raw voltage on Serial Monitor
         mySerial.print("V= ");
         mySerial.print(Voltage); // print our float 
        }
    else
        {
         Serial.println(SensorReading);   // display Sensor Reading on monitor 
         mySerial.print(SensorReading);
        }  
     delay(TimeBetweenReadings/2);// delay half of period
     digitalWrite(ledPin, LOW);// LED on D13 flashes once per readng
     delay(TimeBetweenReadings/2);// delay the other half
     mySerial.write(254); // cursor to beginning of first line
     mySerial.write(192);
     mySerial.print("                "); // clear 2nd line of display for next loop 
     ReadingNumber++;
   }

 void BTAResistorSensorID(int Channel)
 {
  Page=-1; //assign to -1 so resistor ID sensors read this value
  delay (10);
  int CountID = analogRead(A5);
  VoltageID = CountID / 1024.0 * VCC;// convert from count to voltage 
  if (VoltageID>0.86 & VoltageID<0.95) SensorNumber = 1; //Thermocouple
  if (VoltageID>3.83 & VoltageID<3.86) SensorNumber = 2; // Voltage +/-10 V
  if (VoltageID>1.92 & VoltageID<2.13) SensorNumber = 3; // TI Current Probe (not used)
  if (VoltageID>1.18 & VoltageID<1.30) SensorNumber = 4; //Reistance 
  if (VoltageID>3.27 & VoltageID<3.68) SensorNumber = 5; //Extra-Long Temperature Probe
  if (VoltageID>4.64 & VoltageID<4.73) SensorNumber = 8; //Differential Voltage
  if (VoltageID>4.73 & VoltageID<4.83) SensorNumber = 9; //Current
  if (VoltageID>2.38 & VoltageID<2.63) SensorNumber = 10; //Stainless Steel or Surface Temperature Probe
  if (VoltageID>2.85 & VoltageID<3.15) SensorNumber = 11; // Voltage 30 V
  if (VoltageID>1.52 & VoltageID<1.68) SensorNumber = 12; //TILT, TI Light Sensor
  if (VoltageID>0.43 & VoltageID<0.48) SensorNumber = 13; //Exercise Heart Rate
  if (VoltageID>4.08 & VoltageID<4.16) SensorNumber = 14; //Raw Voltage
  if (VoltageID>0.62 & VoltageID<0.68) SensorNumber = 15; //EKG
  if (VoltageID>4.32 & VoltageID<4.40) SensorNumber = 17; //CO2
  if (VoltageID>4.50 & VoltageID<4.59) SensorNumber = 18; //Oxygen
  switch (SensorNumber) 
   {
      case 1:
            Name = "Thermocouple" ;  
            Units = "(Deg C)" ;  
            ShortName = "TC";
            Slope=-2.45455;
            Intercept=6.2115;
            Page = -1; //calibration storage p (1,2, or 3)
       break;
       case 2:
            Name = "Voltage +/- 10V" ;  //!!! do not change this name or you will mess up the code of the loop
            Units = "(V)" ;  
            ShortName = "Voltage10";
            Slope=4;//note correction for Sparkfun circuit done in calculation of Voltage!
            Intercept=-10;
            Page = -1; //calibration storage page 
            CalEquationType=1;
      break;
      case 3:
            Name = "Current" ;  
            Units = "(Amps)" ;  
            ShortName = "Current";
            Slope=-2.665;
            Intercept=6.325;
            Page = -1; //calibration storage page
            CalEquationType=1;
      break;
      case 4:
            Name = "Reistance" ;  
            Units = "(Ohms)" ;  
            ShortName = "Diff V";
            Slope=-2.5;
            Intercept=6.25;
            Page= -1; //calibration storage page
            CalEquationType=1;
      break;
      case 5:
            Name = "EL Temp" ;  //Extra-Long Temperature Probe
            Units = "(Deg C)" ;  
            ShortName = "EL T";
            Slope=58.341;
            Intercept=-53.073;
            Page= -1; //calibration storage page
            CalEquationType=1;
      break;
      case 8:
            Name = "Diff Voltage" ;  
            Units = "(V)" ;  
            ShortName = "Diff V";
            Slope=-2.5;
            Intercept=6.25;
            Page= -1; //calibration storage page 
            CalEquationType=1;
      break;
      case 9:
            Name = "Current" ;  
            Units = "(Amps)" ;  
            ShortName = "I";
            Slope=-0.25;
            Intercept=0.625;
            Page = -1; //calibration storage page
            CalEquationType=1;
      break;
      case 10:
            Name= "Temperature" ;  
            Units= "(Deg C)" ;  
            ShortName= "Temp";
            Slope=1;
            Intercept=0;
            Page = -1; //calibration storage page
            CalEquationType=12;//Steinhart-Hart
       break;
       case 11:
            Name = "Voltage +/- 30V" ;//!!! do not change this name or you will mess up the code of the loop
            Units = "(V)" ;  
            ShortName = "Volts";
            Slope=15.41;
            Intercept=-40.35;
            Page= -1; //calibration storage page
            CalEquationType=1;
        break;
        case 12:
            Name = "TI Light" ;  
            Units = "(relative)" ;  
            ShortName = "TI Light";
            Slope=1;
            Intercept=0;
            Page = -1; //calibration storage page
            CalEquationType=1;
         break;
         case 13:
            Name = "Exercise Heart Rate" ;  
            Units = "(V)" ;  
            ShortName = "Ex HR";
            Slope=1;
            Intercept=0;
            Page = -1; //calibration storage page
            CalEquationType=1;
         break;
         case 14:
            Name = "Raw Voltage" ;  
            Units = "(R V)" ;  
            ShortName = "Volts";
            Slope=1;
            Intercept=0;
            Page= 1; //calibration storage page
            CalEquationType=1;
         break;
         case 15:
            Name = "EKG" ;  
            Units = "(V)" ;  
            ShortName = "EKG";
            Slope=1;
            Intercept=0;
            Page = -1; //calibration storage page
            CalEquationType=1;
         break;
         case 17:
            Name = "Carbon Dioxide" ;  
            Name = "(ppm)" ;  
            ShortName = "CO2";
            Slope=1;
            Intercept=0;
            Page = -1; //calibration storage page
            CalEquationType=1;
         break;
         case 18:
            Name = "Oxygen" ;  
            Units = "(%)" ;  
            ShortName = "O2";
            Slope=1;
            Intercept=0;
            Page = -1; //calibration storage page
            CalEquationType=1;
         break;
       default: 
            Name = "" ;  
            SensorNumber = 0; //
            Units = "" ;  
            ShortName = "";
            Slope=1;
            Intercept=0;
            Page = - 1; //calibration storage page
            CalEquationType=1;
         break;
   } // end of switch case
  } //end of BTA resistor check

  void DigitalSensorID(int Channel)
    { 
     int i;
     int x;
     char c;
     Name="";// clear name string
     ShortName="";// clear name string
     Units=""; //clear Units String
     SensorNumber = 0;
     // check for digital ID sensor:
     for (i=0; i<133; i++)  // clear our digital ID sensor data
         { 
           sensordata[i]=0; 
         } 
       //Now check for Digital AutoID sensor:
      Wire.begin(); // join i2c bus (address optional for master) 
      //Reading device first time... ;
      Wire.beginTransmission(device); // Now we're going to read it back
      Wire.write(0x0);               // Sending address 0, so it knows where we'll want
      Wire.endTransmission();       // to read from
      x =Wire.requestFrom(device,32);    // Start new transmission and keep reading for 128 bytes
      i=1;
      while(x>1)    
        {  
          x=Wire.available();
           byte a = Wire.read();    // Read a byte and write it out to the Serial port
           sensordata [i]=a;
           i++;  
         } 
      //Reading device second time... ;
      Wire.beginTransmission(device); // Now we're going to read it back
      Wire.write(0x20);               // Sending address 0, so it knows where we'll want
      Wire.endTransmission();       // to read from
      x =Wire.requestFrom(device,32);    // Start new transmission and keep reading for 128 bytes
      i=1;
      while(x>1)    
       {  
         x=Wire.available();
         byte c = Wire.read();    // Read a byte and write it out to the Serial port
         sensordata [i+32]=c;
         i++;  
        }  
       //Reading device third time... ;
       Wire.beginTransmission(device); // Now we're going to read it back
       Wire.write(0x40);               // Sending address 0, so it knows where we'll want
       Wire.endTransmission();       // to read from
       x =Wire.requestFrom(device,32);    // Start new transmission and keep reading for 128 bytes
       i=1;
       while(x>1)    
        {  
          x=Wire.available();
          byte c = Wire.read();    // Read a byte and write it out to the Serial port
          sensordata [i+64]=c;
          i++;  
         }      
       //Reading device a forth time... ;
       Wire.beginTransmission(device); // Now we're going to read it back
       Wire.write(0x60);               // Sending address 0, so it knows where we'll want
       Wire.endTransmission();       // to read from
       x =Wire.requestFrom(device,32);    // Start new transmission and keep reading for 128 bytes
       i=1;
       while(x>1)    
        {  
          x=Wire.available();
          byte c = Wire.read();    // Read a byte and write it out to the Serial port
          sensordata [i+96]=c;
          i++;  
        }      
      /*Print out array:  // remove *'s to get this display for diagnostics
      Serial.print("array: ");  
      for (i = 0; i<121;i++)
       {
         Serial.print (i);
         Serial.print (" ");
         Serial.print(sensordata[i]);  
         Serial.print (" ");
         Serial.println(char(sensordata[i])); 
        }
       */
         //******************************************************************
          //Determine sensor number:  
         //  VoltageID[Channel]=-1;// indicated resistor ID not used
        SensorNumber =sensordata[2];  
        //Determine the sensor name:    
        Name="";
        for (i = 0; i<20;i++)
         {
           c =  sensordata[i+9]; 
           if (c !=0) Name += c; 
         }
        Name += '\0';    //add terminating character
        Name.trim();// get rid of any leading characters
        //Determine the short name:  
         for (i = 0; i<12;i++)
         {
           c =  sensordata[i+28]; 
           if (c !=0) ShortName += c;
         }
        ShortName += '\0';    //add terminating character
        ShortName.trim();// get rid of any leading characters
        
        //Determine the calibration equation type:  
        CalEquationType=sensordata[59]; 
        
        //Determines the  calibration page:  
        Page= sensordata[70];  
  
        // the code below uses the calibration page set:
        // Intercept starts at 71 for page 1, 90 for p2, and 109 for p3
        
        //Determines intercept:   
        for (i = 0; i<4;i++)
          {
            floatbyte[i]=sensordata[i+71+(Page)*19];
          }
         float j = *(float*) &floatbyte;  
         Intercept =j;
         
         //Determines slope:
         // slope starts at 75 for page 1, 94 for p2, and 113 for p3
         for (i = 0; i<4;i++)
          {
            floatbyte[i]=sensordata[i+75+(Page)*19];
          }  
         float y = *(float*) &floatbyte;  
         Slope =y;
         
          //Determines the cfactor:
         // cfactor starts at 79 for page 1, 98 for p2, and 117 for p3
         for (i = 0; i<4;i++)
          {
            floatbyte[i]=sensordata[i+79+(Page)*19];
          }  
         float z = *(float*) &floatbyte;  
         cfactor =z;
         //Determines the units:  
         // the cryptic code in the next line just uses the
         //correct bytes, depending on the page selected.
         // units start at 83 for page 1, 102 for p2, and 121 for p3
         for (i= 0; i<7;i++)
           {
             c =  sensordata[83+i+(Page)*19]; 
            if (c !=0) Units += c;
           }
         Units += '\0';    //add terminating character
         Units.trim();// get rid of any leading characters
     } //end of checking for digital ID sensor
    
void PrintSensorInfo()
 {// print out information about sensor:
  //(This code can be commented out, but add it for more feedback)
   Serial.println(" "); 
   Serial.print("BTA Connector ");
   Serial.println(Channel);
   Serial.print("sensor number: "); 
   Serial.println(SensorNumber);
   Serial.print("ID voltage level: "); 
   Serial.println(VoltageID);
   Serial.print("sensor name: ");  
   Serial.println (Name);
   Serial.print("sensor short name: ");  
   Serial.println (ShortName); 
   Serial.print("calibration page: ");  
   Serial.println(Page);
   Serial.print("calibration equation type: ");  
   Serial.println(CalEquationType);
   Serial.print("intercept: ");
   Serial.println (Intercept);
   Serial.print("slope ");
   Serial. println(Slope); 
   Serial.print("cfactor ");
   Serial. println(cfactor); 
   Serial.print("units: "); 
   Serial.println (Units);
   // end of PrintSensorInfo
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
 float Resistor = 15000; //bridge resistor
// the measured resistance of your particular bridge resistor in
//the Vernier BTA-ELV this is a precision 15K resisitor 
  float Temp;  // Dual-Purpose variable to save space.
  Resistance=( Resistor*Raw /(1024-Raw)); 
  Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  Temp = 1 / (0.00102119 + (0.000222468 * Temp) + (0.000000133342 * Temp * Temp * Temp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
  return Temp;                                      // Return the Temperature
}



