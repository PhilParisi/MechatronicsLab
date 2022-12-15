// Jason Noel & Phil Parisi - Mechatronics Final Project
   

// 'z' --> null state (does nothing, holds current state)

// Manual States
byte state0Entry = false;  // 0 --> 'dead' (system off)
byte state1Entry = false;  // 1 --> primed
byte state2Entry = false;  // 2 --> turning left
byte state3Entry = false;  // 3 --> turning right
byte state4Entry = false;  // 4 --> drive forward
byte state5Entry = false;  // 5 --> drive backward

// Immutable 'Variables'
#define bridgeForwardRight 4  // (Hbridge connection 4A)
#define bridgeBackwardRight 2  // (Hbridge connection 3A)
#define bridgeForwardLeft 7  // (Hbridge connection 1A)
#define bridgeBackwardLeft 8  // (Hbridge connection 2A)
#define bridgePWM 9 //PWM pin (Hbridge connection 1,2EN and 3,4EN)
#define redLED 6
#define yellowLED 5

#define slowMotorPWM 170  // these probably need adjustment based on what it takes to move the motors
#define fastMotorPWM 255

#define serialCheckTimeInterval 50  // time in ms
#define turnTimeInterval 1500 // time in ms NEEDS TO BE TUNED

#define echoPin 11 // attach pin to Echo pin of JSN-SR04T sensor (this is the PWM)
#define trigPin 12 // attach to to Trig pin of JSN-SR04T 
#define minimumSensorDistance 25 // centimeters
//#define maxSensorHits 15 // hits below minimumSensorDistance to trigger robot stop
#define sensorPingTime 10 // microseconds
#define maxSensorHitCounts 2

// Mutable 'Global Variables'
byte state;  // State and NextState of 1st task transition diagram
byte nextState;
byte returnToState; 
byte sensorTimerOn;

char command;       // User command

unsigned long serialTime;
unsigned long turnTime;
unsigned long sensorTimer;

long objectDistance;
long duration;
long distance; 
int sensorHitCounts;

// Function Declarations  (Prototypes)
void controlTask(void);         // Task state transition diagram function
void controlHbridgePWM();
unsigned long getMilliTimeNow(void);  // Returns time in ms units
unsigned long getMicroTimeNow(void);  // Returns time in us units
void motorSetSpeed(int val);
void turnLeft();
void turnRight();
void moveForward();
void moveBackward();
void objectAvoidance(long distance);
void sensorPing();


// Setup Function
void setup() {

  // Build In LED
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(redLED, LOW);
  pinMode(yellowLED, OUTPUT);
  digitalWrite(yellowLED, LOW);
 
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

  // Sensor Setup
  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
  sensorTimerOn = false;
  sensorHitCounts = 0;
  
  // Control Flow Setup
  char command = 'z';       // User command
  nextState = 0;            // start in null 'dead' state

  Serial.begin(9600);
}


