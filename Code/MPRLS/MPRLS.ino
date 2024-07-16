#include<Arduino.h>
#include<Wire.h>

// MPRLS addr/pins
uint8_t mprlsAddr = 0x28; // I2C addr
// MPRLS variables
uint8_t mprlsOutput[7];
uint8_t mprlsExitStandby[3] = {0xAA, 0x00, 0x00};
double press_counts = 0; // digital pressure reading [counts]
double temp_counts = 0; // digital temperature reading [counts]
double pressure = 0; // pressure reading [bar, psi, kPa, etc.]
double temperature = 0; // temperature reading in deg C
double outputmax = 15099494; // output at max pressure [counts]
double outputmin = 1677722; // output at min pressure [counts]
double pmax = 25; // max value of pressure range [bar, psi, kPa, etc.]
double pmin = 0; // min value of pressure range [bar, psi, kPa, etc.]
double percentage = 0; // holds percentage of full scale data
char printBuffer[200], cBuff[20], percBuff[20], pBuff[20], tBuff[20];


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }
  Wire1.begin();
}

void loop() {
  // Exit StandBy mode: done before every reading
  Wire1.beginTransmission(mprlsAddr);
  Wire1.write(mprlsExitStandby, 3);
  Wire1.endTransmission();
  delay(10);
  // Take readings
  Wire1.requestFrom(mprlsAddr, 7); // read back Sensor data 7 bytes
  for (int i = 0; i < 7; i++) {
    mprlsOutput[i] = Wire1.read();
  }

  // Pressure Readings
  press_counts = mprlsOutput[3] + mprlsOutput[2] * 256 + mprlsOutput[1] * 65536; // calculate digital pressure counts
  percentage = (press_counts / 16777215) * 100; // calculate pressure as percentage of full scale
  pressure = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin; // Calculate pressure: eq2 on datasheet
  // Temp Readings
  temp_counts = mprlsOutput[6] + mprlsOutput[5] * 256 + mprlsOutput[4] * 65536; // calculate digital temperature counts
  temperature = (temp_counts * 200 / 16777215) - 50; // calculate temperature in deg c
  // String conversions
  dtostrf(percentage, 4, 3, percBuff);
  dtostrf(pressure, 4, 3, pBuff);
  dtostrf(temperature, 4, 3, tBuff);

  // String printing
  if(mprlsOutput[0]==0x40) { // Status: Good
    Serial.print("% of full pressure: ");Serial.println(percBuff);
    Serial.print("Pressure: ");Serial.println(pBuff);
    Serial.print("Temp: ");Serial.println(tBuff);
  }else { // Status: Bad
    Serial.println("MPRLS Status: BAD");
    Serial.print("Status Reg: ");Serial.println(mprlsOutput[0]);
  }
  Serial.println("----------------");


  delay(1000);
}