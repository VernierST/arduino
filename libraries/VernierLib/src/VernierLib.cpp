/* This is a library to make using Vernier sensors and the Digital Control Unit (DCU)
with a Vernier/Sparkfun Interface Shield easier. There are several useful functions:
AutoID: reads information about the sensor, including calibration information
readSensor:  uses the calibration information and return calibrated values from the sensor
readMotionDetector: returns the distance reading from a Vernier Motion Detector in Digital 2
DCU:  allows you to control the output of a Vernier Digital Control Unit (DCU)
DCUStep:  allows you to easily control a stepper motor connected to the DCU
DCUPWM: allows you to control the voltage to a DC motor connected to the DCU
*/

//#define DEBUG1 // add for print statements
#define DEBUG3 // add for resistor AutoID
#define DEBUG4 // add for I2C AutoID
#define DEBUG5 // add for readSensor
#define DEBUG6 // add for Print Sensor Info (actually used in the main sketch)
#define DEBUG6 // add for Print Sensor Info (actually used in the main sketch)
#define DEBUG7 // add for DCU, PWM, and stepper
#define DEBUG8 // add for Motion Detector
//single channel version
#include "VernierLib.h"
#include "Arduino.h"
#include <Wire.h>// used for I2C communication

VernierLib::VernierLib()
{
  pinMode(2, INPUT); //Echo pin; this is the pin that goes high when an echo is received 
  pinMode(3, OUTPUT);//Trigger Pin used for Motion Detector
  pinMode(6, OUTPUT);// set up DCU lines, assuming it is on Digital 2
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT); //multiplexer on the shield, lsb
  pinMode(11, OUTPUT); //multiplexer on the shield, msb
  pinMode (12, INPUT_PULLUP); //button on DCU 
  pinMode (13, OUTPUT); //LED on shield
 }

