/*
VernierDCU_simple  (v 2015.03)
Controls the Vernier Digital Control Unit (DCU) connected to BTD 2 connector. The 
DCU lines for BTD 2 are wired to Arduino pins 6, 7, 8, and 9.

In order to turn on individual lines of the DCU the following cases should be written to 
the pins 6, 7, 8, and 9.
D1 on: (high, Low, low, low) > This is Case 1
D2 on: (low, high, low, low) > This is Case 2
D3 on: (low, low, high, low) > This is Case 4
D4 on: (low, low, low, high) > This is Case 8
D5 on: (high, low, high, high) > This is Case 13
D6 on: (low, high, high, high) > This is Case 14

Other combinations to turn on multiple lines at once can be found in the DCU User Manual.

See www.vernier.com/arduino for more information. 

*/
int Duration=1000; //step time in ms
int value = 0;
//the lines below are so that you can quickly change this code if you want to
//use the DCU in the BTD1 connector for some reason.
int DCUinBTD2=1;// change this to 0 if you want to use the DCU on BTD 1
const int Pin1 = 2 +DCUinBTD2*4;
const int Pin2 = 3 +DCUinBTD2*4;
const int Pin3 = 4 +DCUinBTD2*4;
const int Pin4 = 5 +DCUinBTD2*4;
int output; //number sent to DCU
void setup() 
{
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("DCU Output");
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);
  pinMode(Pin4, OUTPUT);
}
void loop ()
{   
   value = analogRead(A0);
   Serial.println (value); 
   if(value > 545)
   {
   digitalWrite (Pin1, HIGH); 
   digitalWrite (Pin2, HIGH);    
   digitalWrite (Pin3, HIGH); 
   //digitalWrite (Pin4, HIGH); 
   delay (Duration);
   }
   else
   {
   
   digitalWrite (Pin1, LOW); 
   digitalWrite (Pin2, LOW); 
   digitalWrite (Pin3, LOW);
   //digitalWrite (Pin4, LOW); 
   //digitalWrite (Pin2, HIGH);
   delay (Duration*2);
   /*digitalWrite (Pin2, LOW); //
   digitalWrite (Pin3, HIGH);
   delay (Duration);
   digitalWrite (Pin3, LOW);
   digitalWrite (Pin4, HIGH);
   delay (Duration*2);*/
   }
 }
