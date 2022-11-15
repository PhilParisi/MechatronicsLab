// Lab 6 - Jason Noel & Phil Parisi - Mechatronics

// Task 1
// 'Run' --> 'a'
// 'Pause' --> 'b'
// 'Resume' --> 'c'
// 'Stop' --> 'd'

//Task 2
// 'Run' --> 'e'
// 'Stop' --> 'f'

// Immutable 'Variables'
#define Task1LED1 2
#define Task1LED2 3
#define Task1LED3 4                 
#define Task2LED1 6
#define Task2LED2 7
#define Button 8
#define Task1Interval 2000          // Time in ms
#define Task2Interval8 8000
#define Task2Interval4 4000
#define SerialCheckTimeInterval 50  // Time in ms

// Mutable 'Global Variables'
byte State_Task1;  // State and NextState of 1st task transition diagram
byte State_Task2;
byte nextState_Task1;
byte nextState_Task2;
byte returnToState_Task1;  // store the state to return to, for use by Pause (state 5)

byte State0Entry_Task1 = false;  // 0 --> all LED off
byte State1Entry_Task1 = false;  // 1 --> 1st LED on
byte State2Entry_Task1 = false;  // 2 --> 2nd LED on
byte State3Entry_Task1 = false;  // 3 --> 3rd LED on
byte State5Entry_Task1 = false;  // 5 --> pause mode

byte State0Entry_Task2 = false;
byte State1Entry_Task2 = false;
byte State2Entry_Task2 = false;

char Command = 'z';       // User command
unsigned long StartTime;  // Variable to record starting time
unsigned long StartTime1;

float StateTime_Task1;
float StateTime_Task2;
float SerialTime_Task1;

// Function Declarations  (Prototypes)
void ControlTask1(void);         // Task1 state transition diagram function
unsigned long GetTimeNow(void);  // Returns time in ms units


// Setup Function
void setup() {
  pinMode(Task1LED1, OUTPUT);  // Set up D/O line for output
  pinMode(Task1LED2, OUTPUT);
  pinMode(Task1LED3, OUTPUT);
  pinMode(Task2LED1, OUTPUT);
  pinMode(Task2LED2, OUTPUT);
  pinMode(Button, INPUT);


  digitalWrite(Task1LED1, LOW);  // Set all LEDs to off to start
  digitalWrite(Task1LED2, LOW);
  digitalWrite(Task1LED3, LOW);
  digitalWrite(Task2LED1, LOW);
  digitalWrite(Task2LED2, LOW);


  Serial.begin(38400);
}


// Main Loop
void loop() {
  // Starting Parameters
  StateTime_Task1 = GetTimeNow();   // for switching between tasks
  StateTime_Task2 = GetTimeNow();
  SerialTime_Task1 = GetTimeNow();  // for serial check time interval
  nextState_Task1 = 0;              // Set the state at which the state transition diagram starts
  nextState_Task2 = 0;
  int counter = 0;

  while (2 > 1)  // Start infinite loop
  {

    if ((GetTimeNow() - SerialTime_Task1) >= SerialCheckTimeInterval)  // This if statement is used to not check the serial port all the time
    {
      SerialTime_Task1 = GetTimeNow();
      // Code to read command (a single char) from serial port here
      if (Serial.available() > 0)  //Wait for a char to show up
      {
        Command = Serial.read();  // Read one byte from serial buffer
      }
    }

    ControlTask1(); //  Call ControlTask 1
    ControlTask2(); // Call ControlTask 2  
    Command = 'z';   // reset this so commands are executed only once
  }
}

