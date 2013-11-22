/*
VernierCount (v 2013.11)
Monitors a Vernier Drop Counter or Vernier photogate connected to BTD connector.

The sketch sums the "Blocked" events in the period "CountTime". As written, the 
CountTime is 10 seconds.

See www.vernier.com/arduino for more information.

 */
unsigned long TimeMsStart = 0;
int photogate = 2; // Pin assignment for photogate
int LEDpin =13;/// line for LED to turn on when count is received.
int countSum = 0;
int IntervalNumber =0;
int CountTime = 10000;// this is the total time between reported results, the collect time

void setup() 
{
  Serial.begin(9600);           // set up Serial library at 9600 bps
  pinMode(LEDpin, OUTPUT);
  Serial.println("Vernier Format 2");
  Serial.println("Count Readings taken using Ardunio");
  Serial.println("Data Set");
  Serial.print("Int");
  Serial.print("\t"); //tab character
  Serial.println ("Count"); //change to match sensor
  Serial.print("#");
  Serial.print("\t"); // tab character
  Serial.println ("#"); //change to match sensor
};// end of setup
void loop ()
{
  countSum = 0;
  TimeMsStart = millis();
  while ((millis() - TimeMsStart) <= CountTime)
  {
    if (digitalRead(photogate)==LOW) 
    { 
      digitalWrite(LEDpin, HIGH);// turn on LED
      countSum ++;
      while(digitalRead(photogate)==LOW);
      digitalWrite(LEDpin, LOW); //turn off LED
     }
  }
Serial.print(IntervalNumber);
Serial.print("\t"); // tab character
Serial.println(countSum);
IntervalNumber++;
} ;// end of loop



