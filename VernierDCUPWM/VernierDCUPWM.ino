/*
 VernierDCUPMW (v 2014.09)
 This sketch uses Pulse-Width Modulation to vary the voltage
 on an output of the DCU which is connected to the BTD 2 socket.
 
Only certain pins on the Arduino are set up pulse-width modulation.
Here we use this pin D6 on the Arduino, which controls DCU output line 1
if the DCU is plugged into the BTD 2 connector.

This sketch is a variation of the Ardunio examples sketch fading
(see: http://arduino.cc/en/Tutorial/Fading)

See www.vernier.com/arduino for more information.
 
 */

int PWMPin = 6; // this sets the right pin for PWM assuming DCU is in BTD 2

void setup()  
{ 
  Serial.begin(9600); //initialize serial communication at 9600 baud
} 
void loop()  
{ 
  // fade in from min to max in increments of 5 points:
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) 
  { 
    // sets the value (range from 0 to 255):
   Serial.println(PWMPin);
   analogWrite(PWMPin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 
// fade out from max to min in increments of 5 points:
for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) 
  { 
// sets the value (range from 0 to 255):
analogWrite(PWMPin, fadeValue);         
// wait for 30 milliseconds to see the dimming effect    
delay(30);                            
  } 
}


