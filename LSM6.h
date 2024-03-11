#ifndef LSM6_H
#define LSM6_H
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_Sensor.h>
#include "DataPacket.h"

void setupLSM6(); // LSM6DS3TRC IMU
void readLSM6(DataPacket* pData);

#endif