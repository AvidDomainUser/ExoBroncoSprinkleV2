#ifndef MS8607_H
#define MS8607_H
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MS8607.h>
#include <Adafruit_Sensor.h>
#include "DataPacket.h"

void setupMS8607();
void readMS8607(DataPacket* pData);

#endif
