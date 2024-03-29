#include "XTSD.h"

#define DATE "20240327"
#define TRIAL "1"

#define NUM_FIELDS 24

// Define a macro to convert a variable name to a string
#define STR(x) #x

char filename[50];
// Define an array of variable names
const char* fieldNames[NUM_FIELDS] = {
    STR(Time), STR(Date),
    STR(MS8607 Tmp), STR(MS8607 Pres), STR(MS8607 Hum),
    STR(LPS22 Tmp), STR(LPS22 Pres),
    STR(LSM Ax), STR(LSM Ay), STR(LSM Az),
    STR(LSM Gx), STR(LSM Gy), STR(LSM Gz),
    STR(ICM Ax), STR(ICM Ay), STR(ICM Az),
    STR(ICM Gx), STR(ICM Gy), STR(ICM Gz),
    STR(Lat), STR(Long), STR(Alt),
    STR(Speed),
    STR(Angle),
};

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append to an already existing file
void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(!file.print(message)){
      // Serial.println("Message appended");
      
  //} else {
    Serial.println("Append failed");
  }
  file.close();
}

void setupXTSD()
{
  Serial.begin(115200);
  delay(3000);

  // Initializing SD chip
  Serial.println("Mounting Flash Memory...\n");
  if(!SD.begin(3)){
    Serial.println("Card Mount Failed");
    return;
  }

  uint8_t cardType = SD.cardType();
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

  snprintf(filename, sizeof(filename), "/sprinkle_log_%s_TRIAL_%s.txt", DATE, TRIAL);

  Serial.printf("Creating %s", filename);

  String headers;
  for (int i = 0; i < NUM_FIELDS; i++) {
    headers += fieldNames[i];
    if (i < NUM_FIELDS - 1) {
      headers += ", "; // Tab separator
    }
  }

  writeFile(SD, filename, headers.c_str());  
}

String formatDataPacket(DataPacket* packet) {
  // Format the string with tab-separated values
  return packet->timestamp + ", " + packet->datestamp + ", " +
         String(packet->ms8607_temp) + ", " + String(packet->ms8607_press) + ", " + String(packet->ms8607_humid) + ", " +
         String(packet->lps22_temp) + ", " + String(packet->lps22_press) + ", " +
         String(packet->lsm_accel[0]) + ", " + String(packet->lsm_accel[1]) + ", " + String(packet->lsm_accel[2]) + ", " +
         String(packet->lsm_gyro[0]) + ", " + String(packet->lsm_gyro[1]) + ", " + String(packet->lsm_gyro[2]) + ", " +
         String(packet->icm_accel[0]) + ", " + String(packet->icm_accel[1]) + ", " + String(packet->icm_accel[2]) + ", " +
         String(packet->icm_gyro[0]) + ", " + String(packet->icm_gyro[1]) + ", " + String(packet->icm_gyro[2]) + ", " +
         String(packet->latitude) + ", " + String(packet->longitude) + ", " + String(packet->altitude) + ", " + 
         String(packet->speed) + ", " +
         String(packet->angle) + "\n";
}

void writeXTSD(DataPacket* pData)
{
  String formattedPacket = formatDataPacket(pData);
  appendFile(SD, filename, formattedPacket.c_str());
}