// Definition of ControlTask1 Function
void ControlTask1(void) {

  State_Task1 = nextState_Task1;

  // Jump between STATES
  switch (State_Task1)  // Go to current state
  {

    case 0:  // All LEDs off


      if (State0Entry_Task1 == false)  // if not in the state, get in it!
      {
        State0Entry_Task1 = true;
        // turn all LEDs off
        digitalWrite(Task1LED1, LOW);
        digitalWrite(Task1LED2, LOW);
        digitalWrite(Task1LED3, LOW);
        // get out of the switch-case
      }

      if (Command == 'a')  // move to State 1! 1st LED on
      {
        nextState_Task1 = 1;        // change current state
        State0Entry_Task1 = false;  // we're leaving the state, set it to false
      }

      // if any other Command (b-pause, c-resume, d-end) is sent, don't do anything
      // if any inState0_Task1 == true, we're already off so don't do anything
      break;

    case 1:                            // 1st LED on
      if (State1Entry_Task1 == false)  // if not in the state, get in it!
      {
        State1Entry_Task1 = true;  // we are in state 1
        // adjust lighting
        digitalWrite(Task1LED1, HIGH);
        digitalWrite(Task1LED2, LOW);
        digitalWrite(Task1LED3, LOW);
        StateTime_Task1 = GetTimeNow();
      }

      // COMMAND CODE
      if (Command == 'b')  // Pause Task!
      {
        returnToState_Task1 = State_Task1;  // store this for the Pause state to use
        nextState_Task1 = 5;                // get into state 5
        State1Entry_Task1 = false;          // get out of state 1
      }

      if (Command == 'd')  // Stop Task!
      {
        nextState_Task1 = 0;        // get into state 0
        State1Entry_Task1 = false;  // get out of state 1
      }


      // if Command = c-resume (nothing to resume), if Command = a-start (we're already started)
      // if any inState1_Task1 == true, we're already in the state, so COUNT!!

      // COUNTING CODE
      if ((GetTimeNow() - StateTime_Task1) >= Task1Interval) {
        nextState_Task1 = 2;        // get into state 2
        State1Entry_Task1 = false;  // get out of state 1
      }

      break;

    case 2:                            // 2nd LED is on
      if (State2Entry_Task1 == false)  // if not in the state, get into it
      {
        State2Entry_Task1 = true;
        // adjust lighting, 2nd on
        digitalWrite(Task1LED1, LOW);
        digitalWrite(Task1LED2, HIGH);
        digitalWrite(Task1LED3, LOW);
        StateTime_Task1 = GetTimeNow();
      }

      // COMMAND CODE
      if (Command == 'b')  // Pause Task!
      {
        returnToState_Task1 = State_Task1;  // store this for the Pause state to use
        nextState_Task1 = 5;                // get into state 5
        State2Entry_Task1 = false;          // get out of state 1
      }

      if (Command == 'd')  // Stop Task!
      {
        nextState_Task1 = 0;        // get into state 0
        State2Entry_Task1 = false;  // get out of state 2
      }

      // if Command = c-resume (nothing to resume), if Command = a-start (we already started)
      // if inState2_Task2 == true, keep coutning because we're already in the state!

      // COUNTING CODE
      if ((GetTimeNow() - StateTime_Task1) >= Task1Interval) {
        nextState_Task1 = 3;        // get into state 3
        State2Entry_Task1 = false;  // get out of state 2
      }
      break;

    case 3:                            // 3nd LED is on
      if (State3Entry_Task1 == false)  // if not in the state, get into it
      {
        State3Entry_Task1 = true;
        // adjust lighting, 3nd on
        digitalWrite(Task1LED1, LOW);
        digitalWrite(Task1LED2, LOW);
        digitalWrite(Task1LED3, HIGH);
        StateTime_Task1 = GetTimeNow();
      }

      if (Command == 'b')  // Pause Task!
      {
        returnToState_Task1 = State_Task1;  // store this for the Pause state to use
        nextState_Task1 = 5;                // get into state 5
        State3Entry_Task1 = false;          // get out of state 3
      }

      if (Command == 'd')  // Stop Task!
      {
        nextState_Task1 = 0;        // get into state 0
        State3Entry_Task1 = false;  // get out of state 3
      }

      // if Command = a-start (we're already started), if Command = c-resume (nothing to resume)
      // if inState3_Task1 == true, we're already in the state so keep counting!

      // COUNTING CODE
      if ((GetTimeNow() - StateTime_Task1) >= Task1Interval) {
        nextState_Task1 = 1;        // get into state 1
        State3Entry_Task1 = false;  // get out of state 3
      }
      break;

    case 5:                            // Pause (don't change anything!)
      if (State5Entry_Task1 == false)  // if not in the state, get into it
      {
        State5Entry_Task1 = true;
        // don't change anything
      }

      if (Command == 'c')  // Resume Task! Get back into state we we're in
      {
        nextState_Task1 = returnToState_Task1;
        State5Entry_Task1 = false;  // get out of state 5
      }

      // Pause state 5 only changes state on Command = c-resume, otherwise do nothing
      break;
  }
}

// TASK 2 CODE
void ControlTask2(){
  State_Task2 = nextState_Task2;

  switch(State_Task2)
  {
    case 0:  // All LEDs off

      if (State0Entry_Task2 == false)  // if not in the state, get in it!
      {
        State0Entry_Task2 = true;
        // turn all LEDs off
        digitalWrite(Task2LED1, LOW);
        digitalWrite(Task2LED2, LOW);
        // get out of the switch-case
      }

      if (Command == 'e')  // move to State 1! 1st LED on
      {
        nextState_Task2 = 1;        // change current state
        State0Entry_Task2 = false;  // we're leaving the state, set it to false
      }

      break;

    case 1:                            // 1st LED on
      if (State1Entry_Task2 == false)  // if not in the state, get in it!
      {
        State1Entry_Task2 = true;  // we are in state 1
        // adjust lighting
        digitalWrite(Task2LED1, HIGH);
        digitalWrite(Task2LED2, LOW);
        StateTime_Task2 = GetTimeNow();
      }

      // COUNTING CODE
      if ((GetTimeNow() - StateTime_Task2) >= Task2Interval8 and digitalRead(Button) == HIGH) 
      {
        nextState_Task2 = 2;        // get into state 2
        State1Entry_Task2 = false;  // get out of state 1
      }
      if (Command == 'f')  // "Off"
      {
        nextState_Task2 = 0;        // change current state
        State1Entry_Task2 = false;  // we're leaving the state, set it to false
      }

      break;

     case 2:                            // Both LEDs on
      if (State2Entry_Task2 == false)  // if not in the state, get in it!
      {
        State2Entry_Task2 = true;  // we are in state 1
        // adjust lighting
        digitalWrite(Task2LED1, HIGH);
        digitalWrite(Task2LED2, HIGH);
        StateTime_Task2 = GetTimeNow(); //Resetting timer
      }

      // COUNTING CODE
      if ((GetTimeNow() - StateTime_Task2) >= Task2Interval4) 
      {
        nextState_Task2 = 1;        // get into state 2
        State2Entry_Task2 = false;  // get out of state 1
      }
      if (Command == 'f')  // "Off"
      {
        nextState_Task2 = 0;        // change current state
        State2Entry_Task2 = false;  // we're leaving the state, set it to false
      }

      break;
  }
}

// Definition of GetTimeNow() function
unsigned long GetTimeNow(void)  // Returns time in ms units
{
  return (millis());
}
