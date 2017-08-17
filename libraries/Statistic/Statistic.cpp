
//#define DEBUG// turn off when things are finalized
#include "Statistic.h"
#include "Arduino.h"
#include <Wire.h>// used for I2C communication

Statistic::Statistic()
{
}
void Statistic::AutoID(int value)
{
  byte _sensordata [128];
  byte _ResistorIDInfo[][32]=
  {  
  {78,97,109,101,32,48,32,32,32,32,32,32,32,32,          83,104,111,114,116,78,97,109,101,32,   85,110,105,116,115,32,32,0},///nothing*
  {84,72,101,114,109,111,99,111,117,112,108,101,32,32,    84,101,109,112,32,32,32,32,32,32,      68,101,103,32,67,32,32,0},//Thermocouple*
  {86,111,108,116,97,103,101,32,43,47,45,49,48,86,        80, 111,116,32,32,32,32,32,32,32,      69, 75, 71,32,32,32,32,0},//Voltage +/-10*
  {67,117,114,114,101,110,116,32,32,32,32,32,32,32,        73,32,32,32,32,32,32,32,32,32,        65,32,32,32,32,32,32,0},//Current*
  {82,101,115,105,115,116,97,110,99,101,32,32,32,32,       82,101,115,32,32,32,32,32,32,32,     79,104,109,115,32,32,32,0},//Resistance*
  {69,76,32,84,101,109,112,32,32,32,32,32,32,32,          84,101,109,112,32,32,32,32,32,32,    68,101,103,32,67,32,32,0},//5 EL Temp*
  {109,105,115,115,105,110,103,32,32,32,32,32,32,32,      109,105,115,115,105,110,103,32,32,32,  109,105,115,115,105,110,103,0},//missing*
  {109,105,115,115,105,110,103,32,32,32,32,32,32,32,      109,105,115,115,105,110,103,32,32,32,  109,105,115,115,105,110,103,0},//missing*
  {68,105,102,102,32,86,111,108,116,97,103,101,32,32,      80,111,116,32,32,32,32,32,32,32,      86,32,32,32,32,32,32,0},//Diff Voltage*
  {67,117,114,114,101,110,116,32,32,32,32,32,32,32,        73,32,32,32,32,32,32,32,32,32,        65,32,32,32,32,32,32,0},//Current*
  {84,101,109,112,101,114,97,116,117,114,101,32,32,32,    84,101,109,112,32,32,32,32,32,32,     68,101,103,32,67,32,32,0},//10 Temp*
  {86,111,108,116,97,103,101,32,43,47,45,51,48,86,        80,111,116,32,32,32,32,32,32,32,     86,32,32,32,32,32,32,0},//Voltage +/-30*
  {84,73,32,76,105,103,104,116,32,32,32,32,32,32,        76,105,103,104,116,32,32,32,32,32,     32,32,32,32,32,32,32,0},//TI Light*
  {69,120,32,72,101,97,114,116,32,82,97,116,101,32,       86,32,32,32,32,32,32,32,32,32,         86,32,32,32,32,32,32,0},//Ex Heart Rate*
  {82,97,119,32,86,111,108,116,115,32,32,32,32,32,       86,32,32,32,32,32,32,32,32,32,         86,32,32,32,32,32,32,0},//Raw Volts*
  {69, 75, 71,32,32,32,32,32,32, 32,32,32,32,32,          69, 75, 71,32,32,32,32,32,2,           86,32,32,32,32,32,32,0},//15 EKG*
  {109,105,115,115,105,110,103,32,32,32,32,32,32,32,      109,105,115,115,105,110,103,32,32,32,  109,105,115,115,105,110,103,0},//missing*
  {67,97,114,98,111,110,32,68,105,111,120,105,100,101,    67,79,50,32,32,32,32,32,32,32,      112,112,109,32,32,32,32,0},//CO2*
  {79,120,121,103,101,110,32,32,32,32,32,32,32,32,       79,50,32,32,32,32,32,32,32,32,        37,32,32,32,32,32,32,0}//O2*};
  };
   /*
BELOW IS THE REAL ARRAY:!!!!!!!
int _ResistorIDInfo[][33]={
  *
{78,97,109,101,32,48,32,32,32,32,32,32,32,32,          83,104,111,114,116,78,97,109,101,32,   85,110,105,116,115,32,32,0},///nothing*
{84,72,101,114,109,111,99,111,117,112,108,101,32,32,    84,101,109,112,32,32,32,32,32,32,      68,101,103,32,67,32,32,0},//Thermocouple*
{86,111,108,116,97,103,101,32,43,47,45,49,48,86,        80, 111,116,32,32,32,32,32,32,32,      69, 75, 71,32,32,32,32,0},//Voltage +/-10*
{67,117,114,114,101,110,116,32,32,32,32,32,32,32,        73,32,32,32,32,32,32,32,32,32,        65,32,32,32,32,32,32,0},//Current*
{82,101,115,105,115,116,97,110,99,101,32,32,32,32,       82,101,115,32,32,32,32,32,32,32,     79,104,109,115,32,32,32,0},//Resistance*
{69,76,32,84,101,109,112,32,32,32,32,32,32,32,          84,101,109,112,32,32,32,32,32,32,    68,101,103,32,67,32,32,0},//5 EL Temp*
{109,105,115,115,105,110,103,32,32,32,32,32,32,32,      109,105,115,115,105,110,103,32,32,32,  109,105,115,115,105,110,103,0},//missing*
{109,105,115,115,105,110,103,32,32,32,32,32,32,32,      109,105,115,115,105,110,103,32,32,32,  109,105,115,115,105,110,103,0},//missing*
{68,105,102,102,32,86,111,108,116,97,103,101,32,32,      80,111,116,32,32,32,32,32,32,32,      86,32,32,32,32,32,32,0},//Diff Voltage*
{67,117,114,114,101,110,116,32,32,32,32,32,32,32,        73,32,32,32,32,32,32,32,32,32,        65,32,32,32,32,32,32,0},//Current*
{84,101,109,112,101,114,97,116,117,114,101,32,32,32,    84,101,109,112,32,32,32,32,32,32,     68,101,103,32,67,32,32,0},//10 Temp*
{86,111,108,116,97,103,101,32,43,47,45,51,48,86,        80,111,116,32,32,32,32,32,32,32,     86,32,32,32,32,32,32,0},//Voltage +/-30*
{84,73,32,76,105,103,104,116,32,32,32,32,32,32,        76,105,103,104,116,32,32,32,32,32,     32,32,32,32,32,32,32,0},//TI Light*
{69,120,32,72,101,97,114,116,32,82,97,116,101,32,       86,32,32,32,32,32,32,32,32,32,         86,32,32,32,32,32,32,0},//Ex Heart Rate*
{82,97,119,32,86,111,108,116,115,32,32,32,32,32,       86,32,32,32,32,32,32,32,32,32,         86,32,32,32,32,32,32,0},//Raw Volts*
{69, 75, 71,32,32,32,32,32,32, 32,32,32,32,32,          69, 75, 71,32,32,32,32,32,2,           86,32,32,32,32,32,32,0},//15 EKG*
{109,105,115,115,105,110,103,32,32,32,32,32,32,32,      109,105,115,115,105,110,103,32,32,32,  109,105,115,115,105,110,103,0},//missing*
{67,97,114,98,111,110,32,68,105,111,120,105,100,101,    67,79,50,32,32,32,32,32,32,32,      112,112,109,32,32,32,32,0},//CO2*
{79,120,121,103,101,110,32,32,32,32,32,32,32,32,       79,50,32,32,32,32,32,32,32,32,        37,32,32,32,32,32,32,0}//O2*
};
*/
    int _i=0;
    //int _SensorNumber=0;
    _VoltageID=0;
    _Voltage=0;
    _slope  =1;
    _intercept =0;
    _cFactor = 0;
    _device=0x50;
    _page = 1; //calibration storage page (1 for resistor ID sensors)
    _CalEquationType = 1; // for all but thermistors; it will be changed in that case below
    _Channel = 1;// !!! will need to change???
    int _x;
    char _c;
    int _device=0x50;
    byte _floatbyte[5];

    Serial.println(" AutoID Check for Channel 1:");
    //Read BTA1 Sensor
    _VoltageID= analogRead(A5)/ 1024.0 * 5.00;// convert from count to voltage should use Vcc!!!
    Serial.println(_VoltageID);
    if (_VoltageID > 0.86 &_VoltageID < 0.95) _SensorNumber = 1; //Thermocouple
    if (_VoltageID > 3.72 &_VoltageID < 3.86) _SensorNumber = 2; // Voltage +/-10 V
    if (_VoltageID > 1.92 &_VoltageID < 2.13) _SensorNumber = 3; // TI Current Probe (not used)
    if (_VoltageID > 1.18 &_VoltageID < 1.30) _SensorNumber = 4; //Resistance
    if (_VoltageID > 3.27 &_VoltageID < 3.68) _SensorNumber = 5; //Extra-Long Temperature Probe
    if (_VoltageID > 4.64 &_VoltageID < 4.73) _SensorNumber = 8; //Differential Voltage
    if (_VoltageID > 4.73 &_VoltageID < 4.82) _SensorNumber = 9; //Current
    if (_VoltageID > 2.38 &_VoltageID < 2.63) _SensorNumber = 10; //Stainless Steel or Surface Temperature Probe
    if (_VoltageID > 2.85 &_VoltageID < 3.15) _SensorNumber = 11; // Voltage 30 V
    if (_VoltageID > 1.52 &_VoltageID < 1.68) _SensorNumber = 12; //TILT, TI Light Sensor
    if (_VoltageID > 0.43 &_VoltageID < 0.48) _SensorNumber = 13; //Exercise Heart Rate
    if (_VoltageID > 4.08 &_VoltageID < 4.16) _SensorNumber = 14; //Raw Voltage
    if (_VoltageID > 0.62 &_VoltageID < 0.68) _SensorNumber = 15; //EKG
    if (_VoltageID > 4.32 &_VoltageID < 4.40) _SensorNumber = 17; //CO2
    if (_VoltageID > 4.50 &_VoltageID < 4.59) _SensorNumber = 18; //Oxygen
    
    if (_SensorNumber != 0) // if any resistor ID sensor found
  {
    for (_i = 0; _i < 128; _i++) // clear our digital ID sensor data
     {
       _sensordata[_i] = 0;
     }
     //code below assumes the _ResistorIDInfo array is correct and sticks those numbers, for a particular
     //sensor into the sensorData array.
     //read Name char string into array in the right spot \; first 14 character are Name
      for (_i = 0; _i < 14; _i++)
        {
         _sensordata[9+_i]= _ResistorIDInfo [_SensorNumber][_i];
        }
      //read ShortName char string into array in the right spots, characters 15-24
      for (_i = 0; _i < 10; _i++)// 
        {
       _sensordata[40+_i]= _ResistorIDInfo [_SensorNumber][14+_i];
        }
      // read Units char string into array in the right spots, characters 25-32
       for (_i = 0; _i <7; _i++)// !!!
        {     
         _sensordata[83+_i]= _ResistorIDInfo [_SensorNumber][24+_i];//note page is always 1
        }
       
        /*#if defined DEBUG
         // Serial.print("_ResistorIDInfo array: ");  //only if "#define DEBUG" is in the code
            for (_i = 0; _i < 33; _i++)//display whole line of array as numbers
              {
                Serial.print(_ResistorIDInfo [_SensorNumber][_i]);
                Serial.print(",");
              }
               Serial.println("}");
              // Serial.println("_ResistorIDInfo array as char: ");  //only if "#define DEBUG" is in the code
            for (_i = 0; _i < 33; _i++)//display whole line of array as characters
              {
                Serial.print(char(_ResistorIDInfo[_SensorNumber][_i]));
              }
               Serial.println("}");
        Serial.print("sensordata array: ");  //only if "#define DEBUG" is in the code
        for (_i = 9; _i < 90; _i++)//display whole array 129 (the first 90 cover Name, ShortName, and Units)
         {
           Serial.print (_i);
           Serial.print (" ");
           Serial.print(_sensordata[_i]);
           Serial.print (" ");
           Serial.println(char(_sensordata[_i]));
          }
     */
   
    switch (_SensorNumber)
       {
        case 1:
            // Thermocouple ;
            _slope = -2.45455;
            _intercept = 6.2115;
            break;
        case 2:
            // Voltage +/- 10V" ;
            _slope = 4; //note correction for Sparkfun circuit done in calculation of Voltage!
            _intercept = -10;
            break;
        case 3:
            // Current;
            _slope = -2.665;
            _intercept = 6.325;
            break;
        case 4:
            // Resistance ;
             _slope = -2.5;
            _intercept = 6.25;
            break;
        case 5:
            // EL Temp   //Extra-Long Temperature Probe
             _slope = 58.341;
            _intercept = -53.073;
            break;
        case 8:
            // Diff Voltage ;
             _slope = -2.5;
            _intercept = 6.25;
            break;
        case 9:
            // Current ;
             _slope = -0.25;
            _intercept = 0.625;
            break;
        case 10:
            // Temperature ;
         //    _c= "Temperature";  !!!
             _slope = 1;
            _intercept = 0;
            _CalEquationType = 12; //Steinhart-Hart (for this sensor only)
            break;
        case 11:
            // Voltage +/- 30V" ;//
             _slope = 15.41;
            _intercept = -40.35;
            break;
        case 12:
            // TI Light ;
             _slope = 1;
            _intercept = 0;
            break;
        case 13:
            // Exercise Heart Rate ;
             _slope = 1;
            _intercept = 0;
            break;
        case 14:
            // Raw Voltage ;
             _slope = 1;
            _intercept = 0;
            break;
        case 15:
            // EKG ;
             _slope = 1;
            _intercept = 0;
            break;
        case 17:
            // Carbon Dioxide ;
             _slope = 1;
            _intercept = 0;
            break;
        case 18:
            // Oxygen ;
             _slope = 1;
            _intercept = 0;
            break;
        default:
            //Units[1] = 0; // Units is 10 characters
             _slope=1;
            _intercept = 0;
            break;
        } // end of switch case
  }// end of if a resistor ID sensor is found
 //temporary end of function with Digital ID not included!!!

  if (_SensorNumber==0)// no reistor ID sensor found; check I2C
  {
    Wire.begin(); // join i2c bus (address optional for master) !!!
    pinMode(A4, OUTPUT); //Turn on the I2C communication!!! this can cause problems!!!
    pinMode(A5, OUTPUT);
    // check for digital ID sensor:
    Wire.begin(); // join i2c bus (address optional for master) !!!
    //Reading _device first time... ;
    Wire.beginTransmission(_device); // Now we're going to read it back
    Wire.write(0x0); // Sending address 0, so it knows where we'll want
    Wire.endTransmission();
    _x = Wire.requestFrom(_device, 32);  // Start new transmission and keep reading for 32 bytes
    // note: the default buffer size for Arduino is 23 bytes. You can change it to larger. It would be
    // desirable to change it to 128 bytes and read all the data in one read. That is the way all Vernier
    //interfaces do it. is done as follows:  add#define SERIAL_BUFFER_SIZE 128 !!!
    //check it in your sketch with:
    // Serial.print(SERIAL_BUFFER_SIZE);
    _i = 1;
    while(_x > 1)
       {
        _x = Wire.available();
        byte _c = Wire.read();    // Read a byte and write it out to the Serial port
        _sensordata [_i] = _c;
        _i++;
       }
    //Reading device second time... ;
    Wire.beginTransmission(_device); // Now we're going to read it back
    Wire.write(0x20);               // Sending address 0, so it knows where we'll want
    Wire.endTransmission();       // to read from
    _x = Wire.requestFrom(_device, 32);  // Start new transmission and keep reading for 128 bytes
    _i = 1;
    while(_x > 1)
    {
        _x = Wire.available();
        byte _c = Wire.read();    // Read a byte and write it out to the Serial port
        _sensordata [_i + 32] = _c;
        _i++;
    }
    //Reading device third time... ;
    Wire.beginTransmission(_device); // Now we're going to read it back
    Wire.write(0x40);               // Sending address 0, so it knows where we'll want
    Wire.endTransmission();       // to read from
    _x = Wire.requestFrom(_device, 32);  // Start new transmission and keep reading for 128 bytes
    _i = 1;
    while(_x > 1)
    {
        _x = Wire.available();
        byte _c = Wire.read();    // Read a byte and write it out to the Serial port
        _sensordata [_i + 64] = _c;
        _i++;
    }
    //Reading device a forth time... ;
    Wire.beginTransmission(_device); // Now we're going to read it back
    Wire.write(0x60);               // Sending address 0, so it knows where we'll want
    Wire.endTransmission();       // to read from
    _x = Wire.requestFrom(_device, 32);  // Start new transmission and keep reading for 128 bytes
    _i = 1;
    while(_x > 1)
    {
        _x = Wire.available();
        byte _c = Wire.read();    // Read a byte and write it out to the Serial port
        _sensordata [_i + 96] = _c;
        _i++;
    } 

/*#if defined DEBUG
    Serial.print("array: ");  //only if "#define DEBUG" is in the code
    for (_i = 0; _i < 129; _i++)//display whole array
    {
        Serial.print (_i);
        Serial.print (" ");
        Serial.print(_sensordata[_i]);
        Serial.print (" ");
        Serial.println(char(_sensordata[_i]));
    }
#endif
*/
        _VoltageID=-1;/// flags a I2C sensor
        ;//Determines the  SensorNumber:  
        _page= _sensordata[2];  
  
        // the code below uses the calibration page set:
        // Intercept starts at 71 for page 1, 90 for p2, and 109 for p3
        /* this code  (for Name, ShortName, and Units, should be move to the end of the function and used for either kind of AutoID: !!!!
    // Determine the sensor name:    
        _Name="";
        for (_i = 0; _i<20;_i++)
         {
           char _c =  _sensordata[_i+9]; 
           _Name += _c;
         }
      _Name += '\0';    //add terminating character
         
        //Determine the short name:  
         _ShortName ="";
         for (_i = 0; _i<12;_i++)
          {
           char _c =  _sensordata[_i+28]; 
           _ShortName  += _c;
         }
         _ShortName  += '\0';    //add terminating character
       */
     
        //Determine the calibration equation type:  
        _CalEquationType=_sensordata[57]; 
        
        //Determines the  calibration page:  
        _page= _sensordata[70];  
  
        // the code below uses the calibration page set:
        // Intercept starts at 71 for page 1, 90 for p2, and 109 for p3
        
        //Determines intercept:   
        for (_i = 0; _i<4;_i++)
          {
            _floatbyte[_i]=_sensordata[_i+71+(_page)*19];
          }
         float _j = *(float*) &_floatbyte;  
         _intercept=_j;
         
         //Determines slope:
         // slope starts at 75 for page 1, 94 for p2, and 113 for p3
         for (_i = 0; _i<4;_i++)
          {
            _floatbyte[_i]=_sensordata[_i+75+(_page*19)];

          } 
          float _y = *(float*) &_floatbyte;  
         _slope=_y;
         /*determine the Units:
         // the cryptic code in the next line just uses the
         //correct bytes, depending on the page selected.
         // units start at 83 for page 1, 102 for p2, and 121 for p3
         for (_i= 0; _i<7;_i++)
           {
             char c =  _sensordata[83+_i+(_page )*19]; 
             _Units  += _c;
           }
            _Units  += '\0';    //add terminating character

 }  //end of checking for digital ID sensorRaw=analogRead(A0);
 */
 } // end of if 

  // Determine the sensor name:    
        Serial.print ("Name: ");
         //_Name="";
        for (_i = 0; _i<20;_i++)
         {
           char _c =  _sensordata[_i+9]; 
           Serial.print(_c);
          // _Name += _c;
         }
        Serial.print ("  Short Name: ");
        //Determine the short name:  
        //_ShortName ="";
         for (_i = 0; _i<12;_i++)
          {
           char _c =  _sensordata[_i+28]; 
           Serial.print(_c);
           //_ShortName  += _c;
          }
          //_ShortName  += '\0';    //add terminating character
          //determine the Units:
          // units start at 83 for page 1, 102 for p2, and 121 for p3
             Serial.print (" Units: ");
             for (_i= 0; _i<7;_i++)
           {
             char c =  _sensordata[_i+(_page )*19]; 
             Serial.print(_c);
            // _Units  += _c;
           }
            // _Units += '\0';    //add terminating character
          Serial.println (" ");

 }// end of AutoID function
   
   void Statistic::ReadSensor()//This function converts count to sensor reading
{
    //do I need to turn of I2C communication? !!!
    int _numberAveraged=10; //number of readings averaged for reading reported
    int _Count;
    int _AltCount;
    int _sum=0;
    float _Voltage;

    //better code for reading voltage:
    if  (_SensorNumber == 2 || _SensorNumber == 11) // one of two sensors using the +/- 10 volt line
    {
        for (int i = 0 ; i < _numberAveraged; i++)
        {
            _AltCount = analogRead(A1); //read  +/- 10 volt lin
            _sum = _sum + _AltCount;
        }
        _Voltage = _sum / _numberAveraged / 1024.0 * 5.0 ; // convert from count to raw voltage if using 10 volt range:
    }
    else
    {
        for (int i = 0 ; i < _numberAveraged; i++)
        {
            _Count = analogRead(A0); //read 0 to 5 volt analog lines
            _sum = _sum + _Count;
        }
        _Voltage = _sum / _numberAveraged / 1024.0 * 5.0;  //convert average count to voltage (0 to 5 volt input)
    }
    _SensorReading = _intercept + _Voltage * _slope; //for all linear sensors
   
    //the code below deals with BTA sensors which have non-linear calibrations
    //Special calibration for Wide Range Tempeature Sensor(78):
    if  (_SensorNumber == 78 ) _SensorReading = _intercept + _Voltage * _slope + _cFactor * _Voltage * _Voltage;
    //Special quadratic calibration for Ethanol Sensor(97):
    if  (_SensorNumber == 97) _SensorReading = _intercept * pow(_Voltage,_slope);
    //Special quadratic calibration for Sound Level Sensor(118)
    if  (_SensorNumber == 118) _SensorReading = _intercept + _slope * _Voltage + _cFactor * _Voltage * _Voltage;
    //Special calibration for Melt Station(92):
    if  (_SensorNumber == 92) _SensorReading = _intercept + _Voltage * _slope + _cFactor * _Voltage * _Voltage;
    //Special calibration for ISEs, CA(38), NH4(39), NO3(40), Cl(41):
    if  (_SensorNumber > 37 && _SensorNumber < 42) _SensorReading = (137.55 * _Voltage - 0.1682);
    //Special calibration for Potasium(113) ISE:
    if  (_SensorNumber == 113) _SensorReading = (137.55 * _Voltage - 0.1682);//Potasium ISE
    if  (_SensorNumber == 123) _SensorReading = _intercept + _Voltage * _slope + _cFactor * _Voltage * _Voltage;
  //Special quadratic calibration for New (Oct. 2016 Thermocouple(123));
    if (_SensorNumber==10)
    {
 /* Inputs ADC count from Thermistor and outputs Temperature in Celsius
 *  note that this requires: include <math.h>
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

 Resistance = ( Count*RawADC /(1024-Count))  !!!what the hell does this mean???
 */
  
    _resistor = 15000;
    long Resistance= (_resistance*_sum/_numberAveraged )/(1024-_sum/_numberAveraged );
   _logR = log(_resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
   _SensorReading = 1 / (0.00102119 + (0.000222468 * _logR + (0.000000133342 * _logR*_logR*_logR)));
   _SensorReading =  _SensorReading - 273.15;  // Convert Kelvin to Celsius   

   _resistor = 15000;
   float _Temp;  // Dual-Purpose variable to save space.
  long _Resistance = ( _resistor * (_sum/_numberAveraged )) / (1024 - (_sum/_numberAveraged));
  _Temp = log(_Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  _Temp = 1 / (0.00102119 + (0.000222468 * _Temp) + (0.000000133342 * _Temp * _Temp * _Temp));
   _SensorReading = _Temp-273.15;  // Convert Kelvin to Celsius
  return  _SensorReading;                                      // Return the Temperature        
}// end of thermistor code
}// END OF Read Sensor