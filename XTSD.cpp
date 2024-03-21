#include "XTSD.h"
#include <FreeRTOS.h>

char dataFilePath = "/data.txt";

bool writeFile(fs::FS &fs, const char * path, const char * message){
  bool status = true;
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return false;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
    status = false;
  }
  file.close();
  return status;
}

bool succ_load = false;
void setupXTSD() {
	Serial.print("Mounting SD Card: ");
	if (!SD.begin(3)) {
		Serial.println("Failed Card Mount! Fatal.");
		return;
	}
  Serial.println("Success!");

	uint8_t card_type = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  // Should return SDSC
  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  if (!writeFile(SD, dataFilePath, "Starting up!!\n")) return;

  chronos = xTimerCreate("SD Time Tracker", )

	succ_load = true;
}

bool writeXTSD(DataPacket *pData) {
  if (!succ_load) Serial.println("Cannot write to SD due to unsuccessful load!"), return false;

  //Create Message
  char msg[1024]; //Unsure because we need this much space in case of long numbers

  sprintf(msg, "%s - %s; ms8607: %f, %f, %f; lps22: %f, %f; lsm: (%f, %f, %f), (%f, %f, %f); icm: (%f, %f, %f), (%f, %f, %f); gps: %f, %f, %f, %f, %f\n\0",  //TODO: figure out if it's necessary to add the \0
    pData->datestamp, pData->timestamp,
    pData->ms8607_temp, pData->ms8607_press, pData->ms8607_humid,
    pData->lps22_temp, pData->lps22_press,
    pData->lsm_accel[0], pData->lsm_accel[1], pData->lsm_accel[2], pData->lsm_gyro[0], pData->lsm_gyro[1], pData->lsm_gyro[2], 
    pData->ism_accel[0], pData->ism_accel[1], pData->ism_accel[2], pData->ism_gyro[0], pData->ism_gyro[1], pData->ism_gyro[2],
    pData->latitude, pData->longitude, pData->speed, pData->angle, pData->altitude
  );

  bool succ = writeFile(SD, dataFilePath, msg);

  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));

  return succ;
}