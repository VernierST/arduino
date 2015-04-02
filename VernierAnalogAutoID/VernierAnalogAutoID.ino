/*
VernierAnalogAutoID (v 2015.04)
Reads the information to AutoID a Vernier BTA sensor with digital AutoID,
and resistor ID sensors including Vernier Stainless Steel Temperature Probe (TMP-BTA). 
It should read the +/-10 volt Voltage Probe correctly also.
This version does all tests for resistorID sensors first, then
turns on the I2C clock and tests for digital autoID sensors. After that,
it turns off the I2C communications.

Note that this sketch handles multiple pages of sensor calibrations.

When used with the SparkFun Vernier Interface Shield, this program will AutoID
two different sensors on BTA1 and BTA2. With homemade, breadboard
connections, it will work with only one sensor.

After the AutoID:
Assuming Vernier analog (BTA) Sensors are connected to the BTA connectors,
this sketch displays the time and sensor readings on the Serial Monitor.
As written, the readings will be displayed every second. 
Change the variable TimeBetweenReadings to change the rate.

The changes in the 2015.04 version are:
  - fixed bug which lead to incorrect readings of analog sensors in BTA2 
    connector because I2C lines affected the readings
  - minor changes to the voltage thresholds on resistor ID section
  
 See www.vernier.com/arduino for more information.
*/

int ReadingNumber;
int Channel; //BTA (Channel 1 or 2) 
float VoltageID[5];
int led =13;
int SensorNumber[5]; //integer indicating sensor number'
String Name[5];
String ShortName[5];
String Units[5];
float Intercept[5];
float Slope[5];
int Page[5];
int (CalEquationType[5]);
float VCC= 5.00;// "5 volt" power supply voltage used in resistor ID section

/////////////////////////////////////////
int TimeBetweenReadings = 1000; // in ms
////////////////////////////////////////
#include <Wire.h>
byte sensordata [128];
int device=0x50;
byte floatbyte[5];

void setup()
 {
    int muxlsb = 10; //low byte of multiplexer
    int muxmsb = 11; //high byte of multiplexer
    Serial.begin(9600);
    pinMode(led, OUTPUT); //LED on SparkFun Vernier Shield
    digitalWrite(led, LOW);   
    pinMode(muxlsb, OUTPUT); 
    pinMode(muxmsb, OUTPUT); 

    Serial.println(""); 
    //Read BTA1 Sensor
    digitalWrite(muxlsb, LOW); //set multiplexer for BTA1   
    digitalWrite(muxmsb, LOW);    
    Channel=1;
    BTAResistorSensorID(Channel);
    //Read BTA2 Sensor
    digitalWrite(muxlsb, HIGH); //set multiplexer for BTA2   
    digitalWrite(muxmsb, LOW);  
    Channel=2;  
    BTAResistorSensorID(Channel); 
    Wire.begin(); //start I2C communication
    //Read BTA1 Sensor
    digitalWrite(muxlsb, LOW); //set multiplexer for BTA1   
    digitalWrite(muxmsb, LOW);    
    Channel=1;
    if (SensorNumber[Channel]==0) DigitalSensorID( Channel);// if no resistorID, check for digital ID
    PrintSensorInfo();// this line can be commented out if you do not need all this info!!!
    //Read BTA2 Sensor
    digitalWrite(muxlsb, HIGH); //set multiplexer for BTA2   
    digitalWrite(muxmsb, LOW);  
    Channel=2;  
    if (SensorNumber[Channel]==0) DigitalSensorID( Channel);// if no resistorID, check for digital ID
    pinMode(A4, INPUT); //Turn off the I2C communication
    pinMode(A5, INPUT); 
    PrintSensorInfo();// this line can be commented out if you do not need all this info!!!
    Serial.println(" ");    
    Serial.println("Vernier Format 2");
    Serial.println("Readings taken using Ardunio");
    Serial.println("Data Set");
    Serial.print("Time");
    for (Channel=1;Channel<3;Channel++)
     { 
       Serial.print("\t"); //tab character
       //print sensor name:
       Serial.print (ShortName[Channel]); 
     } 
    Serial.println("");      
    Serial.print("seconds");
    for (Channel=1;Channel<5;Channel++)
     { 
       Serial.print("\t"); //tab character
       //print sensor name:
       Serial.print (Units[Channel]); 
     } 
    Serial.println ("");
  }
  
