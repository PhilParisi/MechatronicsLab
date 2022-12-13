// Jason Noel & Phil Parisi - Mechatronics Final Project
   

// 'z' --> null state (does nothing, holds current state)

// Manual States
byte state0Entry = false;  // 0 --> 'dead' (system off)
byte state1Entry = false;  // 1 --> primed
byte state2Entry = false;  // 2 --> turning left
byte state3Entry = false;  // 3 --> turning right
byte state4Entry = false;  // 4 --> drive forward
byte state5Entry = false;  // 5 --> drive backward

// Autonomous States
byte state10Entry = false; // 10 --> autonomous mode
byte state11Entry = false; // 11 --> auto forward
byte state12Entry = false; // 12 --> auto reverse
byte state13Entry = false; // 13 --> auto turn


// Immutable 'Variables'
#define bridgeForwardRight 2  // (Hbridge connection 4A)
#define bridgeBackwardRight 4  // (Hbridge connection 3A)
#define bridgeForwardLeft 8  // (Hbridge connection 1A)
#define bridgeBackwardLeft 7  // (Hbridge connection 2A)
#define bridgePWM 9 //PWM pin (Hbridge connection 1,2EN and 3,4EN)

#define LED1 5 //PWM pin
#define LED2 6 //PWM pin              
#define echoPin 11 // attach pin D2 Arduino to pin Echo of JSN-SR04T (PWM)
#define trigPin 12 //attach pin D3 Arduino to pin Trig of JSN-SR04T

#define slowMotorPWM 200  // these probably need adjustment based on what it takes to move the motors
#define fastMotorPWM 255

#define serialCheckTimeInterval 50  // time in ms
#define turnTimeInterval 1000 // time in ms NEEDS TO BE TUNED

#define minimumSensorDistance 25  // 25cm stopping distance
#define maxSensorHits 10

// Mutable 'Global Variables'
byte state;  // State and NextState of 1st task transition diagram
byte nextState;
byte returnToState; // state we want to go back to after turning

long duration; // variable for the duration of sound wave travel
long distance; // variable for the distance measurement

char command;       // User command
char progMode;      // Current control state 'm' for manual ; 'a' for auto
unsigned long startTime;  // Variable to record starting time

float turnTime;
float serialTime;
float autoTimer;
float backTime;
long objectDistance;

int sensorHitCounter; // for counting when the sensors records values that are 'too close'

// Function Declarations  (Prototypes)
void controlTask(void);         // Task state transition diagram function
void controlHbridgePWM();
unsigned long getTimeNow(void);  // Returns time in ms units
long sensorPing(void);          // for using the foward-looking sensor
void motorSetSpeed(int val);
void turnLeft();
void turnRight();
void moveForward();
void moveBackward();
void objectAvoidance(int distance);
//void inoToPython(int distance);

// Setup Function
void setup() {
  
  // LED Pin Setup
  pinMode(LED1, OUTPUT);  // Set up D/O line for output
  pinMode(LED2, OUTPUT);
  
  // LED Pin Initial Values
  digitalWrite(LED1, LOW);  // Set all LEDs to off to start
  digitalWrite(LED2, LOW);

  // H-Bridge Pin Setup
  pinMode(bridgeForwardRight, OUTPUT);
  pinMode(bridgeBackwardRight, OUTPUT);     // HIGH and LOW values
  pinMode(bridgeForwardLeft, OUTPUT);
  pinMode(bridgeBackwardRight, OUTPUT);
  pinMode(bridgePWM, OUTPUT);               // PWM for speed control

  // H-Bridge Pin Initial Values [all low]
  digitalWrite(bridgeForwardRight, LOW);
  digitalWrite(bridgeBackwardRight, LOW);
  digitalWrite(bridgeForwardLeft, LOW);
  digitalWrite(bridgeBackwardLeft, LOW);
  motorSetSpeed(slowMotorPWM); // sets bridgePWM pin to slowMotorPWM speed as default
  
  // Ultrasonic Sensor Setup
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baud rate speed

  // Control Flow Setup
  char command = 'z';       // User command
  char progMode = 'm';      // Current control state 'm' for manual ; 'a' for auto
  nextState = 0;            // start in null 'dead' state

}


