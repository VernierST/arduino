/*
VernierRadiation (v 2013.11)
Monitors a Vernier Radiation Monitor connected to BTD connector.

The sketch sums the Radiation Monitor Counts in the period "CountTime". 
The default CountTime is 1 second.

See www.vernier.com/arduino for more information.
 */
unsigned long TimeMsStart = 0;
int photogate = 2; // Pin assignment for radiation monitor
int LEDpin =13;/// line for LED to turn on when count is received.
int countSum = 0;
int IntervalNumber =0;
int CountTime = 1000;// this is the total time between reported results, the collect time
void setup() 
{
  Serial.begin(9600);           // set up Serial library at 9600 bps
  pinMode(LEDpin, OUTPUT);
  Serial.println("Vernier Format 2");
  Serial.println("Radiation Count Readings taken using Ardunio");
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
    if (digitalRead(photogate)==HIGH) 
    { 
      digitalWrite(LEDpin, HIGH);// turn on LED
      countSum ++;
      while(digitalRead(photogate)==HIGH);
      digitalWrite(LEDpin, LOW); //turn off LED
     }
  }
Serial.print(IntervalNumber);
Serial.print("\t"); // tab character
Serial.println(countSum);
IntervalNumber++;
} ;// end of loop