// Main Loop
void loop() {
  
  // Starting Timer for Serial Connection
  serialTime = getMilliTimeNow();  // for serial check time interval

  // Infinite While Loop
  while (2 > 1)  // Start infinite loop
  {

    if ((getMilliTimeNow() - serialTime) >= serialCheckTimeInterval)  // This if statement is used to not check the serial port all the time
    {
      serialTime = getMilliTimeNow();
      // Code to read command (a single char) from serial port here
      if (Serial.available() > 0)  //Wait for a char to show up
      {
        command = Serial.read();  // Read one byte from serial buffer
      }
    }


  // Tasks for Every Loop
  controlTask(); //  Call ControlTask 1 (looks for commands of the letter format like 's' 'f' 'b')
  controlHbridgePWM(); // allow commands sent from GUI to update the PWM signal for hbridge (commands of the form '1' for slow and '2' for fast)

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
        digitalWrite(redLED, HIGH);
      }

      if (command == 'i')  // 'initialize', move to State 1! 
      {
        nextState = 1;        // change current state
        state0Entry = false;  // we're leaving the state, set it to false
        digitalWrite(redLED, LOW);
      }


      break;


    case 1: // Primed & Stopped

      if (state1Entry == false)  // if not in the state, get in it!
      {
        state1Entry = true;  // we are in state 1
        stopMotors();
        digitalWrite(yellowLED, HIGH);
      }
    
      if (command == 'f')  // 'forward', move to state 4 (move forward)
      {
        nextState = 4;                
        state1Entry = false; 
        digitalWrite(yellowLED, LOW);        
      }

      if (command == 'b')  // 'reverse', move to state 5 (move backward)
      {
        nextState = 5;        
        state1Entry = false;  
        digitalWrite(yellowLED, LOW);
      }

      if (command == 'd') // 'die', go back to initialize
      {
        nextState = 0;
        state1Entry = false;
        digitalWrite(yellowLED, LOW);
      }

      if (command == 'l') // 'left', move to state 2 (turn left)
      {
        returnToState = 1;
        nextState = 2;
        state1Entry = false;
        digitalWrite(yellowLED, LOW);
      }

      if (command == 'r') // 'right', move to state 3 (turn right)
      {
        returnToState = 1;
        nextState = 3;
        state1Entry = false;
        digitalWrite(yellowLED, LOW);
      }

      break;


    case 2: // turn left 90deg
  
      if (state2Entry == false)  // if not in the state, get into it
      {
        turnTime = getMilliTimeNow();
        state2Entry = true;
        turnLeft();
      }

      if (command == 's')  // Stop Task!
      {
        nextState = 1;        // get into state 1
        state2Entry = false;  // get out of state 2
        turnTime = 0;
      }

      if ((getMilliTimeNow() - turnTime) >= turnTimeInterval)  // we've been learning for long enough!
      {
        nextState = returnToState;      // go back to previous state
        state2Entry = false;            // get out of current state
        turnTime = 0;
      }

      break;


    case 3: // turn right 90deg                         
      if (state3Entry == false)  // if not in the state, get into it
      {
        turnTime = getMilliTimeNow();
        state3Entry = true;
        turnRight();
      }

      if (command == 's')  // stop
      {
        nextState = 1;        // 
        state3Entry = false;  // get out of state 2
        turnTime = 0;
      }

      if ((getMilliTimeNow() - turnTime) >= turnTimeInterval)  // we've been turning for long enough!
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
        digitalWrite(LED_BUILTIN, HIGH);
      }

      if (state4Entry == true) // do this every loop when we're in the state
      {
        sensorPing(); // ping with ultrasonic sensor
        
        /*
        if ( (getMilliTimeNow() >= 10000) && (getMilliTimeNow() <= 10100) )
        {
          Serial.print('p');
        }
        */
          
      }

      if (command == 's')   // stop
      {
        nextState = 1;
        state4Entry = false;  // get out of state 4
        digitalWrite(LED_BUILTIN, LOW);
      }

      if (command == 'b')   // go backwards
      {
        nextState = 5;
        state4Entry = false;
        digitalWrite(LED_BUILTIN, LOW);
      }

      if (command == 'd') // 'die'
      {
        nextState = 0;
        state4Entry = false;
        digitalWrite(LED_BUILTIN, LOW);
      }

      if (command == 'l') // 'left', move to state 2 (turn left)
      {
        returnToState = 4;
        nextState = 2;
        state4Entry = false;
        digitalWrite(LED_BUILTIN, LOW);
      }

      if (command == 'r') // 'right', move to state 3 (turn right)
      {
        returnToState = 4;
        nextState = 3;
        state4Entry = false;
        digitalWrite(LED_BUILTIN, LOW);
      }

      if (nextState != 4)
      {
        state4Entry = false;
        digitalWrite(LED_BUILTIN, LOW);
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

      if (command == 'f')   // go forwards
      {
        nextState = 4;
        state5Entry = false;
      }

      if (command == 'd') // 'die'
      {
        nextState = 0;
        state5Entry = false;
      }
      
      if (command == 'l') // 'left', move to state 2 (turn left)
      {
        returnToState = 5;
        nextState = 2;
        state5Entry = false;
      }

      if (command == 'r') // 'right', move to state 3 (turn right)
      {
        returnToState = 5;
        nextState = 3;
        state5Entry = false;
      }

      break;

  }
}


unsigned long getMilliTimeNow(void)  // Returns time in ms units
{
  return (millis());
}

unsigned long getMicroTimeNow(void) // Returns time in us units
{
  return (micros());
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

void sensorPing() // function from Sensor Machine 
{
  if (sensorTimerOn == false) // timer is off, start next cycle 
  {
    //distance = 1000; // generically high value to not trigger stopping
    sensorTimerOn = true;
    sensorTimer = getMicroTimeNow();
    digitalWrite(trigPin, HIGH);
  }

  if (sensorTimerOn == true) // timer is on, check if enough time has elapsed
  {
    if ((getMicroTimeNow() - sensorTimer) >= sensorPingTime) // if enough time elapsed, get dist
    {
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.0343 / 2;
      sensorTimerOn = false;
      objectAvoidance(distance);
      distance = 1000; // reset distance
    }
  }
  
}

void objectAvoidance(long distance)
{
  if (distance <= minimumSensorDistance)
  {
    sensorHitCounts = sensorHitCounts + 1;
  }
  
  if (sensorHitCounts >= maxSensorHitCounts) // allows us to deal with noisy/random hits that aren't true
  {
    Serial.print('p'); // tell the GUI that we are too close to something!
    nextState = 1;
    sensorHitCounts = 0;
  }
} 
