//Lab 4
//Ex_4.1 --> Blink
//Jason Noel & Phil Parisi


void setup() {
pinMode(13, OUTPUT);
}

void loop() {
digitalWrite(13, HIGH);
delay(2000);
digitalWrite(13, LOW);
delay(2000);
}

////////////////////////////////////////////////////////////
//Lab 4
//Ex 4.1_B (alternating blinking)
//Blink
//Jason Noel & Phil Parisi


void setup() {
pinMode(13, OUTPUT);
pinMode(12, OUTPUT);
}

void loop() {
digitalWrite(12, HIGH);
digitalWrite(13, HIGH);
delay(2000);
digitalWrite(12, LOW);
digitalWrite(13, LOW);
delay(2000);
}

////////////////////////////////////////////////////////////

//Lab 4
//Ex 4.2
//Jason Noel & Phil Parisi


void setup() {
pinMode(13, OUTPUT);
pinMode(12, OUTPUT);
pinMode(10, INPUT);
}

void loop() {
digitalRead(10);

if (digitalRead(10) == LOW)
{
digitalWrite(12, HIGH);
digitalWrite(13, HIGH);
delay(2000);
digitalWrite(12, LOW);
digitalWrite(13, LOW);
delay(2000);
}

else if (digitalRead(10) == HIGH)
{
digitalWrite(12, LOW);
digitalWrite(13, HIGH);
delay(500);
digitalWrite(12, HIGH);
digitalWrite(13, LOW);
delay(500);
}

///////////////////////////////////////////////////////////////

////Lab 4
////Ex 4.3 - Motor Operation
////Jason Noel & Phil Parisi

void setup() {
  pinMode(11, OUTPUT);
}

void loop() {
  digitalWrite(11, HIGH);
  delay(500);
  digitalWrite(11, LOW);
  delay(500);
}

//////////////////////////////////////////////////////////////

////Lab 4
////4.4 - Output Screen using AND Gate
////Jason Noel & Phil Parisi

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  // Establish Parameters
  int bio[7]; bio[0] = 0x01; bio[1] = 0x02; bio[2] = 0x04; bio[3] = 0x08; bio[4] = 0x10; bio[5] = 0x20; bio[6] = 0x40;
  int vals[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};
  int val;  int pin_eval;
  
  // Loop thru each of the Decimal Numbers
  for (int v_ind = 0; v_ind < 10; v_ind++)
  {
    val = vals[v_ind]; // grab a value out
    Serial.print("next value:");
    Serial.print(val); Serial.print(" ");

    // Loop thru each of the Binaries and Write to Pins
    for (int b_ind = 0; b_ind < 7; b_ind++)
    {    
      pin_eval = val & bio[b_ind]; // 63 & 1, 63 & 2, 63 & 4, ... (output is either 0 or the 1,2,4,8,...

      // Set Pin to HIGH or LOW
      if (pin_eval == bio[b_ind]){
        digitalWrite(b_ind+2, HIGH);
      } 
      else {
        digitalWrite(b_ind+2, LOW);
      }
      Serial.print(pin_eval); Serial.print(" ");
    }
    delay(1000);
  }
  delay(1000);
}