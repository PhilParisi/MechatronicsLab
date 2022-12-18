Jason Noel and Phillip Parisi
MCE 433 - Mechatronics
26October2022

**********************************************************

// Lab 5, Ex 5.1b - Jason Noel and Phil Parisi
// Light Combinations (w/ binary AND gates)

double addata;
float interval;
int interval_int;
int pin2; int pin3; int pin4; int pin5;

void setup() {
  // put your setup code here, to run once:
pinMode(13, OUTPUT);
pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  addata=analogRead(0);
  Serial.println(addata);

  interval = addata / 64; //vals from 0 to 15.99 float
  interval_int = int(interval); //vals 0, 1, 2, 3, ..., 15
  Serial.println(interval_int);
  
  pin2 = interval_int & 1; 
  pin3 = interval_int & 2;
  pin4 = interval_int & 4;
  pin5 = interval_int & 8;
  
  digitalWrite(2, pin2);
  digitalWrite(3, pin3);
  digitalWrite(4, pin4);
  digitalWrite(5, pin5);
  
  delay(1000);
}

***************************************************

// Lab 5, Ex 5.2 - Jason Noel and Phil Parisi
// Motor Intervals w/ PWM

int addata;
float interval;
int interval_int;
int PWM_int;

void setup() {
pinMode(11, OUTPUT); // PWM output
pinMode(0, INPUT); // potentiometer read
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  addata=analogRead(0);
  Serial.println(addata);

  interval = addata / 64; //vals from 0 to 15.99 float
  interval_int = int(interval); //vals 0, 1, 2, 3, ..., 15
  Serial.println(interval_int);

  PWM_int = interval_int * 16;
  analogWrite(11, PWM_int);
  
}

************************************************************
// Lab 5.3 - Jason Noel and Phil Parisi
// DC Motor Control w/ Serial Input

char UserInput;
int addata = 0;
int pwm_out = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT); // Digital output (5V or 0V), 1A
  pinMode(7, OUTPUT); // Digital output (5V or 0V), 2A
  pinMode(11, OUTPUT); // PWM output to vary motor speed
  pinMode(0, INPUT); // Analog read from potentiometer
}

void loop()
{
  Serial.println("-------------------------------------------------------------");
  Serial.println("Enter a character in the top box: ");

  //Constantly check for serial input and update PWM
  while (Serial.available() == 0)
  {
    addata = analogRead(0); // 0 to 1024
    pwm_out = addata / 4;
    analogWrite(11, pwm_out);
  }

  //Read the input from Serial Window
  UserInput = Serial.read();
  Serial.println("Value read is: ");
  Serial.println(UserInput);

  //Change Gates of H-Bridge
  if (UserInput == '1') // Clockwise
  {
    Serial.println("Turning Motor CW!");
    digitalWrite(2, HIGH); //1A
    digitalWrite(7, LOW); // 2A
  }
  else if (UserInput == '2') // Counterclockwise
  {
    Serial.println("Turning Motor CCW!");
    digitalWrite(2, LOW); //1A
    digitalWrite(7, HIGH); // 2A
  }
  else // Off
  {
    Serial.println("Turning Motor Off!");
    digitalWrite(2, LOW); //1A
    digitalWrite(7, LOW); // 2A
  }

  //Write PWM Voltage
  analogWrite(11, pwm_out);
  UserInput = 0;
 }

  

//********************************************************************
// Exercise 5.4 (basic servo)
//********************************************************************
#include <Servo.h>    // Include the servo libray in your code
int angle = 0;        // Declare angle to be an int type and initialize to 0
Servo servo1;         // Declare a variable of type servo
void setup()          // This function executes only once
{

  servo1.attach(11);  // Use pin #11 to control the servo
  Serial.begin(9600); // Initialize serial communication at a baud rate of 9600
}

void loop()           // This function executes repeatedly
{
  Serial.println("Enter an angle between 0 and 180"); // Print a message
  while ( Serial.available() == 0)
  {

  }
  
  if (Serial.available() > 0)
  {
    angle = Serial.parseInt();  // Convert incoming characters to an integer 
    Serial.print(" The number received is: ");
    Serial.println(angle);      // Print back the angle to the user
    servo1.write(angle);        // Send desired angle to the servo
  }
  delay(500);
}


************************************************************
// Exercise 5.4b (servo control with potentiometer)

#include <Servo.h>    // Include the servo libray in your code
float angle = 0;      // Declare angle to be an int type and initialize to 0
Servo servo1;         // Declare a variable of type servo
float potent_val;

void setup()          // This function executes only once
{
  pinMode(0,INPUT);
  servo1.attach(11);  // Use pin #11 to control the servo
  Serial.begin(9600); // Initialize serial communication at a baud rate of 9600
}

void loop()           // This function executes repeatedly
{
  potent_val = analogRead(0);
  Serial.print(analogRead(0));
  angle = potent_val * 180 / 1023;

  Serial.print(" The angle will be: ");
  Serial.println(angle);      // Print back the angle to the user
  servo1.write(angle);        // Send desired angle to the servo
  delay(100);
}