// Program to move the robot according to line position, as well as to pick and place a golf ball (LOCATION 1)
// Created by Dylan Plut
// Last modified 2024-01-17

// Input pins for sensors
const int qti1 = A1;
const int qti2 = A2;
const int qti3 = A3;

// Input pins for the Motors
const int motorpinLF = 13;
const int motorpinLR = 12;
const int motorpinRR = 11;
const int motorpinRF = 10;
const int motorpin3R = 9;
const int motorpin3F = 8;

//Sensor readings
bool SLonLine = false;
bool SRonLine = false;
bool SConLine = false;

void setup()
{
  pinMode(qti1, INPUT);
  pinMode(qti2, INPUT);
  pinMode(qti3, INPUT);
  
  Serial.begin(9600);
}

//Global varaibles to help determine logic for turning
bool phase1 = true;
bool phase2 = false;
bool phase3 = false;
bool done = false;


//The move function controlls motor-sensor logic from the start location untill ball pickup phase
void move()
{
  int SL = analogRead(qti1);
  int SC = analogRead(qti2);
  int SR = analogRead(qti3);
  
  SLonLine = (SL >= 500);
  SRonLine = (SR >= 500);
  SConLine = (SC >= 500);
  
  if (SConLine && !SRonLine && !SLonLine) // Moving Forwards
  {
    digitalWrite(motorpinLF, HIGH);
    digitalWrite(motorpinRF, HIGH);
  } 
  else if (!SConLine && !SRonLine && SLonLine) // Moving Left
  {
    while (!SConLine)
    {
      digitalWrite(motorpinLF, LOW);
      digitalWrite(motorpinRF, HIGH);

      int SL = analogRead(qti1);
      int SC = analogRead(qti2);
      int SR = analogRead(qti3);

      SLonLine = (SL >= 500);
      SRonLine = (SR >= 500);
      SConLine = (SC >= 500);
    }
  }
  else if (!SConLine && SRonLine && !SLonLine) // Moving Right
  {
    while (!SConLine)
    {
      digitalWrite(motorpinLF, HIGH);
      digitalWrite(motorpinRF, LOW);

      int SL = analogRead(qti1);
      int SC = analogRead(qti2);
      int SR = analogRead(qti3);

      SLonLine = (SL >= 500);
      SRonLine = (SR >= 500);
      SConLine = (SC >= 500);
    }
  }
  else if (SConLine && SRonLine && SLonLine) //First time Robot encounters "T" Intersection (spawnpoint), going Right
  {
    digitalWrite(motorpinLF, LOW);
    digitalWrite(motorpinRF, HIGH);
    delay(1000);
  }
  else if (SConLine && SRonLine && !SLonLine)//Positioning to pickup golfball
  {
    int count;
    count = 0;
    count +=1;
    if (count >= 1)
    {
      smartPickup();
    }
  }
}

//The smart pickup is a more optimized ball pickup function, controlling motor-sensor logic durnig the lineup, pickup, and escape procces
void smartPickup()
{
  //Stop Motors
  digitalWrite(motorpinLF, LOW);
  digitalWrite(motorpinRF, LOW);
  delay(1000);

  //Move forward to line up tank turn
  digitalWrite(motorpinLF, HIGH);
  digitalWrite(motorpinRF, HIGH);
  delay(400);
  

  digitalWrite(motorpinLF, LOW);
  digitalWrite(motorpinRF, LOW);

  while (!(SConLine && SRonLine && SLonLine))
  {
    // Read sensor values and update line detection variables
    int SL = analogRead(qti1);
    int SC = analogRead(qti2);
    int SR = analogRead(qti3);

    SLonLine = (SL >= 500);
    SRonLine = (SR >= 500);
    SConLine = (SC >= 500);

    digitalWrite(motorpinLR, HIGH);
    digitalWrite(motorpinRF, LOW); // Tank Turn Left
  }
  Serial.println("Getting outta here");
  lemmeOutPls();
}
unsigned long startTime;
void lemmeOutPls()
{
  //Stop Movement
  digitalWrite(motorpinLR, LOW);
  digitalWrite(motorpinRF, LOW);
  delay(2000);

  //Reverse outta there
  digitalWrite(motorpinLR, HIGH);
  digitalWrite(motorpinRR, HIGH);
  delay(500);
  Serial.println("Reversing");
  digitalWrite(motorpinRR, LOW);
  delay(950);
  digitalWrite(motorpinLR, LOW);
  phase2 = true;
  startTime = millis();
}

