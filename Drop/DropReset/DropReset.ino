#include <Servo.h>

// NASA input pin
int NASA_DROP_PIN = 0;
// DC motor driver
int motorDriverDir = 16;
int motorDriverPwm = 15;
int motorDriverSlp = 14;
//int motorDriverCs = 41; <-- disconnected
// Volatile vars
volatile bool interrFlag = false;
volatile int interrStart = 0;

Servo servoLower;
Servo servoUpper;

void setup() {
  // Serial.begin(115600);
  // Serial.println("Begin drop test");

  // Setup pins: motor driver
  pinMode(motorDriverDir, OUTPUT); digitalWrite(motorDriverDir, LOW); // Motor driver direction
  pinMode(motorDriverPwm, OUTPUT); analogWrite(motorDriverPwm, 0); // Motor driver PWM
  pinMode(motorDriverSlp, OUTPUT); digitalWrite(motorDriverSlp, HIGH);// Motor driver Sleep

  delay(200);
  analogWrite(motorDriverPwm, 255*0.5);
}

void loop() {
}

