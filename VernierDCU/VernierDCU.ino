/*
VernierDCU  (v 2013.11)
Controls the Vernier Digital Control Unit (DCU) connected to BTD connector. The 
DCU lines for BTA 1 are wired to pins 2, 3, 4, and 5.

This sketch goes through the 16 possible DCU output states, holding each for
one second.
  
See www.vernier.com/arduino for more information.int 

*/
int Duration=1000; //step time in ms
const int Pin1 = 2;
const int Pin2 = 3; 
const int Pin3 = 4;
const int Pin4 = 5; 
int output; //number sent to DCU
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);
  pinMode(Pin4, OUTPUT);
}
void loop ()
{
  for (output=0; output<=15; output++)
  {
    DCU(output);
    Serial.println(output);
    delay (Duration);
   }
}
void DCU (int output)
{
  switch (output) 
  {
    case 0:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
      break;
    case 1:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
      break;
    case 2:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
    break;   
    case 3:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
      break;
    case 4:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, LOW);
      break;    
    case 5:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, LOW);
      break;
    case 6:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, LOW);
      break;
    case 7:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, LOW);
      break;
    case 8:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, HIGH);
      break;
    case 9:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, HIGH);
      break;
    case 10:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, HIGH);
      break;
    case 11:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, HIGH);
      break;
    case 12:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, HIGH);
      break;
    case 13:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, HIGH);
      break;
    case 14:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, HIGH);
      break;
    case 15:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, HIGH);
      break;
  }
}