void loop()
 {
    int Count[3]; //reading from 0 to 5 volt input
    int AltCount[3] ;//reading from -10 to +10V input
    float Voltage[3];
    float SensorReading[3];
    float Time;
    digitalWrite(led, HIGH); //turn on LED
    Serial.print(ReadingNumber/1000.0*TimeBetweenReadings); 
    for (int Channel=1;Channel<3;Channel++)
      {
        Serial.print("\t"); //tab character
        if (Name[Channel]=="Voltage +/- 10V")
          {
             AltCount[1] = analogRead(A1); //read both +/- 110 volt lines
             AltCount[2] = analogRead(A3);
              // convert from count to raw voltage if using 10 volt range:
              Voltage[Channel] = AltCount[Channel] / 1024.0 * 5.0 ;          
           }
          else
            {        
              Count[1] = analogRead(A0); //read both analog lines
              Count[2] = analogRead(A2);
              // convert from count to raw voltage on 0 to 5 range:
              Voltage[Channel] = Count[Channel] / 1024.0 * 5.0; 
            }
           SensorReading[Channel]= Intercept[Channel] + Voltage[Channel] * Slope[Channel];
     //special calibration for thermistor temperture probe:
     if (SensorNumber[Channel]==10) SensorReading[Channel]=Thermistor(Count[Channel]);
     Serial.print(SensorReading[Channel]);  
    } // end of going through the channels

  Serial.println(" "); 
  delay(TimeBetweenReadings/2);// delay half of period
  digitalWrite(led, LOW);// LED on D13 flashes once per readng
  delay(TimeBetweenReadings/2);// delay the other half
  ReadingNumber++;
  } // end

