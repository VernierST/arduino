/*
 VernierDCUPMW (v 2013.11)
 This sketch uses Pulse-Width Modulation to vary the voltage
 on an output of the DCU which is connected to the BTD1 socket.
 
Only certain pins on the Arduino are set up pulse-width modulation.
Here we use this pin D3 on the Arduino, which controls DCU output line 2
if the DCU is plugged into the BTD1 connector.

This sketch is a variation of the Ardunio examples sketch fading
(see: http://arduino.cc/en/Tutorial/Fading)

See www.vernier.com/arduino for more information.
 
 */
int PWMPin = 3;    /* LED connected to digital pin 3 
*/
void setup()  
{ 
  // nothing happens in setup 
} 
void loop()  
{ 
  // fade in from min to max in increments of 5 points:
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) 
  { 
    // sets the value (range from 0 to 255):
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


