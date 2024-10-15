// Motor Driver Pins
const int motorA1 = 7;  // IN1: Control pin for motor A, direction 1
const int motorA2 = 6;  // IN2: Control pin for motor A, direction 2

const int motorB1 = 4;  // IN4: Control pin for motor B, direction 1
const int motorB2 = 5;  // IN5: Control pin for motor B, direction 2 

const int badbuzz = 10; // Buzzer pin to indicate danger

const int ldrPin = A0;  // LDR (Light Dependent Resistor) connected to analog pin A0
const int ledPin = 9;   // LED connected to digital pin 9

// Ultrasonic sensor pins
const int trigPin = 11; // Trig pin of the ultrasonic sensor to send pulse
const int echoPin = 12; // Echo pin of the ultrasonic sensor to receive pulse

int lightThreshold = 500;  // Light intensity threshold for LDR (adjust based on environment)

void setup() {
  // Set motor control pins as outputs
  pinMode(motorA1, OUTPUT); // Set motor A control pin 1 as output
  pinMode(motorA2, OUTPUT); // Set motor A control pin 2 as output

  pinMode(motorB1, OUTPUT); // Set motor B control pin 1 as output
  pinMode(motorB2, OUTPUT); // Set motor B control pin 2 as output

  pinMode(ledPin, OUTPUT);   // Set LED pin as output

  pinMode(badbuzz, OUTPUT);  // Set buzzer pin as output
  digitalWrite(badbuzz, LOW); // Initialize buzzer to be off (LOW)

  // Set ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);  // Set Trig pin as output
  pinMode(echoPin, INPUT);   // Set Echo pin as input

  // Start motors stopped
  stopMotors();              // Ensure motors are stopped at the beginning

  // Begin serial communication for debugging (optional)
  Serial.begin(9600);        // Start serial communication at 9600 baud rate
}

void loop() {
  int distance = measureDistance(); // Measure the distance using the ultrasonic sensor
  int ldrValue = analogRead(ldrPin); // Read the value from the LDR

  Serial.print("Distance: ");       // Print distance label to serial monitor
  Serial.print(distance);           // Print the measured distance
  Serial.println(" cm");            // Print unit (centimeters)

  if (distance > 4) {               // Check if distance is greater than 4 cm
    rotateMotorsDown();             // Rotate motors to lower the platform
    digitalWrite(badbuzz, LOW);     // Ensure buzzer is off
  } else if (distance < 3) {        // Check if distance is less than 3 cm
    digitalWrite(badbuzz, HIGH);    // Turn on the buzzer to alert
    rotateMotorsUp();               // Rotate motors to raise the platform
  } else {
    stopMotors();                   // Stop motors if distance is between 3 and 4 cm
    digitalWrite(badbuzz, LOW);     // Ensure buzzer is off
  }

  if (ldrValue < lightThreshold) {  // Check if LDR value is below the threshold
    digitalWrite(ledPin, LOW);      // Turn off the LED (light condition)
  } else {
    digitalWrite(ledPin, HIGH);     // Turn on the LED (dark condition)
  }
}

// Function to measure distance using the ultrasonic sensor
int measureDistance() {
  // Send a 10 microsecond pulse to trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);       // Ensure Trig pin is low
  delayMicroseconds(2);              // Wait for 2 microseconds
  digitalWrite(trigPin, HIGH);      // Set Trig pin high to send pulse
  delayMicroseconds(10);             // Wait for 10 microseconds
  digitalWrite(trigPin, LOW);       // Set Trig pin low

  // Measure the time it takes for the pulse to return
  long duration = pulseIn(echoPin, HIGH); // Read the duration of the echo pulse

  // Calculate the distance (in cm)
  int distance = duration * 0.034 / 2; // Convert duration to distance (cm)

  // Limit the distance to maxDistance
  if (distance > 9) {                 // Ignore any distance greater than 9 cm
    distance = 9; 
  }

  return distance;                    // Return the measured distance
}

// Function to rotate motors to raise the platform (up)
void rotateMotorsUp() {
  digitalWrite(motorA1, LOW);        // Set motor A1 low
  digitalWrite(motorA2, HIGH);       // Set motor A2 high to raise motor A
  digitalWrite(motorB1, LOW);        // Set motor B1 low
  digitalWrite(motorB2, HIGH);       // Set motor B2 high to raise motor B
}

// Function to rotate motors to lower the platform (down)
void rotateMotorsDown() {
  digitalWrite(motorA1, HIGH);       // Set motor A1 high to lower motor A
  digitalWrite(motorA2, LOW);        // Set motor A2 low
  digitalWrite(motorB1, HIGH);       // Set motor B1 high to lower motor B
  digitalWrite(motorB2, LOW);        // Set motor B2 low
}

// Function to stop motors
void stopMotors() {
  digitalWrite(motorA1, LOW);        // Stop motor A1
  digitalWrite(motorA2, LOW);        // Stop motor A2
  digitalWrite(motorB1, LOW);        // Stop motor B1
  digitalWrite(motorB2, LOW);        // Stop motor B2
}