void VernierLib::autoID()
{
  _channel =1;//this is the Analog 1 only version of the library\  
  _sensorNumber=0;
  _voltageID = 0;
  _slope  = 1;
  _intercept = 0;
  _cFactor = 0;
  _page = 0; //calibration storage page (always 0 for resistor ID sensors)
  _calEquationType = 1; // for all resisto-ID sensrs, but thermistors and some I2C sensors; it will be changed in that case below
  const int _device = 0x50; //used for I2C autoID
  byte _floatbyte[5];
  byte _sensorData [128];
  for (_i = 0; _i < 128; _i++) // clear our digital ID sensor data
  {
      _sensorData[_i] = 0;
  }

  #if defined DEBUG3
  byte _resistorIDInfo[][32] =
  {
    {78, 97, 109, 101, 32, 48, 32, 32, 32, 32, 32, 32, 32, 32,          83, 104, 111, 114, 116, 78, 97, 109, 101, 32,   85, 110, 105, 116, 115, 32, 32, 0}, ///nothing*
    {84, 72, 101, 114, 109, 111, 99, 111, 117, 112, 108, 101, 32, 32,    84, 101, 109, 112, 32, 32, 32, 32, 32, 32,      68, 101, 103, 32, 67, 32, 32, 0}, //Thermocouple*
    {86, 111, 108, 116, 97, 103, 101, 32, 43, 47, 45, 49, 48, 86,        86, 32, 32, 32, 32, 32, 32, 32, 32, 32,      86, 32, 32, 32, 32, 32, 32, 0}, //voltage +/-10*
    {67, 117, 114, 114, 101, 110, 116, 32, 32, 32, 32, 32, 32, 32,        73, 32, 32, 32, 32, 32, 32, 32, 32, 32,        65, 32, 32, 32, 32, 32, 32, 0}, //Current*
    {82, 101, 115, 105, 115, 116, 97, 110, 99, 101, 32, 32, 32, 32,       82, 101, 115, 32, 32, 32, 32, 32, 32, 32,     79, 104, 109, 115, 32, 32, 32, 0}, //resistance*
    {69, 76, 32, 84, 101, 109, 112, 32, 32, 32, 32, 32, 32, 32,          84, 101, 109, 112, 32, 32, 32, 32, 32, 32,    68, 101, 103, 32, 67, 32, 32, 0}, //5 EL Temp*
    {109, 105, 115, 115, 105, 110, 103, 32, 32, 32, 32, 32, 32, 32,      109, 105, 115, 115, 105, 110, 103, 32, 32, 32,  109, 105, 115, 115, 105, 110, 103, 0}, //missing*
    {109, 105, 115, 115, 105, 110, 103, 32, 32, 32, 32, 32, 32, 32,      109, 105, 115, 115, 105, 110, 103, 32, 32, 32,  109, 105, 115, 115, 105, 110, 103, 0}, //missing*
    {68, 105, 102, 102, 32, 86, 111, 108, 116, 97, 103, 101, 32, 32,      80, 111, 116, 32, 32, 32, 32, 32, 32, 32,      86, 32, 32, 32, 32, 32, 32, 0}, //Diff voltage*
    {67, 117, 114, 114, 101, 110, 116, 32, 32, 32, 32, 32, 32, 32,        73, 32, 32, 32, 32, 32, 32, 32, 32, 32,        65, 32, 32, 32, 32, 32, 32, 0}, //Current*
    {84, 101, 109, 112, 101, 114, 97, 116, 117, 114, 101, 32, 32, 32,    84, 101, 109, 112, 32, 32, 32, 32, 32, 32,     68, 101, 103, 32, 67, 32, 32, 0}, //10 Temp*
    {86, 111, 108, 116, 97, 103, 101, 32, 43, 47, 45, 51, 48, 86,        80, 111, 116, 32, 32, 32, 32, 32, 32, 32,     86, 32, 32, 32, 32, 32, 32, 0}, //voltage +/-30*
    {84, 73, 32, 76, 105, 103, 104, 116, 32, 32, 32, 32, 32, 32,        76, 105, 103, 104, 116, 32, 32, 32, 32, 32,     32, 32, 32, 32, 32, 32, 32, 0}, //TI Light*
    {69, 120, 32, 72, 101, 97, 114, 116, 32, 82, 97, 116, 101, 32,       86, 32, 32, 32, 32, 32, 32, 32, 32, 32,         86, 32, 32, 32, 32, 32, 32, 0}, //Ex Heart Rate*
    {82, 97, 119, 32, 86, 111, 108, 116, 115, 32, 32, 32, 32, 32,       86, 32, 32, 32, 32, 32, 32, 32, 32, 32,         86, 32, 32, 32, 32, 32, 32, 0}, //Raw Volts*
    {69, 75, 71, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,          69, 75, 71, 32, 32, 32, 32, 32, 2,           86, 32, 32, 32, 32, 32, 32, 0}, //15 EKG*
    {109, 105, 115, 115, 105, 110, 103, 32, 32, 32, 32, 32, 32, 32,      109, 105, 115, 115, 105, 110, 103, 32, 32, 32,  109, 105, 115, 115, 105, 110, 103, 0}, //missing*
    {67, 97, 114, 98, 111, 110, 32, 68, 105, 111, 120, 105, 100, 101,    67, 79, 50, 32, 32, 32, 32, 32, 32, 32,      112, 112, 109, 32, 32, 32, 32, 0}, //CO2*
    {79, 120, 121, 103, 101, 110, 32, 32, 32, 32, 32, 32, 32, 32,       79, 50, 32, 32, 32, 32, 32, 32, 32, 32,        37, 32, 32, 32, 32, 32, 32, 0} //O2*};
  };
  
  _sensorName[16]="1234567890123456"; // I am using 16 characters here, plus terminator.
  _shortName[12]="shortName   ";//12 characters on name, plus terminator.
  _sensorUnits[7]="Units  ";//7 characters units, plus terminator.
  _sensorName[17]='\0'; // THESE MAY NOT BE NECESSARY
  _shortName[13]='\0';
  _sensorUnits[8]='\0';
  //set multiplexer to match channel: NOTE THIS IS GETTING READY FOR A 2-CHANNEL VERSION
  if (_channel == 1 )
    {
      digitalWrite(10, LOW); //set multiplexer for BTA1  
    } 
  else
    {
      digitalWrite(10, HIGH); //set multiplexer for BTA2   
    }
   digitalWrite(11, LOW);//

  #if defined DEBUG1
        Serial.print("_resistorIDInfo array: ");  //only if "#define" is in the code
        for (_i = 0; _i < 33; _i++)//display whole line of array as numbers
        {
            Serial.print(_resistorIDInfo [_sensorNumber][_i]);
            Serial.print(",");
        }
        Serial.println("}");
        Serial.println("_resistorIDInfo array as char: ");  //only if "#define" is in the code
        for (_i = 0; _i < 33; _i++)//display whole line of array as characters
        {
            Serial.print(char(_resistorIDInfo[_sensorNumber][_i]));
        }
        Serial.println("}");
  #endif  //DEBUG1

  //Read BTA1 Sensor with resistor ID:
  _voltageID = analogRead(A5) / 1024.0 * 5.00; // convert from count to voltage could use Vcc!!!
  if (_voltageID > 0.86 & _voltageID < 0.95) _sensorNumber = 1; //Thermocouple
  if (_voltageID > 3.72 & _voltageID < 3.86) _sensorNumber = 2; // voltage +/-10 V
  if (_voltageID > 1.92 & _voltageID < 2.13) _sensorNumber = 3; // TI Current Probe (not used)
  if (_voltageID > 1.18 & _voltageID < 1.30) _sensorNumber = 4; //resistance
  if (_voltageID > 3.27 & _voltageID < 3.68) _sensorNumber = 5; //Extra-Long Temperature Probe
  if (_voltageID > 4.64 & _voltageID < 4.73) _sensorNumber = 8; //Differential voltage
  if (_voltageID > 4.73 & _voltageID < 4.82) _sensorNumber = 9; //Current
  if (_voltageID > 2.38 & _voltageID < 2.63) _sensorNumber = 10; //Stainless Steel or Surface Temperature Probe
  if (_voltageID > 2.85 & _voltageID < 3.15) _sensorNumber = 11; // voltage 30 V
  if (_voltageID > 1.52 & _voltageID < 1.68) _sensorNumber = 12; //TILT, TI Light Sensor
  if (_voltageID > 0.43 & _voltageID < 0.48) _sensorNumber = 13; //Exercise Heart Rate
  if (_voltageID > 4.08 & _voltageID < 4.16) _sensorNumber = 14; //Raw voltage
  if (_voltageID > 0.62 & _voltageID < 0.68) _sensorNumber = 15; //EKG
  if (_voltageID > 4.32 & _voltageID < 4.40) _sensorNumber = 17; //CO2
  if (_voltageID > 4.50 & _voltageID < 4.59) _sensorNumber = 18; //Oxygen
  
  if (_sensorNumber != 0) // if any resistor ID sensor found
  {
    //code below assumes the _resistorIDInfo array is correct and sticks those numbers, for a particular
    //sensor into the sensorData array.
    //read Name char string into array in the righ
     for (_i = 0; _i < 14; _i++)//
    {
      _sensorData[9 + _i] = _resistorIDInfo [_sensorNumber][_i];
    }
    //read ShortName char string into array in the right spots, characters 15-24
    for (_i = 0; _i < 10; _i++)//
    {
     _sensorData[29 + _i] = _resistorIDInfo [_sensorNumber][14 + _i];
    }
    // read Units char string into array in the right spots, characters 25-32
    for (_i = 0; _i < 7; _i++) // 
    {
      _sensorData[83 + _i] = _resistorIDInfo [_sensorNumber][24 + _i]; //note page is always 0
    }
     
    switch (_sensorNumber)
    {
      case 1:
        // Thermocouple ;
        _slope = -2.45455;
        _intercept = 6.2115;
        break;
      case 2:
        // voltage +/- 10V" ;
        _slope = 4; //note correction for Sparkfun circuit done in calculation of voltage!
        _intercept = -10;
        break;
      case 3:
        // Current;
        _slope = -2.665;
        _intercept = 6.325;
        break;
      case 4:
        // resistance ;
        _slope = -2.5;
        _intercept = 6.25;
        break;
      case 5:
        // EL Temp   //Extra-Long Temperature Probe
        _slope = 58.341;
        _intercept = -53.073;
        break;
      case 8:
        // Diff voltage ;
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
        _slope = 1;
        _intercept = 0;
        _calEquationType = 12; //Steinhart-Hart (for this sensor only)
        break;
      case 11:
        // voltage +/- 30V" ;//
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
        // Raw voltage ;
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
        _slope = 1;
        _intercept = 0;
        break;
    } // end of switch case
  }// end of if a resistor ID sensor is found
#endif  //DEBUG3

#if defined DEBUG4    
if (_sensorNumber == 0) // no resistor ID sensor found; check I2C
{
    pinMode(A4, OUTPUT); //Turn on the I2C communication!!! this can cause problems!!!
    pinMode(A5, OUTPUT);

    // check for digital ID sensor:
    Wire.begin(); // join i2c bus (address optional for master) 
    //Reading _device first time... ;
    Wire.beginTransmission(_device); // Now we're going to read it back
    Wire.write(0x0); // Sending address 0, so it knows where we'll want
    Wire.endTransmission();
    int _x = Wire.requestFrom(_device, 32);  // Start new transmission and keep reading for 32 bytes
    // note: the default buffer size for Arduino is 23 bytes. You can change it to larger. It would be
    // desirable to change it to 128 bytes and read all the data in one read. That is the way all Vernier
    //interfaces do it. is done as follows:  add#define SERIAL_BUFFER_SIZE 128 
    //check it in your sketch with:
    // Serial.print(SERIAL_BUFFER_SIZE);
    _i = 1;
    while (_x > 1)
    {
      _x = Wire.available();
      byte _c = Wire.read();    // Read a byte and write it out to the Serial port
      _sensorData [_i] = _c;
      _i++;
    }
    //Reading device second time... ;
    Wire.beginTransmission(_device); // Now we're going to read it back
    Wire.write(0x20);               // Sending address 0, so it knows where we'll want
    Wire.endTransmission();       // to read from
    _x = Wire.requestFrom(_device, 32);  // Start new transmission and keep reading for 128 bytes
    _i = 1;
    while (_x > 1)
    {
      _x = Wire.available();
      byte _c = Wire.read();    // Read a byte and write it out to the Serial port
      _sensorData [_i + 32] = _c;
      _i++;
    }
    //Reading device third time... ;
    Wire.beginTransmission(_device); // Now we're going to read it back
    Wire.write(0x40);               // Sending address 0, so it knows where we'll want
    Wire.endTransmission();       // to read from
    _x = Wire.requestFrom(_device, 32);  // Start new transmission and keep reading for 128 bytes
    _i = 1;
    while (_x > 1)
    {
      _x = Wire.available();
      byte _c = Wire.read();    // Read a byte and write it out to the Serial port
      _sensorData [_i + 64] = _c;
      _i++;
    }
    //Reading device a forth time... ;
    Wire.beginTransmission(_device); // Now we're going to read it back
    Wire.write(0x60);               // Sending address 0, so it knows where we'll want
    Wire.endTransmission();       // to read from
    _x = Wire.requestFrom(_device, 32);  // Start new transmission and keep reading for 128 bytes
    _i = 1;
    while (_x > 1)
    {
      _x = Wire.available();
      byte _c = Wire.read();    // Read a byte and write it out to the Serial port
      _sensorData [_i + 96] = _c;
      _i++;
    }
    _voltageID = -1; /// flags a I2C sensor
    ;//Determines the  sensorNumber:
    _sensorNumber= _sensorData[2];

    //Determine the calibration equation type:
    _calEquationType = _sensorData[57];

    //Determines the  calibration page:
    _page = _sensorData[70];
    // the code below uses the calibration page set:
    // Intercept starts at 71 for page 1, 90 for p2, and 109 for p3

    //Determines intercept:
    for (_i = 0; _i < 4; _i++)
    {
      _floatbyte[_i] = _sensorData[_i + 71 + (_page) * 19];
    }
    float _j = *(float*) &_floatbyte;
    _intercept = _j;

    //Determines slope:
    // slope starts at 75 for page 1, 94 for p2, and 113 for p3
    for (_i = 0; _i < 4; _i++)
    {
      _floatbyte[_i] = _sensorData[_i + 75 + (_page * 19)];
    }
    float _y = *(float*) &_floatbyte;
    _slope = _y;

    pinMode(A4, INPUT); //Turn off the I2C communication!!! this can cause problems!!!
    pinMode(A5, INPUT);

} // end of if I2C autoID
#endif  //DEBUG4

// Determine the sensor name:
for (_i = 0; _i < 16; _i++)
        {
          char _c =  _sensorData[_i + 9];
          _sensorName[_i] = _c;
        }
      _sensorName[16] = '\0';

  //Determine the short name:
for (_i = 0; _i < 11; _i++)
      {
        char _c =  _sensorData[_i + 29];  // changed from 28 to 29
        _shortName[_i] = _c;
      }
_shortName[11] = '\0';

  //determine the Units:
  // units start at 83 for page 1, 102 for p2, and 121 for p3
for (_i = 0; _i < 7; _i++)
      {
        char _c =  _sensorData[_i + 83+(_page)* 19];
        _sensorUnits[_i] = _c;
      }
_sensorUnits[7] = '\0'; //add terminating character
// Special handling for ISEs, CA, NH4, NO3, or Cl
if  (_sensorNumber > 37 && _sensorNumber < 42) strncpy(_sensorUnits, "mV     ", 7);
//Special calibration for Potasium ISE:
if  (_sensorNumber == 113) strncpy(_sensorUnits, "mV      ",7); // assign name based on sensor number

#if defined DEBUG1
     Serial.print("_voltageID ");// use this line, if you want to check the autoID voltage
     Serial.println(_voltageID);// use this line, if you want to check the autoID voltage/*
 #endif  //DEBUG1


#if defined DEBUG1
      Serial.print("sensorData array: ");  //only if "#define" is in the code
        for (_i = 0; _i < 129; _i++)//display whole array 
        {
            Serial.print (_i);
            Serial.print (" ");
            Serial.print(_sensorData[_i]);
            Serial.print (" ");
            Serial.println(char(_sensorData[_i]));
        }
#endif  //DEBUG1
 //!!!        Serial.print("calEquationType ");//!!!
 //!!!         Serial.println(calEquationType );
}// end of AutoID function

