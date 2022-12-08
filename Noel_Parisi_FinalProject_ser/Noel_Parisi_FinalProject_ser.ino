// Jason Noel & Phil Parisi - Mechatronics Final Project

// Commands

// 'z' --> null state (does nothing, holds current state)


// States
byte state0Entry = false;  // 0 --> 'dead' (system off)
byte state1Entry = false;  // 1 --> primed
byte state2Entry = false;  // 2 --> turning left
byte state3Entry = false;  // 3 --> turning right
byte state4Entry = false;  // 4 --> drive forward
byte state5Entry = false;  // 5 --> drive backward
byte state6Entry = false;  // 6 --> emergency stop
byte state7Entry = false;  // 7 --> autonomous mode


// Immutable 'Variables'
#define bridgeForwardRight 2  // (Hbridge connection 4A)
#define bridgeBackwardRight 4  // (Hbridge connection 3A)
#define bridgeForwardLeft 8  // (Hbridge connection 1A)
#define brdigeBackwardLeft 7  // (Hbridge connection 2A)
#define brdigePWM 9 //PWM pin (Hbridge connection 1,2EN and 3,4EN)

#define LED1 5 //PWM pin
#define LED2 6 //PWM pin              
#define echoPin 11 // attach pin D2 Arduino to pin Echo of JSN-SR04T (PWM)
#define trigPin 12 //attach pin D3 Arduino to pin Trig of JSN-SR04T

#define slowMotorPWM 500  // these probably need adjustment based on what it takes to move the motors
#define fastMotorPWM 1023

#define serialCheckTimeInterval 50  // time in ms
#define turnTimeInterval 1000 // time in ms NEEDS TO BE TUNED

// Mutable 'Global Variables'
byte state;  // State and NextState of 1st task transition diagram
byte nextState;
byte returnToState; // state we want to go back to after turning

long duration; // variable for the duration of sound wave travel
long distance; // variable for the distance measurement

char command = 'z';       // User command
unsigned long startTime;  // Variable to record starting time

float turnTime;
float stateTime;
float serialTime;

// Function Declarations  (Prototypes)
void controlTask(void);         // Task state transition diagram function
unsigned long getTimeNow(void);  // Returns time in ms units
long sensorPing(void);          // for using the foward-looking sensor

// Setup Function
void setup() {
  
  // LED Pin Setup
  pinMode(LED1, OUTPUT);  // Set up D/O line for output
  pinMode(LED2, OUTPUT);
  
  // LED Pin Initial Values
  digitalWrite(LED1, LOW);  // Set all LEDs to off to start
  digitalWrite(LED2, LOW);

  // H-Bridge Pin Setup
  pinMode(bridgeFowardRight, OUTPUT);
  pinMode(bridgeBackwardRight, OUTPUT);
  pinMode(bridgeFowardLeft, OUTPUT);
  pinMode(bridgeBackwardRight, OUTPUT);
  
  // Ultrasonic Sensor Setup
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baud rate speed

  // Serial Monitor Setup
  //Serial.begin(38400);
}


// Main Loop
void loop() {
  // Starting Parameters
  stateTime = getTimeNow();   // for switching between tasks
  serialTime = getTimeNow();  // for serial check time interval
  nextState = 0;              // Set the state at which the state transition diagram starts
  int counter = 0;

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
	objectDistance = sensorPing()  // object avoidance not implemented yet
	Serial.print(objectDistance); // send sensor reading back to GUI
  controlTask(); //  Call ControlTask 1
  controlHbridgePWM(); // allow commands sent from GUI to update the PWM signal for hbridge
  
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
		// stop motors
      }

      if (command == 'l')  // 'left', move to state 2 (turn left)
      {
        nextState = 2;                
        state1Entry = false;         
      }

      if (command == 'r')  // 'right', move to state 3 (turn right)
      {
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

      break;


    case 2: // turn left 90deg
	
      if (state2Entry == false)  // if not in the state, get into it
      {
        returnToState = state;
		    turnTime = getTimeNow();
        state2Entry = true;
		    
		    // start turning left (send mototor commands)
		
      }

      if (Command == 's')  // Stop Task!
      {
        nextState = 1;        // get into state 0
        state2Entry = false;  // get out of state 2
      }

      if ((getTimeNow() - turnTime) >= turnTimeInterval)  // we've been learning for long enough!
      {
        nextState = returnToState;      // go back to previous state
        state2Entry = false;            // get out of current state
      }

      break;

    case 3: // turn right 90deg                         
      if (state3Entry == false)  // if not in the state, get into it
      {
        returnToState = state;
        turnTime = getTimeNow();
        state3Entry = true;
        
        // start turning right (send mototor commands)
    
      }

      if (Command == 's')  // stop
      {
        nextState = 1;        // get into state 0
        state3Entry = false;  // get out of state 2
      }

      if ((getTimeNow() - turnTime) >= turnTimeInterval)  // we've been turning for long enough!
      {
        nextState = returnToState;      // go back to previous state
        state3Entry = false;            // get out of current state
      }

      break;


    case 4: // drive forward                           
      if (state4Entry == false)  // if not in the state, get into it
      {
        state4Entry = true;

        // send motor commands to drive forward
        
      }

      if (command == 's')   // stop
      {
        nextState = 1;
        state4Entry = false;  // get out of state 5
        driveFoward();
      }

      if (command == 'l')   // turn left
      {
        nextState = 2;
        state4Entry = false;  // get out of state 5
      }

      if (command == 'r')   // turn right
      {
        nextState = 3;
        state4Entry = false;  // get out of state 5
      }

      break;


    case 5: // drive backward                           
      if (state5Entry == false)  // if not in the state, get into it
      {
        state5Entry = true;

        // send motor commands to drive forward
        
      }

      if (command == 's')   // stop
      {
        nextState = 1;
        state4Entry = false;  // get out of state 5
      }

      if (command == 'l')   // turn left
      {
        nextState = 2;
        state4Entry = false;  // get out of state 5
      }

      if (command == 'r')   // turn right
      {
        nextState = 3;
        state4Entry = false;  // get out of state 5
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

void turnRight()
{
  // to turn right...
  digitalWrite(bridgeForwardRight, LOW); // right side backward
  digitalWrite(bridgeBackwardRight, HIGH);

  digitalWrite(bridgeForwardLeft, HIGH); // left side forward
  digitalWrite(bridgeBackwardLeft, LOW);
}

void turnLeft()
{
  // to turn left...
  digitalWrite(bridgeForwardRight, HIGH); // right side foward
  digitalWrite(bridgeBackwardRight, LOW);

  digitalWrite(bridgeForwardLeft, LOW); // left side backward
  digitalWrite(bridgeBackwardLeft, HGIH);
}

void controlHbridgePWM()
{
  if (command == 'x') // slow
  {
    motorSetSpeed(slowMotorPWM);
  }
  if (command == 'y') // fast
  {
    motorSetSpeed(fastMotorPWM);
  }
}


void motorSetSpeed()
{
  analogWrite(brdigePWM, val);
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
  
  return distance
}
