#define echoPin 11 // attach pin D2 Arduino to pin Echo of JSN-SR04T (PWM)
#define trigPin 12 //attach pin D3 Arduino to pin Trig of JSN-SR04T

long duration;
long distance;
long sensorPing(void);
long sensorPingPhil(void);
unsigned long getMicroTimeNow();
byte sensorTimerOn;
unsigned long sensorTimer;
#define sensorPingTime 10 // microseconds

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baud rate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  sensorTimerOn = false;
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorPingPhil();
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
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");// working  code for aj-sr04m
  
  return distance;
}

long sensorPingPhil(void) // function from Sensor Machine 
{
  if (sensorTimerOn == false) // timer is off, start next cycle 
  {
    distance = 1000; // generically high value to not trigger stopping
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
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");// working  code for aj-sr04m
      return distance;
    }
  }


  /*Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");// working  code for aj-sr04m
  return distance;
  */
  
}

unsigned long getMicroTimeNow()
{
  return (micros());
}
