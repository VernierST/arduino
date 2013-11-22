/*
Vernier3DA (v 2013.11)
This project assumes that you have a Vernier 3D Accelerometer connected to 
three BTA connectors. It also assumes that you have an RGB LED connected to
PWM outputs 3, 5, and 6.

Plug x into the first BTA (A0), y into the second BTA (A2), and z into
the third BTA (A4).
The brightness of the red LED is controlled by the x axis acceleration.
The brightness of the green LED is controlled by the y axis acceleration.
The brightness of the blue LED is controlled by the z axis acceleration.

The minReading, maxReading, and BlueAdj values are not essential,
but they were added to make better color mixes.

See www.vernier.com/arduino for more information.
*/
const int RedPin = 3;
const int GreenPin = 5;
const int BluePin = 6;

const int sensorPinRed = A0;
const int sensorPinGreen = A2;
const int sensorPinBlue = A4;

const int minReading = 490;
const int maxReading =560;
const int BlueAdj = 50;

int analogValue = 0;
int analogOut; 

void setup()
{ 
  Serial.begin(9600);  
} 
void loop() 
{
  //Red
  analogValue = analogRead(sensorPinRed);
  analogValue = constrain(analogValue, minReading, maxReading);
  analogOut = map(analogValue, minReading, maxReading, 0, 255);
  Serial.print("analogValue Red = " );                       
  Serial.print(analogValue, DEC);
  analogWrite(RedPin, analogOut);
  //Green
  analogValue = analogRead(sensorPinGreen);
  analogValue = constrain(analogValue, minReading, maxReading);
  analogOut = map(analogValue, minReading, maxReading, 0, 255);
  Serial.print("  Green = " );                       
  Serial.print(analogValue, DEC);
  analogWrite(GreenPin, analogOut);
  //Blue
  analogValue = analogRead(sensorPinBlue);
  analogValue = analogValue-BlueAdj;
  analogValue = constrain(analogValue, minReading, maxReading);
  analogOut = map(analogValue, minReading, maxReading, 0, 255);
  Serial.print("  Blue = " );     
  Serial.println(analogValue, DEC);
  analogWrite(BluePin, analogOut);
  delay(25);
}