void BTAResistorSensorID(int Channel)
 {
  Name[Channel]="";// clear name string
  ShortName[Channel]="";// clear name string
  SensorNumber[Channel] = 0;
  delay (10);
  int CountID = analogRead(A5);
  VoltageID[Channel] = CountID / 1024.0 * VCC;// convert from count to voltage 
  if (VoltageID[Channel]>0.86 & VoltageID[Channel]<0.95) SensorNumber[Channel] = 1; //Thermocouple
  if (VoltageID[Channel]>3.83 & VoltageID[Channel]<3.86) SensorNumber[Channel] = 2; // Voltage +/-10 V
  if (VoltageID[Channel]>1.92 & VoltageID[Channel]<2.13) SensorNumber[Channel] = 3; // TI Current Probe (not used)
  if (VoltageID[Channel]>1.18 & VoltageID[Channel]<1.30) SensorNumber[Channel] = 4; //Reistance 
  if (VoltageID[Channel]>3.27 & VoltageID[Channel]<3.68) SensorNumber[Channel] = 5; //Extra-Long Temperature Probe  
  if (VoltageID[Channel]>4.64 & VoltageID[Channel]<4.73) SensorNumber[Channel] = 8; //Differential Voltage
  if (VoltageID[Channel]>4.73 & VoltageID[Channel]<4.83) SensorNumber[Channel] = 9; //Current
  if (VoltageID[Channel]>2.38 & VoltageID[Channel]<2.63) SensorNumber[Channel] = 10; //Stainless Steel or Surface Temperature Probe
  if (VoltageID[Channel]>2.85 & VoltageID[Channel]<3.15) SensorNumber[Channel] = 11; // Voltage 30 V  
  if (VoltageID[Channel]>1.52 & VoltageID[Channel]<1.68) SensorNumber[Channel] = 12; //TILT, TI Light Sensor
  if (VoltageID[Channel]>0.43 & VoltageID[Channel]<0.48) SensorNumber[Channel] = 13; //Exercise Heart Rate
  if (VoltageID[Channel]>4.08 & VoltageID[Channel]<4.16) SensorNumber[Channel] = 14; //Raw Voltage
  if (VoltageID[Channel]>0.62 & VoltageID[Channel]<0.68) SensorNumber[Channel] = 15; //EKG
  if (VoltageID[Channel]>4.32 & VoltageID[Channel]<4.40) SensorNumber[Channel] = 17; //CO2 
  if (VoltageID[Channel]>4.50 & VoltageID[Channel]<4.59) SensorNumber[Channel] = 18; //Oxygen
  switch (SensorNumber[Channel]) 
   {
      case 1:
            Name[Channel] = "Thermocouple" ;  
            Units[Channel] = "Deg C " ;  
            ShortName[Channel] = "TC";
            Slope[Channel]=-2.45455;
            Intercept[Channel]=6.2115;
            Page[Channel] = 1;; //calibration storage p (1,2, or 3)
            CalEquationType[Channel]=1;
       break;
       case 2:
            Name[Channel] = "Voltage +/- 10V" ;  //!!! do not change this name or you will mess up the code of the loop
            Units[Channel] = "V" ;  
            ShortName[Channel] = "Voltage10";
            Slope[Channel]=4;//note correction for Sparkfun circuit done in calculation of Voltage!!
            Intercept[Channel]=-10;
            Page[Channel] = 1;; //calibration storage page 
            CalEquationType[Channel]=1;
      break;
      case 3:
            Name[Channel] = "Current" ;  
            Units[Channel] = "Amps" ;  
            ShortName[Channel] = "Current";
            Slope[Channel]=-2.665;
            Intercept[Channel]=6.325;
            Page[Channel] = 1;; //calibration storage page
            CalEquationType[Channel]=1;
      break;
      case 4:
            Name[Channel] = "Reistance" ;  
            Units[Channel] = "Ohms" ;  
            ShortName[Channel] = "Diff V";
            Slope[Channel]=-2.5;
            Intercept[Channel]=6.25;
            Page[Channel] = 1;; //calibration storage page
            CalEquationType[Channel]=1;
      break;
      case 8:
            Name[Channel] = "Diff Voltage" ;  
            Units[Channel] = "V" ;  
            ShortName[Channel] = "Diff V";
            Slope[Channel]=-2.5;
            Intercept[Channel]=6.25;
            Page[Channel] = 1;; //calibration storage page 
            CalEquationType[Channel]=1;
      break;
      case 9:
            Name[Channel] = "Current" ;  
            Units[Channel] = "Amp" ;  
            ShortName[Channel] = "I";
            Slope[Channel]=1;
            Intercept[Channel]=0;
            Page[Channel] = 1;; //calibration storage page
            CalEquationType[Channel]=1;
      break;
      case 10:
            Name[Channel] = "Temperature" ;  
            Units[Channel] = "Deg C" ;  
            ShortName[Channel] = "Temp";
            Slope[Channel]=1;
            Intercept[Channel]=0;
            Page[Channel] = 1;; //calibration storage page
            CalEquationType[Channel]=1;
       break;
       case 11:
            Name[Channel] = "Temperature" ;  
            Units[Channel] = "Deg C" ;  
            ShortName[Channel] = "Temp";
            Slope[Channel]=1;
            Intercept[Channel]=0;
            Page[Channel] = 1;; //calibration storage page
            CalEquationType[Channel]=1;
        break;
        case 12:
            Name[Channel] = "TI Light" ;  
            Units[Channel] = "relative" ;  
            ShortName[Channel] = "TI Light";
            Slope[Channel]=1;
            Intercept[Channel]=0;
            Page[Channel] = 1;; //calibration storage page
            CalEquationType[Channel]=1;
         break;
         case 13:
            Name[Channel] = "Exercise Heart Rate" ;  
            Units[Channel] = "V" ;  
            ShortName[Channel] = "Ex HR";
            Slope[Channel]=1;
            Intercept[Channel]=0;
            Page[Channel] = 1;; //calibration storage page
            CalEquationType[Channel]=1;
         break;
         case 14:
            Name[Channel] = "Voltage" ;  
            Units[Channel] = "V" ;  
            ShortName[Channel] = "Volts";
            Slope[Channel]=1;
            Intercept[Channel]=0;
            Page[Channel] = 1;; //calibration storage page
            CalEquationType[Channel]=1;
         break;
         case 15:
            Name[Channel] = "EKG" ;  
            Units[Channel] = "V" ;  
            ShortName[Channel] = "EKG";
            Slope[Channel]=1;
            Intercept[Channel]=0;
            Page[Channel] = 1;; //calibration storage page
            CalEquationType[Channel]=1;
         break;
         case 17:
            Name[Channel] = "Carbon Dioxide" ;  
            Units[Channel] = "ppm" ;  
            ShortName[Channel] = "CO2";
            Slope[Channel]=1;
            Intercept[Channel]=0;
            Page[Channel] = 1;; //calibration storage page
            CalEquationType[Channel]=1;
         break;
         case 18:
            Name[Channel] = "Oxygen" ;  
            Units[Channel] = "%" ;  
            ShortName[Channel] = "O2";
            Slope[Channel]=1;
            Intercept[Channel]=0;
            Page[Channel] = 1;; //calibration storage page
            CalEquationType[Channel]=1;
         break;
       default: 
            Name[Channel] = "nothing on BTA" ;  
            SensorNumber[Channel] = 0; //
            Units[Channel] = "" ;  
            ShortName[Channel] = "";
            Slope[Channel]=1;
            Intercept[Channel]=0;
            Page[Channel] = 1;; //calibration storage page
            CalEquationType[Channel]=1;
         break;
   } // end of switch case
  } //end of BTA resistor check


  void DigitalSensorID(int Channel)
    { 
      int i;
      int x;
        // check for digital ID sensor:
        for (i=0; i<133; i++)  // clear our digital ID sensor data
         { 
           sensordata[i]=0; 
         } 
       //Now check for Digital AutoID sensor:
      Wire.begin(); // join i2c bus (address optional for master) !!!
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
      for (i = 68; i<121;i++)
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
        SensorNumber[Channel] =sensordata[2];  
  
        //Determine the sensor name:    
        Name[Channel]="";
        for (i = 0; i<20;i++)
         {
           char c =  sensordata[i+9]; 
           Name[Channel] += c;
         }
        Name[Channel] += '\0';    //add terminating character
        
        //Determine the short name:  
         ShortName[Channel]="";
         for (i = 0; i<12;i++)
         {
           char c =  sensordata[i+28]; 
           ShortName[Channel] += c;
         }
        ShortName[Channel] += '\0';    //add terminating character
       
        //Determine the calibration equation type:  
        CalEquationType[Channel]=sensordata[57]; 
        
        //Determines the  calibration page:  
        Page[Channel]= sensordata[70];  
  
        // the code below uses the calibration page set:
        // Intercept starts at 71 for page 1, 90 for p2, and 109 for p3
        
        //Determines intercept:   
        for (i = 0; i<4;i++)
          {
            floatbyte[i]=sensordata[i+71+(Page[Channel])*19];
          }
         float j = *(float*) &floatbyte;  
         Intercept[Channel] =j;
         
         //Determines slope:
         // slope starts at 75 for page 1, 94 for p2, and 113 for p3
         for (i = 0; i<4;i++)
          {
            floatbyte[i]=sensordata[i+75+(Page[Channel])*19];
          }  
         float y = *(float*) &floatbyte;  
         Slope[Channel] =y;
         
         //Determines the units:  
         // the cryptic code in the next line just uses the
         //correct bytes, depending on the page selected.
         // units start at 83 for page 1, 102 for p2, and 121 for p3
         for (i= 0; i<7;i++)
           {
            char c =  sensordata[83+i+(Page[Channel])*19]; 
             Units[Channel] += c;
           }
         Units[Channel] += '\0';    //add terminating character
     } //end of checking for digital ID sensor
    
void PrintSensorInfo()
 {// print out information about sensor:
  //(This code is commented out, but add it for more feedback)
   Serial.println(" "); 
   Serial.print("BTA Connector ");
   Serial.println(Channel);
   Serial.print("sensor ID number: "); 
   Serial.println(SensorNumber[Channel]);
   Serial.print("ID voltage level: "); 
   Serial.println(VoltageID[Channel]);
   Serial.print("sensor name: ");  
   Serial.println (Name[Channel]);
   Serial.print("sensor short name: ");  
   Serial.println (ShortName[Channel]); 
   Serial.print("calibration page: ");  
   Serial.println(Page[Channel]);
   Serial.print("calibration equation type: ");  
   Serial.println(CalEquationType[Channel]);
   Serial.print("intercept: ");
   Serial.println (Intercept[Channel]);
   Serial.print("slope ");
   Serial. println(Slope[Channel]); 
   Serial.print("units: ");  
   Serial.println (Units[Channel]);
   }// end of PrintSensorInfo

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