float VernierLib::readSensor()//This function converts count to sensor reading
{
  #if defined DEBUG5
  int _numberAveraged = 10; //number of readings averaged for reading reported
  int _count;
  int _sum = 0;
  float _voltage;
  byte _buttonState=0;//condition of button
  //better code for reading voltage:
  if  (_sensorNumber == 2 || _sensorNumber == 11) // one of two sensors using the +/- 10 volt line
  {
    for (_i = 0 ; _i < _numberAveraged; _i++)
    {
      if (_channel == 1) 
      {
       _count = analogRead(A1); //read 0 to 5 volt analog lines, Analog 1
      }
      else 
      { 
       _count = analogRead(A3); //read 0 to 5 volt analog lines Analog 2
      }
    _sum = _sum + _count;
    }//end of for loop
  }//end of if
  else
  {
    for (_i = 0 ; _i < _numberAveraged; _i++)
    {
      if (_channel == 1) 
      {
       _count = analogRead(A0); //read 0 to 5 volt analog lines, Analog 1
      }
      else 
      { 
       _count = analogRead(A2); //read 0 to 5 volt analog lines Analog 2
      }
          _sum = _sum + _count;
    }//end of for loop
  }//end of else
  _voltage = _sum / _numberAveraged / 1024.0 * 5.0;  //convert average count to voltage (0 to 5 volt input)
  _sensorReading = _intercept + _voltage * _slope; //for all linear sensors

  //the code below deals with BTA sensors which have non-linear calibrations
  //Special calibration for Wide Range Tempeature Sensor(78):
  if  (_sensorNumber == 78 ) _sensorReading = _intercept + _voltage * _slope + _cFactor * _voltage * _voltage;
  //Special quadratic calibration for Ethanol Sensor(97):
  if  (_sensorNumber == 97) _sensorReading = _intercept * pow(_voltage, _slope);
  //Special quadratic calibration for Sound Level Sensor(118)
  if  (_sensorNumber == 118) _sensorReading = _intercept + _slope * _voltage + _cFactor * _voltage * _voltage;
  //Special calibration for Melt Station(92):
  if  (_sensorNumber == 92) _sensorReading = _intercept + _voltage * _slope + _cFactor * _voltage * _voltage;
  //Special calibration for ISEs, CA(38), NH4(39), NO3(40), Cl(41):
  if  (_sensorNumber > 37 && _sensorNumber < 42) _sensorReading = (137.55 * _voltage - 0.1682);
  //Special calibration for Potasium(113) ISE:
  if  (_sensorNumber == 113) _sensorReading = (137.55 * _voltage - 0.1682);//Potasium ISE
  if  (_sensorNumber == 123) _sensorReading = _intercept + _voltage * _slope + _cFactor * _voltage * _voltage;
  //Special quadratic calibration for New (Oct. 2016 Thermocouple(123));
  if (_sensorNumber == 10) //if thermistor:
  {
    /* Inputs ADC count from Thermistor and outputs Temperature in Celsius
       note that this requires: include <math.h>
      There is a huge amount of information on the web about using thermistors with the Arduino.
      Here we are concerned about using the Vernier Stainless Steel Temperature Probe TMP-BTA and the
      Vernier Surface Temperature Probe STS-BTA, but the general principles are easy to extend to other
      thermistors.
      This version utilizes the Steinhart-Hart Thermistor Equation:
         Temperature in Kelvin = 1 / {A + B[ln(R)] + C[ln(R)]3}
        for the themistor in the Vernier TMP-BTA probe:
         A =0.00102119 , B = 0.000222468 and C = 1.33342E-7
         Using these values should get agreement within 1 degree C to the same probe used with one
         of the Vernier interfaces

      Schematic:
        [Ground] -- [thermistor] -------- | -- [15,000 ohm bridge resistor] --[Vcc (5v)]
                                          |
                                     Analog Pin 0

      For the circuit above:

      resistance = ( count*RawADC /(1024-count))  
    */
    float _logR;
    float _resistor;
    float logR;//!!() //{ return _logR; };
    _resistor = 15000;// 15k resistor in series with thermistor
    long _resistance = (_resistor * _voltage ) / (5.0- _voltage);
    _logR = log(_resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
    _sensorReading = 1 / (0.00102119 + (0.000222468 * _logR + (0.000000133342 * _logR * _logR * _logR)));
    _sensorReading =  _sensorReading - 273.15;  // Convert Kelvin to Celsius
  }// end of thermistor code

    _buttonState = digitalRead(12);// button on shield
    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if (_buttonState == LOW) // button down
    {
       digitalWrite(13, LOW);//turn on LED on shield
       Serial.print("V= ");
       Serial.println(_voltage, 1);  // display raw voltage on Serial Monitor
    
    }
    else
    return _sensorReading;

   #if defined DEBUG5       
}// END OF Read Sensor


#endif DEBUG7

void VernierLib::DCUPWM (int PWMSetting)
{

  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  if (PWMSetting<0) PWMSetting =0;
  if (PWMSetting>255) PWMSetting =255;
  Serial.print("PWM output set to ");
  Serial.println(PWMSetting);
  analogWrite(9, PWMSetting);// range 0 to 255
}

void VernierLib::DCUStep(int stepCount, int stepDirection, int stepDelay)
{
  int DCUStepPattern[4]; //pattern used to drive step motor
  if (stepDirection ==0)
    { 

    /*   The following sequences are for a "normal" step motor.
      5,9,10,6 steps the motor CW
      Reverse the order for CCW    
      */  
      DCUStepPattern[0]=5; //CW
      DCUStepPattern[1]=9;
      DCUStepPattern[2]=10;
      DCUStepPattern[3]=6; 
    }
    else
    {
       DCUStepPattern[0]=6; //CCW
       DCUStepPattern[1]=10;
       DCUStepPattern[2]=9;
       DCUStepPattern[3]=5; 
    }
  DCU(0);//Turn off all lines
  delay (100);
  Serial.print("step motor rotate for ");
  Serial.print(stepCount);
  Serial.print(" steps, in direction ");
  Serial.print(stepDirection);
  Serial.print(", with delay of ");
  Serial.print(stepDelay);
  Serial.println(" ms");
  for (int _x=0; _x<=stepCount; _x++) // set up step pattern
    {
      int _output = _x%4;// % = modulo: returns the remainder of x divided by the value (4) in the case
      //as "_x" increments "_output" will progress from 0, 1, 2, 3 repeating
      int _stepValue = DCUStepPattern[_output];
      DCU(_stepValue); // This points to the case for the DCU. See void DCU below.
      delay (stepDelay);
    };//end of for
}// end of PWM

void VernierLib::DCU(int DCUSetting)
{

switch (DCUSetting) 
  {
    case 0:
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      break;
    case 1:
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      break;
    case 2:
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      break;   
    case 3:
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      break;
    case 4:
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      break;    
    case 5:
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      break;
    case 6:
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      break;
    case 7:
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      break;
    case 8:
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, HIGH);
      break;
    case 9:
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, HIGH);
      break;
    case 10:
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(9, HIGH);
      break;
    case 11:
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(9, HIGH);
      break;
    case 12:
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
      break;
    case 13:
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
      break;
    case 14:
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
      break;
    case 15:
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
      break;
  }
}//end of DCU 
  #endif  //DEBUG7

  #if defined DEBUG8
  float VernierLib::readMotionDetector()  //This function reads Motion Detector
{
    /*
VernierMotionDetector 
Takes data from a Vernier Motion Detector connected to the Digital 1 connector.

This sketch measures the time taken for the ultrasound to return (in microseconds)
and then calculates the corresponding distance (based on the speed of ultrasound
in air) and displays the distance (in cm) on the Serial Monitor. 

Here is how the Vernier Motion Detector works:
- when pin 2 on BTD is pulled high, this triggers the ultrasound pulse
- the program then starts timing but then delays 0.9 ms *(blanking time,
   0.9 seconds is the time it takes ultrasound to travel 15 cm twice (round trip))
- the program then monitors pin 1 on the BTD, waiting for it to go high. 
This happens when an echo is detected.
  
See www.vernier.com/arduino for more information.
 */
 
  long _time; // clock reading in microseconds
  long _duration; // time it take echo to return
  int _val = 0;
  digitalWrite(3, LOW);
  delayMicroseconds(4000);
  digitalWrite(3, HIGH); // start the ultrasound pulse using Trigger Pin 3
  _time = micros(); //note time
  delayMicroseconds(900); //delay during the blanking time
  do
  {
    _val =digitalRead(2);// read echo pin 2
    // if no echo, repeat loop and wait:
  }
  while (_val == LOW) ;
  _duration =micros() - _time;
  /* The speed of sound is 340 m/s.
  The ultrasound travels out and back, so to find the distance of the
  object we take half of the distance traveled.*/
  _distance= _duration *340/2/10000 ;// note the 340 is the speed of sound in m/s. note convert to cm
  return _distance;
 }
  #endif  //DEBUG8