//Second phase of program, handles motor-sensor logic for lining up the dropoff of the ball
void lineup()
{
  bool droppedoff = false; 

  while (!droppedoff)
  {
    int SL = analogRead(qti1);
    int SC = analogRead(qti2);
    int SR = analogRead(qti3);
  
    SLonLine = (SL >= 500);
    SRonLine = (SR >= 500);
    SConLine = (SC >= 500);

    if (SConLine && !SRonLine && !SLonLine) // Moving Forwards
    {
      digitalWrite(motorpinLF, HIGH);
      digitalWrite(motorpinRF, HIGH);
    } 
    else if (!SConLine && !SRonLine && SLonLine) // Moving Left
    {
      while (!SConLine)
      {
        digitalWrite(motorpinLF, LOW);
        digitalWrite(motorpinRF, HIGH);

        int SL = analogRead(qti1);
        int SC = analogRead(qti2);
        int SR = analogRead(qti3);

        SLonLine = (SL >= 500);
        SRonLine = (SR >= 500);
        SConLine = (SC >= 500);
      }
    }
    else if (!SConLine && SRonLine && !SLonLine) // Moving Right
    {
      while (!SConLine)
      {
        digitalWrite(motorpinLF, HIGH);
        digitalWrite(motorpinRF, LOW);

        int SL = analogRead(qti1);
        int SC = analogRead(qti2);
        int SR = analogRead(qti3);

        SLonLine = (SL >= 500);
        SRonLine = (SR >= 500);
        SConLine = (SC >= 500);
      }
    }
    else if (SConLine && !SRonLine && SLonLine && millis()-startTime > 3500) //Right turn, lining up phase 3 
    {
      digitalWrite(motorpinLF, LOW);
      digitalWrite(motorpinRF, HIGH);
      Serial.println("About to delay");
      delay(850);
      while (!SConLine) 
      {
        Serial.println("In Loop");
        digitalWrite(motorpinRF, HIGH);
        digitalWrite(motorpinLF, LOW); 
      }
      droppedoff = true;
      Serial.println("Done");
    } 
  }
  digitalWrite(motorpinRF, LOW);
  digitalWrite(motorpinLF, LOW);
  phase3 = true;
}

//handles the final phase of the program, motor-sensor logic for dropping off the golf ball
void dropoff()
{
  int SL = analogRead(qti1);
  int SC = analogRead(qti2);
  int SR = analogRead(qti3);
  
  SLonLine = (SL >= 500);
  SRonLine = (SR >= 500);
  SConLine = (SC >= 500);

  if (SConLine && !SRonLine && !SLonLine) // Moving Forwards
  {
    digitalWrite(motorpinLF, HIGH);
    digitalWrite(motorpinRF, HIGH);
  } 
  else if (!SConLine && !SRonLine && SLonLine) // Moving Left
  {
    while (!SConLine)
    {
      digitalWrite(motorpinLF, LOW);
      digitalWrite(motorpinRF, HIGH);

      int SL = analogRead(qti1);
      int SC = analogRead(qti2);
      int SR = analogRead(qti3);

      SLonLine = (SL >= 500);
      SRonLine = (SR >= 500);
      SConLine = (SC >= 500);
    }
  }
  else if (!SConLine && SRonLine && !SLonLine) // Moving Right
  {
    while (!SConLine)
    {
      digitalWrite(motorpinLF, HIGH);
      digitalWrite(motorpinRF, LOW);

      int SL = analogRead(qti1);
      int SC = analogRead(qti2);
      int SR = analogRead(qti3);

      SLonLine = (SL >= 500);
      SRonLine = (SR >= 500);
      SConLine = (SC >= 500);
    }
  }
  else if (SConLine && SRonLine && SLonLine) //T intersection, move Left and stop
  {
    digitalWrite(motorpinRF, HIGH);
    digitalWrite(motorpinLF, LOW);
    delay(900);
    digitalWrite(motorpinLF, LOW);
    digitalWrite(motorpinRF, LOW);

    done = true;
  }
}

//main loop to be executed during program runtime
void loop() 
{
  if (phase1)
  {
    Serial.println("PHASE1");
    move();
  }
  if (phase2)
  {
    Serial.println("PHASE2");
    phase1 = false;
    lineup();
  }
  if (phase3)
  {
    
    phase2 = false;
    dropoff();
  }
  if (done) //Finish Program
  {
    Serial.println("Droppped off!");
    delay(100000);
  }
}