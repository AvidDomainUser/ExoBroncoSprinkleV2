#ifndef XTSD_FLASH_H
#define XTSD_FLASH_H
#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <cstdio>
#include "DataPacket.h"

void setupXTSD();
bool writeXTSD(DataPacket* pData);

#endif