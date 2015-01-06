/*
VernierRadiation (v 2014.12)
Monitors a Vernier Radiation Monitor connected to BTD1 connector.

The sketch sums the Radiation Monitor Counts in the period "CountTime". 
The default CountTime is 1 second.

See www.vernier.com/arduino for more information.
 */
unsigned long TimeMsStart = 0;
int rad = 2; // Pin assignment for radiation monitor
int LEDpin =13;/// line for LED to turn on when count is received.
int CountTime = 1000;// this is the total time between reported results
int IntervalNumber =0;// tracks how many 1 second intervals

void setup() 
{
  Serial.begin(9600);           // set up Serial library at 9600 bps
  pinMode(LEDpin, OUTPUT);
  Serial.println("Vernier Format 2");
  Serial.println("Radiation Count Readings taken using Ardunio");
  Serial.println("Data Set");
  Serial.print("Int");//long name
  Serial.print("\t"); //tab character
  Serial.println ("Count"); //long name
  Serial.print("I");//short name
  Serial.print("\t"); //tab character
  Serial.println ("C"); //short name
  Serial.print("#");
  Serial.print("\t"); // tab character
  Serial.println ("#"); 
};// end of setup

void loop ()
{
  int countSum = 0;
  
  TimeMsStart = millis();
  while ((millis() - TimeMsStart) <= CountTime)// for 1 second
  {
    if (digitalRead(rad)==HIGH) 
    { 
      digitalWrite(LEDpin, HIGH);// turn on LED
      countSum ++;// adds a count
      digitalWrite(LEDpin, LOW); //turn off LED so it has to be forced back on
     }
  }
Serial.print(IntervalNumber);
Serial.print("\t"); // tab character
Serial.println(countSum);
IntervalNumber++;
} ;// end of loop



