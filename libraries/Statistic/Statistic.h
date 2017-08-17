
#ifndef Statistic_h
#define Statistic_h

#include <math.h>

#define STATISTIC_LIB_VERSION "0.1.6.72" // displayed automatically

class Statistic
{
public:
    Statistic();
    void AutoID(int);//this is the autoID code
    // returns calibration information
    int Channel()       { return _Channel; };
    float VoltageID()   { return _VoltageID; }; 
    float Voltage()   { return _Voltage; }; 
    int SensorNumber() { return _SensorNumber; }; 
 //   int SN() { return _SN; }; 
    float slope()  { return _slope; }; 
    float intercept()        { return _intercept; }; 
    float cFactor()       { return _cFactor; }; 
    int CalEquationType()  { return _CalEquationType; };
    int page()            { return _page; }; 

    void ReadSensor();//This function converts count to sensor reading
    float SensorReading()     { return _SensorReading; };
    float logR() { return _logR; };
    long resistance() { return _resistance; };

protected://  also known as local
    int _Channel;
    float _VoltageID;
    float _Voltage;
    int _SensorNumber;

    float _slope;
    float _sum;
    float _intercept;
    float _cFactor;
    int _CalEquationType;
    int _page;
    int _i;
    int _x;
    int _device;
    int _sensordata[128];//should be a byte!!!
//    char _c[18];//name!!!
    int _Raw;
    float _SensorReading;
    float _logR;
    long _resistance;
    float _resistor;

};
#endif

// END OF FILE