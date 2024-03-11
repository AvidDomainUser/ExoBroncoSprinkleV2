#ifndef GPSMODULE_H
#define GPSMODULE_H
#include <Arduino.h>
#include <Adafruit_GPS.h>
#include "DataPacket.h"

void setupGPS();
void readGPS(DataPacket* pData);
/*int getHour();
int getMinute();
int getSecond();
int getMonth();
int getDay();
int getYear();
float getLat();
float getLong();
float getSpeed();
float getAngle();
float getAltitude();*/

#endif 
