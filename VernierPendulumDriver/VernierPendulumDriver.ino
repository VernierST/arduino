/*
VernierPendulumDriver (v 2015.03)
This sketch assumes that a Vernier Photogate is connected to BTD 1 and a 
Vernier Digital Control Unit (DCU) is connected to BTD2.

The sketch is designed to drive a pendulum with a magnetic bob to 
keep it swinging "forever". When the pendulum passes through the Photogate, it
triggers a short digital output from the DCU to an electromagnet. The 
electromagnet should be situated just below the low point of the pendulum bob's
swing. The photogate should be situated so that the bob goes through it as it 
moves downward. The electromagnet will go on to pull the bob downward to add
energy to the pendulumn system.

The sketch also flashes an LED connected to D13 when the photogate is blocked,
and notes the times at which the pendulum blocks the photogate.

Start the sketch and then start the pendulum swinging with the bob going
through the photogate before it reaches the bottom of the swing.

See www.vernier.com/arduino for more information.

 */
unsigned long Timems = 0; //Time in ms
unsigned long PreviousTime= 0;// remember the previous time
int PhotogatePin =2; //
int Photogate = HIGH;// not blocked is HIGH
int PreviousPhotogate=HIGH;

int Count=1; //used to determine which direction the pendulum is swinging
int LEDpin =13;/// line for LED to turn on when photogate is blocked
int DCUline = 6;//This line will turn on the first line of the DCU, if
// the DCU is connected to BTD2
void setup() 
  {
    Serial.begin(9600);           // set up Serial library at 9600 bps
    pinMode (PhotogatePin,INPUT);
    pinMode(DCUline, OUTPUT);
    pinMode(LEDpin, OUTPUT);
    Serial.println("Vernier Format 2");
    Serial.println("Photogate periods taken using Ardunio");
    Serial.print("Swing");
    Serial.print("\t"); //tab character
    Serial.println ("Time"); //change to match sensor
    Serial.print("#");
    Serial.print("\t"); // tab character
    Serial.println ("milliseconds");
  };// end of setup
  
void loop ()
{
  Photogate = digitalRead(PhotogatePin);//low when blocked
  if (Photogate == LOW) //low when blocked
   { 
     digitalWrite(LEDpin, HIGH);
     if (PreviousPhotogate == HIGH)  // if the photogate has just gone to the blocked state
         { 
          Count++; //increment count of times photogate has been blocked
          if (Count % 2 == 0)// if the bob is moving in the right direction
            {
              Timems = millis() ;
              digitalWrite(DCUline, HIGH); // turn on DCU line 1 and the electromagnet DCU line
              // and print out the swing number and period
              Serial.print(Count);
              Serial.print("\t"); // tab character
              Serial.println(Timems-PreviousTime);
              PreviousTime= Timems; // remember the previous time
             }// end of the section for action take at the end of the complete swing
           } // end of photogate just gone blocked section
      }// end of if photogate blocked
      else //photogate not blocked:
        {
          digitalWrite(DCUline, LOW);// turn off DCU line 1 and the electromagnet
          digitalWrite(LEDpin, LOW);// turn off LED
         }
       PreviousPhotogate = Photogate;
     } ;// end of loop


