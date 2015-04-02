/*
Vernier3DA (v 2014.12)
This project assumes that you have a Vernier 3D Accelerometer connected to 
three BTA connectors. It also assumes that you have an RGB LED connected to
PWM outputs 3, 5, and 6.

Plug x into the first BTA (A0), y into the second BTA (A2), and z into
the third BTA (A4).
The brightness of the red LED is controlled by the x axis acceleration.
The brightness of the green LED is controlled by the y axis acceleration.
The brightness of the blue LED is controlled by the z axis acceleration.

During the first 15 seconds of this program you should rotate the 
accelerometer through all 3 axes. This will calibrate the signal for
you specific accelerometer. All LEDs will flash for 1 second at the beginning 
and end of this "calibration".
After calibration, each color should progress through a range of "off" to 
full power based on the orientation of the accelerometer. Sitting flat on a 
surface (z-axis up) is blue, flat on its side (y-axis up) is green, and cord
facing down is red.
Note that either individual LEDs or a RGB LED may be used.

In order to experiment with 2 colors one of the LEDs must be disconnected from 
the digital output (Redpin, Greenpin, or Bluepin).

See www.vernier.com/arduino for more information.
*/
const int RedPin = 3;
const int GreenPin = 5;
const int BluePin = 6;

const int sensorPinRed = A0;
const int sensorPinGreen = A2;
const int sensorPinBlue = A4;

int var = 1;
int minReadingRed = 750; // High value to force the minimum 
int maxReadingRed = 0; // Low value to force the maximum

int minReadingGreen = 750;
int maxReadingGreen = 0;
 
int minReadingBlue = 750;
int maxReadingBlue = 0;

int analogValue = 0;
int time;
int analogOut; 
int flash;

void setup()
{ 
  Serial.begin(9600);  
   Serial.println();
} 
void loop() 
{
  
  while (time < 1000){ // LEDs to flash at start of calibration 
  time = millis();
  analogWrite(RedPin, 255);
  analogWrite(GreenPin, 255);
  analogWrite(BluePin, 255);
  delay (1000);
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 0);
  delay (10);}
  
  while (time>1000 && time < 16000){ // Time to calibrate each sensor to it's particular range of values.
  time = millis();
   //Red
    analogValue = analogRead(sensorPinRed);
    maxReadingRed = max(analogValue, maxReadingRed);
    minReadingRed = min(analogValue, minReadingRed);// This is the absolute min. We'll want midpoint
    Serial.print("Analog Red "); 
    Serial.print(analogValue, DEC);
    delay (20);
    //Green
    analogValue = analogRead(sensorPinGreen);
    maxReadingGreen = max(analogValue, maxReadingGreen);
    minReadingGreen = min(analogValue, minReadingGreen);// This is the absolute min. We'll want midpoint
    Serial.print("   Analog Green "); 
    Serial.print(analogValue, DEC);
    delay (20);
    //Blue
    analogValue = analogRead(sensorPinBlue);
    maxReadingBlue = max(analogValue, maxReadingBlue);
    minReadingBlue = min(analogValue, minReadingBlue);// This is the absolute min. We'll want midpoint    
    Serial.print("   Analog Blue "); 
    Serial.println(analogValue, DEC);
    delay (20);
  }
  while(time > 16000 && time < 17000){// LEDs to flash at finish of calibration 
  time = millis();
  analogWrite(RedPin, 255);
  analogWrite(GreenPin, 255);
  analogWrite(BluePin, 255);
  delay (1000);
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 0);}
    
  while (var == 1){
  
  minReadingRed = minReadingRed+0.5*(maxReadingRed - minReadingRed); // Sets midpoint as minimum
  minReadingGreen = minReadingGreen+0.5*(maxReadingGreen - minReadingGreen); 
  minReadingBlue = minReadingBlue+0.5*(maxReadingBlue - minReadingBlue); 
  
  Serial.println();
  Serial.print("minReadingRed = " );
  Serial.print(minReadingRed, DEC);
  Serial.print("     maxReadingRed = " );
  Serial.println(maxReadingRed, DEC);
  Serial.print("minReadingGreen = " );
  Serial.print(minReadingGreen, DEC);
  Serial.print("     maxReadingGreen = " );
  Serial.println(maxReadingGreen, DEC);
  Serial.print("minReadingBlue = " );
  Serial.print(minReadingBlue, DEC);
  Serial.print("     maxReadingBlue = " );
  Serial.println(maxReadingBlue, DEC);
  var = 2;
  }
  // Below is scrolling printout
  
  //Red
  
  analogValue = analogRead(sensorPinRed);
  analogValue = constrain(analogValue, minReadingRed, maxReadingRed);
  Serial.print("analogValue Red = " ); 
  Serial.print(analogValue);     
  
  analogOut = map(analogValue, minReadingRed, maxReadingRed, 0, 255);
  analogOut = constrain(analogOut, 0, 255);  
  Serial.print(" scaled to " );
  Serial.print(analogOut, DEC);
  analogWrite(RedPin, analogOut);
    
 //Green
       
  analogValue = analogRead(sensorPinGreen);
  analogValue = constrain(analogValue, minReadingGreen, maxReadingGreen);
  Serial.print("  Green = " ); 
  Serial.print(analogValue);  
  
  analogOut = map(analogValue, minReadingGreen, maxReadingGreen, 0, 255);   
  analogOut = constrain(analogOut, 0, 255);  
  Serial.print(" scaled to " ); 
  Serial.print(analogOut, DEC);
  analogWrite(GreenPin, analogOut);
  
    //Blue
  
  analogValue = analogRead(sensorPinBlue);
  analogValue = constrain(analogValue, minReadingBlue, maxReadingBlue);
  Serial.print("  Blue = " );  
  Serial.print(analogValue); 
  
  analogOut = map(analogValue, minReadingBlue, maxReadingBlue, 0, 255);
  analogOut = constrain(analogOut, 0, 255);
  Serial.print(" scaled to " );   
  Serial.println(analogOut, DEC);
  analogWrite(BluePin, analogOut);
  
  delay (25);

  /* TEST AREA ABOVE!! Don't delete below just yet!
  //Red
  analogValue = analogRead(sensorPinRed);
  analogOut = map(analogValue, minReadingRed, maxReadingRed, 0, 255);
  Serial.print("Red = " );                       
  Serial.print(analogValue, DEC);
  analogWrite(RedPin, analogOut);
  
  //Green
  analogValue = analogRead(sensorPinGreen);
  analogOut = map(analogValue, minReadingGreen, maxReadingGreen, 0, 255);
  Serial.print("Green = " );                       
  Serial.print(analogValue, DEC);
  analogWrite(GreenPin, analogOut);
  
  //Blue
  analogValue = analogRead(sensorPinBlue);
  analogOut = map(analogValue, minReadingBlue, maxReadingBlue, 0, 255);
  Serial.print("Blue = " );     
  Serial.println(analogValue, DEC);
  analogWrite(BluePin, analogOut);
 
  delay(25); */
}
