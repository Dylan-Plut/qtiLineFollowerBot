// Program to move the robot according to line position, as well as to pick and place a golf ball
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

bool phase2 = false;

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
  else if (SConLine && SRonLine && SLonLine) //First time Robot encounters "T" Intersection (spawnpoint), going left
  {
    digitalWrite(motorpinLF, LOW);
    digitalWrite(motorpinRF, HIGH);
    delay(800);
  }
  else if (SConLine && !SRonLine && SLonLine)//Positioning to pickup golfball
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
  Serial.println("Thinking about tank turning");

  while (!(SConLine && SRonLine && SLonLine))
  {
    // Read sensor values and update line detection variables
    int SL = analogRead(qti1);
    int SC = analogRead(qti2);
    int SR = analogRead(qti3);

    SLonLine = (SL >= 500);
    SRonLine = (SR >= 500);
    SConLine = (SC >= 500);

    Serial.println("Tank Turning");
    digitalWrite(motorpinLR, LOW);
    digitalWrite(motorpinRF, HIGH); // Tank Turn Left
  }
  Serial.println("Getting outta here");
  lemmeOutPls();
}

void lemmeOutPls()
{
  //Stop Movement
  digitalWrite(motorpinLR, LOW);
  digitalWrite(motorpinRF, LOW);
  delay(2000);

  //Reverse outta there
  digitalWrite(motorpinLR, HIGH);
  digitalWrite(motorpinRR, HIGH);
  delay(650);
  Serial.println("Reversing");
  digitalWrite(motorpinLR, LOW);
  delay(1000);
  digitalWrite(motorpinRR, LOW);
  phase2 = true;
}

void dropoff()
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
    else if (SConLine && SRonLine &&!SLonLine)
    {
      while (!SConLine || SRonLine) //MAY NEED TO BE REVISED
      {
        digitalWrite(motorpinRF, LOW);
        digitalWrite(motorpinLF, HIGH); 
      }
    }
  }
}

void loop() 
{
  move();

  if (phase2)
  {
    Serial.println("Dropping off");
    dropoff();
  }
}
