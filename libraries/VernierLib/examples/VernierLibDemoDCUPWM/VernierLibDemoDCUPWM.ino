//VernierLib Demo Program
#include "VernierLib.h" 
VernierLib Vernier;
byte PWMSetting= 0;

void setup()
{
  Serial.begin(9600);
  Vernier.autoID();// this is the routine to do the autoID
}

void loop()
{
  Serial.print(Vernier.readSensor());
  Serial.print(" ");
  Serial.println(Vernier.sensorUnits);
    // demononstrate PWM by fade in from min to max in increments of 5 points:
    for(int PWMSetting = 0 ; PWMSetting <= 255; PWMSetting +=5) 
    { 
      // sets the value (range from 0 to 255):
      Serial.println(PWMSetting);
      Vernier.DCUPWM(PWMSetting);//set PWM level 
      // wait for 30 milliseconds to see the dimming effect    
      delay(30);                            
    } 
     // fade out from max to min in increments of 5 points:
     for(int PWMSetting = 255 ; PWMSetting >= 0; PWMSetting -=5) 
     { 
       // sets the value (range from 0 to 255):
       Vernier.DCUPWM(PWMSetting);//set PWM level     
       // wait for 30 milliseconds to see the dimming effect    
       delay(30);                            
      } 
     delay(1000);//
   }


   
