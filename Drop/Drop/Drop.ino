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

  // Setup pins: general
  pinMode(LED_BUILTIN, OUTPUT); // Configure Teensy LED
  pinMode(NASA_DROP_PIN, INPUT_PULLUP); // Receiving drop signal
  // Setup pins: motor driver
  pinMode(motorDriverDir, OUTPUT); digitalWrite(motorDriverDir, HIGH); // Motor driver direction
  pinMode(motorDriverPwm, OUTPUT); analogWrite(motorDriverPwm, 0); // Motor driver PWM
  pinMode(motorDriverSlp, OUTPUT); digitalWrite(motorDriverSlp, LOW);// Motor driver Sleep
  // Servos
  servoLower.attach(37); // White = lower
  servoUpper.attach(36); // Blue = upper

  // Start detecting interrupt
  attachInterrupt(digitalPinToInterrupt(NASA_DROP_PIN), record, LOW);
}

void loop() {
  if (interrFlag && (micros()-interrStart)>8*1000) { // Interrupted & lasts > 8ms
    digitalWrite(LED_BUILTIN, HIGH);
    // Lower and power the DC motor
    digitalWrite(motorDriverSlp, HIGH); // Remove motor driver from sleep mode
    analogWrite(motorDriverPwm, 255*0.5); // 50% duty cycle
    delay(20000); 
    // Unpower the DC motor
    analogWrite(motorDriverPwm, 0);
    delay(1000);
    int now = millis();
    while (millis() - now < 5000) { // Pulse servos and end code
      servoLower.writeMicroseconds(1900); // pulse servos
      servoUpper.writeMicroseconds(1900); // pulse servos
      delay(200);
    }

    // Power, raise, and sleep DC motor
    digitalWrite(motorDriverDir, LOW);
    analogWrite(motorDriverPwm, 255*0.5);
    delay(18000);
    analogWrite(motorDriverPwm, 0);
    digitalWrite(motorDriverSlp, LOW);
    while (true) {

    }
  }
  else{ // Not interrupted
    interrFlag = false; // TODO: debug this
    interrStart = micros();

    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);

}

void record() {
  interrFlag = true;
  interrStart = micros();
}

