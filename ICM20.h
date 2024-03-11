#ifndef ICM20_H
#define ICM20_H
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ICM20649.h>
#include <Adafruit_Sensor.h>
#include "DataPacket.h"

void setupICM20(); // ICM20649 IMU
void readICM20(DataPacket* pData);

#endif
