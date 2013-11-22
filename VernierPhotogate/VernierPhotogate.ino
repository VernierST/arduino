/*
 VernierPhotogate (v 2013.11)
 Monitors a Vernier Photogate connected to BTD connector. 

This sketch lists the time in microseconds and milliseconds since the
program started running.
The reason for the two time units is that the microsecond counter overflow 
fairly frequently. Use whichever time you like in your versions of this.
  
See www.vernier.com/arduino for more information.

 */
unsigned long Timems = 0; //Time in ms
unsigned long Timeus = 0; //Time in us
int PhotogatePin =2; //
int Photogate = HIGH;
int status;
int LEDpin =13;/// line for LED to turn on when photogate is blocked

void setup() 
  {
   Serial.begin(9600);           // set up Serial library at 9600 bps
    pinMode(LEDpin, OUTPUT);
    Serial.println("Vernier Format 2");
    Serial.println("Photogate blocked times taken using Ardunio");
    Serial.print("Time");
    Serial.print("\t"); //tab character
    Serial.println ("Time"); //change to match sensor
    Serial.print("ms");
    Serial.print("\t"); // tab character
    Serial.println ("microseconds");
  };// end of setup
void loop ()
{
  Photogate = digitalRead(PhotogatePin);//low when pressed
   if (Photogate == LOW)
   { 
    digitalWrite(LEDpin, HIGH);// turn on LED
        if (status == HIGH)
          {
          Serial.print(Photogate);
          Timems = millis() ;
          Timeus = micros() ;
          Serial.print(Timems);
          Serial.print("\t"); //tab character
          Serial.println(Timeus);
           }
   }
   else digitalWrite(LEDpin, LOW);// turn on LED
   status = Photogate;
 } ;// end of loop


