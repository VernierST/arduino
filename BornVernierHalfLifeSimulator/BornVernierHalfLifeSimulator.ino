/* Radioactive Decay Half-Life Simulator
   by Richard Born on May 31, 2015
   with modifications suggestions by Dave Vernier */

int decayOUT = 6;  // output from Digital 2 port on Vernier Sensor Interface Shield to Vernier Interface (e.g., LabQuest 2)
int decayLED = 13; // pin 13 on the Vernier Shield will flash when a simulated atom decays
float clock = 0;   // simulation clock
int startRate;     // initial decay rate in decays per second
float rate;        // decay rate at simulation 'clock' time
float reciprocalRate;  // time between decays in seconds
float sign;        // will be either 1 or -1 to provide some random variation in time between decays //  
float lambda;      // decay constant
int beginRate[] = {11, 23, 32}; // decays per second (one of these three is randomly selected on each run of the sketch
float decayConstant[] = {0.0125, 0.00625, 0.00357, 0.00227}; // one of these four is randomly selected on each run of the sketch  

void setup() {
  
  // declare pin modes
  pinMode(decayOUT, OUTPUT);
  pinMode(decayLED, OUTPUT);
  randomSeed(analogRead(10));  // need to use an unused pin on the Vernier Interface Shield to provide randomness
                               // even though 10 is digital, analogRead of this pin seems to provide
                               // for random results on each run of the sketch
  startRate = beginRate[random(0,3)];  // randomly select one of the three initial decay rates
  lambda = decayConstant[random(0,4)]; // randomly select on the the 4 decay constants
  
}

void loop() {

  rate = startRate*exp(-lambda*clock); // get decay rate at 'clock' time
  reciprocalRate = 1/rate; // get time between decays
  // sign will be 1 about 50% of the time and 0 the remaining 50%
  if (random(0,2) == 0) {
    sign = -1; // will decrease the reciprocal rate randomly
  } else {
    sign = 1;  // will increaes the reciprocal rate randomly
  }
  //  The following will vary the reciprocal rate an average of 20%
  //  with a minimum of 0% and a maximum of 40% 
  reciprocalRate *= 1.0 + sign * (float(random(2000))/5000.0);
  digitalWrite(decayLED, HIGH);  // turn on the decay LED
  digitalWrite(decayOUT, HIGH);  // send signal to Vernier interface via pin 6
  delay(1);                      // slight pause while LED is ON
  digitalWrite(decayLED, LOW);   // turn off the decay LED
  digitalWrite(decayOUT, LOW);   // lower the signal to the Vernier interface
  delay(reciprocalRate*1000);    // delay the simulation by an amount equal to the time between decays
  clock += reciprocalRate;       // increaase the simulation clock by the time between decays
  
}



