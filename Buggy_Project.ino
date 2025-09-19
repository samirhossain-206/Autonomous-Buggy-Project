#include <Servo.h>
Servo sensorServo;  // Servo used for sensor scanning

int ENA = 7;  // Motor A speed control pin (PWM)
int ENB = 2;  // Motor B speed control pin (PWM)

int IN1 = 6;  // Motor A input 1
int IN2 = 5;  // Motor A input 2
int IN3 = 4;  // Motor B input 1
int IN4 = 3;  // Motor B input 2

int SERVO = 9;     // Servo control pin

int TRIGGER = 10;  // Ultrasonic sensor trigger pin
int ECHO = 11;     // Ultrasonic sensor echo pin

void setup() {
  Serial.begin(9600);  // Initialize serial communication

  // Set motor pins as outputs
  pinMode(ENB, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set ultrasonic pins
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);

  // Set motor speeds
  analogWrite(ENA, 130);  // Motor A speed (0-255)
  analogWrite(ENB, 255);  // Motor B speed (0-255)

  // Attach servo to control pin and move to initial position
  sensorServo.attach(SERVO);
  sensorServo.write(100);
  delay(3000);  // Allow servo to reach position
}

void loop() {
  // Obtain distance in front
  int frontDistance = measureDistance();
  Serial.print("Front: ");
  Serial.println(frontDistance);
  delay(500);
  
  if (frontDistance < 20)  // If obstacle detected close in front
  {
    stopBuggy();  // Stop the buggy
    delay(250);

    turnLeft();   // Turn left to check side
    delay(350);

    int leftDistance = measureDistance(); // Get left distance
    delay(500);
    Serial.print("Left: ");
    Serial.println(leftDistance);

    turnRight();  // Turn right to check right side
    delay(700);

    int rightDistance = measureDistance(); // Get right distance
    delay(500);
    Serial.print("Right: ");  
    Serial.println(rightDistance);

    // Decide which way to go
    if (leftDistance < 20)
    {
      if (rightDistance < 20)
      {
        turnRight();
        delay(350);
        Serial.println("Turnt around");

        moveForward();  // Go forward after turning around
      }
      else
      {
        Serial.println("Went right");
        moveForward();  // Move forward to the right
      }
    }
    else
    {
      turnLeft();
      delay(700);
      Serial.println("Went left");

      moveForward();  // Move forward to the left
    }
  }
  else
  {
    moveForward();  // Move forward if no obstacle
  } 

  delay(100);  // Short delay before next loop
}

// Move both motors forward
void moveForward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Turn the buggy left
void turnLeft()
{
  Serial.println("Turning Left");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Turn the buggy right
void turnRight()
{
  Serial.println("Turning right");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Stop both motors
void stopBuggy()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Measure distance using ultrasonic sensor
long measureDistance()
{
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  long duration = pulseIn(ECHO, HIGH);  // Measure pulse duration
  long distance = (duration / 2) * 0.034; // Convert duration to cm

  return distance;  // Return distance in cm
}
