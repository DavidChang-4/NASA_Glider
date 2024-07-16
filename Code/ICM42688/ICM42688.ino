#include <SPI.h>

// ICM-42688 SPI settings
#define ICM42688_CS 10

// Register addresses
#define WHO_AM_I 0x75
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H 0x43

void writeRegister(uint8_t reg, uint8_t value) {
  digitalWrite(ICM42688_CS, LOW);
  SPI.transfer(reg);
  SPI.transfer(value);
  digitalWrite(ICM42688_CS, HIGH);
}

uint8_t readRegister(uint8_t reg) {
  digitalWrite(ICM42688_CS, LOW);
  SPI.transfer(0x80 | reg);
  uint8_t value = SPI.transfer(0x00);
  digitalWrite(ICM42688_CS, HIGH);
  return value;
}

void readMultipleRegisters(uint8_t reg, uint8_t *buffer, uint8_t length) {
  digitalWrite(ICM42688_CS, LOW);
  SPI.transfer(0x80 | reg);
  for (uint8_t i = 0; i < length; i++) {
    buffer[i] = SPI.transfer(0x00);
  }
  digitalWrite(ICM42688_CS, HIGH);
}

void setup() {
  pinMode(ICM42688_CS, OUTPUT);
  digitalWrite(ICM42688_CS, HIGH);

  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));

  Serial.begin(115200);

  // Check if the ICM-42688 is connected
  uint8_t who_am_i = readRegister(WHO_AM_I);
  Serial.print("WHO_AM_I: ");
  Serial.println(who_am_i, HEX);
  if (who_am_i != 0xEA) {
    Serial.println("ICM-42688 not found");
    while (1);
  }

  // Initialize the ICM-42688 (you may need to set up specific registers here)
  // ...
}

void loop() {
  uint8_t buffer[6];
  int16_t ax, ay, az, gx, gy, gz;

  // Read accelerometer data
  readMultipleRegisters(ACCEL_XOUT_H, buffer, 6);
  ax = (buffer[0] << 8) | buffer[1];
  ay = (buffer[2] << 8) | buffer[3];
  az = (buffer[4] << 8) | buffer[5];

  // Read gyroscope data
  readMultipleRegisters(GYRO_XOUT_H, buffer, 6);
  gx = (buffer[0] << 8) | buffer[1];
  gy = (buffer[2] << 8) | buffer[3];
  gz = (buffer[4] << 8) | buffer[5];

  // Print accelerometer and gyroscope data
  Serial.print("Accel: ");
  Serial.print(ax); Serial.print(" ");
  Serial.print(ay); Serial.print(" ");
  Serial.print(az); Serial.print(" | ");
  Serial.print("Gyro: ");
  Serial.print(gx); Serial.print(" ");
  Serial.print(gy); Serial.print(" ");
  Serial.print(gz); Serial.println();

  delay(500);
}
