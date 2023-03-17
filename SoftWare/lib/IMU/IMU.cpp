#include "IMU.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void GyroInit() {
  bno.begin();
  bno.setExtCrystalUse(false);
  delay(500);
}

int GyroGet() {
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  return euler.x();
}