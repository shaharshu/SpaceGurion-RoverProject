// Definitions Arduino pins connected to input H Bridge
int en1 = 3;
int IN1 = 4;
int IN2 = 5;
int en2 = 9;
int IN3 = 6;
int IN4 = 7;

void setup()
{
//
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
pinMode(en1, OUTPUT);
pinMode(en2, OUTPUT);
}

void loop()
{
// Rotate the Motor A clockwise
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
delay(2000);
// Motor A
digitalWrite(IN1, HIGH);
digitalWrite(IN2, HIGH);
delay(500);
// Rotate the Motor B clockwise
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
delay(2000);
// Motor B
digitalWrite(IN3, HIGH);
digitalWrite(IN4, HIGH);
delay(500);
// Rotates the Motor A counter-clockwise
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
delay(2000);
// Motor A
digitalWrite(IN1, HIGH);
digitalWrite(IN2, HIGH);
delay(500);
// Rotates the Motor B counter-clockwise
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
delay(2000);
// Motor B
digitalWrite(IN3, HIGH);
digitalWrite(IN4, HIGH);
delay(500);
}