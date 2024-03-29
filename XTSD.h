#ifndef XTSD_FLASH_H
#define XTSD_FLASH_H
#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "DataPacket.h"

void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);
void setupXTSD();
String formatDataPacket(DataPacket* packet);
void writeXTSD(DataPacket* pData);

#endif