// Main Loop
void loop() {
  
  // Starting Timer for Serial Connection
  serialTime = getTimeNow();  // for serial check time interval

  // Infinite While Loop
  while (2 > 1)  // Start infinite loop
  {

    if ((getTimeNow() - serialTime) >= serialCheckTimeInterval)  // This if statement is used to not check the serial port all the time
    {
      serialTime = getTimeNow();
      // Code to read command (a single char) from serial port here
      if (Serial.available() > 0)  //Wait for a char to show up
      {
        command = Serial.read();  // Read one byte from serial buffer
      }
    }


  // Tasks for Every Loop
  controlTask(); //  Call ControlTask 1 (looks for commands of the letter format like 's' 'f' 'b')
  controlHbridgePWM(); // allow commands sent from GUI to update the PWM signal for hbridge (commands of the form '1' for slow and '2' for fast)
  //objectDistance = sensorPing();  // ultrasonic sensor sends pings, returns distance to nearest object [centimeters!]
  //objectAvoidance(objectDistance);
  
  // Reset command
  command = 'z';   // reset this so commands are executed only once
  }
}

// Definition of ControlTask Function
void controlTask(void) {

  state = nextState;

  // Jump between STATES
  switch (state)  // Go to current state
  {

    case 0: // System Off 'Dead'

      if (state0Entry == false)  // if not in the state, get in it!
      {
        state0Entry = true;
        stopMotors();
      }

      if (command == 'i')  // 'initialize', move to State 1! 
      {
        nextState = 1;        // change current state
        state0Entry = false;  // we're leaving the state, set it to false
      }


      break;


    case 1: // Primed & Stopped

      if (state1Entry == false)  // if not in the state, get in it!
      {
        state1Entry = true;  // we are in state 1
        stopMotors();
      }

      if (command == 'l')  // 'left', move to state 2 (turn left)
      {
        returnToState = 1;
        nextState = 2;                
        state1Entry = false;         
      }

      if (command == 'r')  // 'right', move to state 3 (turn right)
      {
        returnToState = 1;
        nextState = 3;        
        state1Entry = false;  
      }
	  
      if (command == 'f')  // 'forward', move to state 4 (move forward)
      {
        nextState = 4;                
        state1Entry = false;         
      }

      if (command == 'b')  // 'reverse', move to state 5 (move backward)
      {
        nextState = 5;        
        state1Entry = false;  
      }

      if (command == 'a') // 'autonomous enable'
      {
        nextState = 10;
        progMode = 'a';
        state1Entry = false;
      }

      break;


    case 2: // turn left 90deg
	
      if (state2Entry == false)  // if not in the state, get into it
      {
		    turnTime = getTimeNow();
        state2Entry = true;
		    turnLeft();
      }

      if (command == 's')  // Stop Task!
      {
        nextState = 1;        // get into state 1
        state2Entry = false;  // get out of state 2
        turnTime = 0;
      }

      if ((getTimeNow() - turnTime) >= turnTimeInterval)  // we've been learning for long enough!
      {
        nextState = returnToState;      // go back to previous state
        state2Entry = false;            // get out of current state
        turnTime = 0;
      }

      break;

    case 3: // turn right 90deg                         
      if (state3Entry == false)  // if not in the state, get into it
      {
        turnTime = getTimeNow();
        state3Entry = true;
        turnRight();
      }

      if (command == 's')  // stop
      {
        nextState = 1;        // 
        state3Entry = false;  // get out of state 2
        turnTime = 0;
      }

      if ((getTimeNow() - turnTime) >= turnTimeInterval)  // we've been turning for long enough!
      {
        nextState = returnToState;      // go back to previous state
        state3Entry = false;            // get out of current state
        turnTime = 0;
      }

      break;


    case 4: // drive forward                           
      if (state4Entry == false)  // if not in the state, get into it
      {
        state4Entry = true;
        driveForward();
      }

      if (command == 's')   // stop
      {
        nextState = 1;
        state4Entry = false;  // get out of state 4
      }

      if (command == 'l')   // turn left
      {
        returnToState = 4; // come back to driving forward
        nextState = 2; // turn left
        state4Entry = false;  // get out of state 4
      }

      if (command == 'r')   // turn right
      {
        returnToState = 4; // come back to driving forward
        nextState = 3;
        state4Entry = false;  // get out of state 4
      }

      if (command == 'b')   // go backwards
      {
        nextState = 5;
        state4Entry = false;
      }

      break;


    case 5: // drive backward                           
      if (state5Entry == false)  // if not in the state, get into it
      {
        state5Entry = true;
        driveBackward();
      }

      if (command == 's')   // stop
      {
        nextState = 1;
        state5Entry = false;  // get out of state 5
      }

      if (command == 'l')   // turn left
      {
        returnToState = 5;
        nextState = 2;
        state5Entry = false;  // get out of state 5
      }

      if (command == 'r')   // turn right
      {
        returnToState = 5;
        nextState = 3;
        state5Entry = false;  // get out of state 5
      }

      if (command == 'f')   // go forwards
      {
        nextState = 4;
        state5Entry = false;
      }

      break;


    case 10: // Autonomous Primed (countdown state before moving autonomously)
      if  (state10Entry == false)
      {
        state10Entry = true;
        autoTimer = getTimeNow();
      }
      
      if ((getTimeNow() - autoTimer) >= 3)
      {
        nextState = 11;
        state10Entry = false;
      }
      
      if (command == 's')
      {
        nextState = 1;
        progMode = 'm';
        state10Entry = false;
      }

      break;
      
    case 11: // Autonomous Forward
      if (state11Entry == false)
      {
        state11Entry = true;
        driveForward();
      }
      
      if (command == 's')
      {
        nextState = 1;
        progMode = 'm';
        state11Entry = false;
      }

      // need to build bridge to state 12
      break;
      
    case 12: // Autonomously Avoid Object by Backing Up
      if (state12Entry == false)
      {
        state12Entry = true;
        backTime = getTimeNow();
        driveBackward();
      }

      if (command == 's')
      {
        nextState = 1;
        state12Entry = false;
        progMode = 'm';
      }
      
      if ((getTimeNow() - backTime) >= 2) // back up for a certain period of time
      {
       stopMotors();
       backTime = 0;
       nextState = 13;
       state12Entry = false; 
      }

      break;

    case 13: // Autonomous Avoid Object by Backing Up 
      if (state13Entry == false)
      {
        state13Entry = true;
        turnLeft();
        turnTime = getTimeNow();
      }
      
      if ((getTimeNow() - turnTime) >= turnTimeInterval)
      {
        nextState = 11;
        state12Entry = false;
        turnTime = 0;
        stopMotors();
      }
      
      if (command == 's')
      {
        nextState = 1;
        progMode = 'm';
        state13Entry = false;
        turnTime = 0;
      }
      break;

  }
}


