/*
 VernierPhotogate (v 2017.07)
 Monitors a Vernier Photogate connected to BTD 1 connector. 

This sketch lists the time that the photogate is blocked in 
microseconds and milliseconds since the program started running.

The reason for the two time units is that the microsecond counter overflow 
fairly frequently. Use whichever time you like in your versions of this.
  
See www.vernier.com/arduino for more information.

 */
unsigned long timems = 0; //Time in ms
unsigned long timeus = 0; //Time in us
int photogatePin =2; //
int photogate = HIGH;
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
  photogate = digitalRead(photogatePin);//low when blocked
   if (photogate == LOW)
   { 
    digitalWrite(LEDpin, HIGH);// turn on LED
        if (status == HIGH)
          {
          timems = millis() ;
          timeus = micros() ;
          Serial.print(timems);
          Serial.print("\t"); //tab character
          Serial.println(timeus);
           }
   }
   else digitalWrite(LEDpin, LOW);// turn off LED
   status = photogate;
 } ;// end of loop


