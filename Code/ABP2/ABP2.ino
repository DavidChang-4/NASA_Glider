#include<Arduino.h>
#include<Wire.h>

// ABP2 addr/pins
uint8_t abp2Addr = 0x28; // I2C addr
// ABP2 variables
uint8_t abp2Output[7];
uint8_t abp2ExitStandby[3] = {0xAA, 0x00, 0x00};
double press_counts = 0; // digital pressure reading [counts]
double temp_counts = 0; // digital temperature reading [counts]
double pressure = 0; // pressure reading [bar, psi, kPa, etc.]
double temperature = 0; // temperature reading in deg C
double outputmax = 15099494; // output at max pressure [counts]
double outputmin = 1677722; // output at min pressure [counts]
double pmax = 1; // max value of pressure range [bar, psi, kPa, etc.]
double pmin = 0; // min value of pressure range [bar, psi, kPa, etc.]
double percentage = 0; // holds percentage of full scale data
char printBuffer[200], cBuff[20], percBuff[20], pBuff[20], tBuff[20];


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }
  Wire.begin();
}


void loop() {
  // Exit StandBy mode: done before every reading
  Wire.beginTransmission(abp2Addr);
  Wire.write (abp2ExitStandby, 3); // write command to the sensor
  Wire.endTransmission();
  delay(10);
  // Take readings
  Wire.requestFrom(abp2Addr, 7); // read back Sensor data 7 bytes
  for (int i = 0; i < 7; i++) {
    abp2Output[i] = Wire.read();
  }

  // Pressure Readings
  press_counts = abp2Output[3] + abp2Output[2] * 256 + abp2Output[1] * 65536; // calculate digital pressure counts
  percentage = (press_counts / 16777215) * 100; // calculate pressure as percentage of full scale
  pressure = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin; // Calculate pressure: eq2 on datasheet
  // Temp Readings
  temp_counts = abp2Output[6] + abp2Output[5] * 256 + abp2Output[4] * 65536; // calculate digital temperature counts
  temperature = (temp_counts * 200 / 16777215) - 50; // calculate temperature in deg c
  // String conversions
  dtostrf(percentage, 4, 3, percBuff);
  dtostrf(pressure, 4, 3, pBuff);
  dtostrf(temperature, 4, 3, tBuff);

  // String printing
  if(abp2Output[0]==0x40) { // Status: Good
    Serial.print("% of full pressure: ");Serial.println(percBuff);
    Serial.print("Pressure: ");Serial.println(pBuff);
    Serial.print("Temp: ");Serial.println(tBuff);
  }else { // Status: Bad
    Serial.println("ABP2 Status: BAD");
    Serial.print("Status Reg: ");Serial.println(abp2Output[0]);
  }
  Serial.println("----------------");


  delay(1000);
}