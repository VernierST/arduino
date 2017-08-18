/*
VernierCount (v 2017.07)
Monitors a Vernier Drop Counter or Vernier photogate connected to BTD connector.

The sketch sums the "Blocked" events in the period "countTime". As written, the 
countTime is 10 seconds.

See www.vernier.com/arduino for more information.

 */
int photogate = 2; // Pin assignment for photogate
int LEDpin = 13;/// line for LED to turn on when count is received.
int countTime = 10000;// this is the total time between reported results, the collect time
int intervalNumber =0;

void setup() 
{
  Serial.begin(9600);           // set up Serial library at 9600 bps
  pinMode(LEDpin, OUTPUT);
  Serial.println("Vernier Format 2");
  Serial.println("Count Readings taken using Ardunio");
  Serial.println("Data Set");
  Serial.print("Int"); //long name
  Serial.print("\t"); //tab character
  Serial.println ("Count"); //long name
  Serial.print ("I"); //short name 
  Serial.print("\t"); //tab character 
  Serial.println("s"); //short name
  Serial.print("#"); //units
  Serial.print("\t"); // tab character
  Serial.println ("#"); //units
};// end of setup

void loop ()
{
  unsigned long timemsStart;
  int countSum = 0;

  timemsStart = millis();
  while ((millis() - timemsStart) <= countTime)
  {
    if (digitalRead(photogate)==LOW) 
    { 
      digitalWrite(LEDpin, HIGH);// turn on LED
      countSum ++;
      while(digitalRead(photogate)==LOW);
      digitalWrite(LEDpin, LOW); //turn off LED
     }
  }
Serial.print(intervalNumber);
Serial.print("\t"); // tab character
Serial.println(countSum);
intervalNumber++;
} ;// end of loop



