#ifndef LPS22_H
#define LPS22_H
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>
#include "DataPacket.h"

void setupLPS22();
void readLPS22(DataPacket* pData);

#endif