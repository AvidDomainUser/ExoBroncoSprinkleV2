#include "GPS.h"

#define RX_PIN 44
#define TX_PIN 43

HardwareSerial gpsSerial(2);
Adafruit_GPS GPS(&gpsSerial);

#define GPSECHO  false

void setupGPS() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("Adafruit GPS library basic test!");
  
  GPS.begin(9600);

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  gpsSerial.println(PMTK_Q_RELEASE);
}

void readGPS(DataPacket* pData) {
  char c = GPS.read();
  if (c && GPSECHO) {
    Serial.write(c);
  }

  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) {
      return;
    }
  }
/*
  pData->hour = GPS.hour;
  pData->minute = GPS.minute;
  pData->second = GPS.seconds;
  pData->millisecond = GPS.milliseconds;
  pData->day = GPS.day;
  pData->month = GPS.month;
  pData->year = GPS.year;
*/
  //Serial.println("Reading GPS");
  if (GPS.fix) {
    //Serial.println("Getting Fix");
    /*float latitude = (float) GPS.latitudeDegrees;
    Serial.println(latitude, 8);
    pData->latitude = latitude;*/

    pData->latitude = GPS.latitudeDegrees;
    pData->longitude = GPS.longitudeDegrees;
    pData->altitude = GPS.altitude;
  }
}
