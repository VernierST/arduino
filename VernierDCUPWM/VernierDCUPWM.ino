/*
 VernierDCUPMW
 This sketch uses Pulse-Width Modulation to vary the voltage
 on pin 3 of the BTD socket. It would normally be used with
 the Vernier DCU connected to BTD connector. 

We use this pin D3 on the DCU because it is the first of the 
digital output lines on the BTD connnector (as wired here)
which has the Pulse Width Modulation capability. This is just a slight
variation of the Ardunio examples sketch fading (see: http://arduino.cc/en/Tutorial/Fading)

See www.vernier.com/arduino for more information.
 
 */
int PWMPin = 6;    /* LED connected to digital pin 6 
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


