/*
VernierFrequency (v 2013.11)

This sketch produces a tone on a speaker connected to pin D9 of
the Arduino. It is based on the Tone 2 tutorial.
(see http://arduino.cc/en/Tutorial/Tone2).

Note that you do not deal with calibration here. Any Vernier
Analog sensor which uses the Sig1 (0 to 5 volt) line will work
if it is plugged into the BTA connector.

If you want to change the range of frequencies, change the last two
numbers in the line:  
int thisPitch = map(sensorReading, 400, 1000, 120, 1500)
They are the minimum and maximum frequencies produced.
If you want to change the sensitiviy of the system, change the first
two numbers of the same line. They are the range of expected readings 
from the sensor.

See www.vernier.com/arduino for details.

*/
void setup() 
{
  // initialize serial communications (for debugging only):
  Serial.begin(9600);
}

void loop() {
  // read the sensor:
  int sensorReading = analogRead(A0);
  // print the sensor reading so you know its range
  Serial.println(sensorReading);
  // map the analog input range (in this case, 400 - 1000 from the photoresistor)
  // to the output pitch range (120 - 1500Hz)
  // change the minimum and maximum input numbers below
  // depending on the range your sensor's giving:
  int thisPitch = map(sensorReading, 400, 1000, 120, 1500);

  // play the pitch:
  tone(9, thisPitch, 10);
  delay(1);        // delay in between reads for stability
}