unsigned long getTimeNow(void)  // Returns time in ms units
{
  return (millis());
}

void driveForward()
{
  // to move forward...
  digitalWrite(bridgeForwardRight, HIGH); // right side foward
  digitalWrite(bridgeBackwardRight, LOW);

  digitalWrite(bridgeForwardLeft, HIGH); // left side foward
  digitalWrite(bridgeBackwardLeft, LOW);
}

void driveBackward()
{
  // to move backward...
  digitalWrite(bridgeForwardRight, LOW); // right side backward
  digitalWrite(bridgeBackwardRight, HIGH);

  digitalWrite(bridgeForwardLeft, LOW); // left side backward
  digitalWrite(bridgeBackwardLeft, HIGH);
}

void turnLeft()
{
  // to turn left...
  digitalWrite(bridgeForwardRight, LOW); // right side backward
  digitalWrite(bridgeBackwardRight, HIGH);

  digitalWrite(bridgeForwardLeft, HIGH); // left side forward
  digitalWrite(bridgeBackwardLeft, LOW);
}

void turnRight()
{
  // to turn right...
  digitalWrite(bridgeForwardRight, HIGH); // right side foward
  digitalWrite(bridgeBackwardRight, LOW);

  digitalWrite(bridgeForwardLeft, LOW); // left side backward
  digitalWrite(bridgeBackwardLeft, HIGH);
}

void stopMotors()
{
  // to stop motors...
  digitalWrite(bridgeForwardRight, LOW); // right side backward
  digitalWrite(bridgeBackwardRight, LOW);

  digitalWrite(bridgeForwardLeft, LOW); // left side backward
  digitalWrite(bridgeBackwardLeft, LOW);
}

void controlHbridgePWM()
{
  if (command == '1') // slow
  {
    motorSetSpeed(slowMotorPWM);
  }
  if (command == '2') // fast
  {
    motorSetSpeed(fastMotorPWM);
  }
}


void motorSetSpeed(int val)
{
  analogWrite(bridgePWM, val);
}

long sensorPing(void) // returns distance in cm from ultrasonic sensor
{
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);  //
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.0343 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.print(distance);
  //Serial.println(" cm");// working  code for aj-sr04m
  
  return distance;
}

void objectAvoidance(int distance)
{
  // to account for noise, we have to count up to maxSensorHits in a row before stopping!
  if (distance <= minimumSensorDistance)
  {
    sensorHitCounter = sensorHitCounter + 1;
  } 
  else 
  {
    sensorHitCounter = 0;
  }

  // send to 'stop' state 1 if we go above our max limit
  if (sensorHitCounter >= maxSensorHits)
  {
    if (progMode == 'm')
    {
      nextState = 1; // send program to 'stop' because we're too close!
      sensorHitCounter = 0; 
    }
    if (progMode == 'a')
    {
      nextState = 12;
      sensorHitCounter = 0;
    }
  }
}

/*void inoToPython(int distance)
{
  Serial.write(distance);
}*/
