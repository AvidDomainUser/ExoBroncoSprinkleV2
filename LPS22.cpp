#include "LPS22.h"

Adafruit_LPS22 lps22;
sensors_event_t lps22_temp, lps22_pressure;

void setupLPS22()
{
  Serial.println("Adafruit LPS22 test!");

   // Try to initialize!
  if (!lps22.begin_I2C()) {
    Serial.println("Failed to find LPS22 chip");
    while (1) { delay(10); }
  }
  Serial.println("LPS22 Found!");

  lps22.setDataRate(LPS22_RATE_10_HZ);
  Serial.print("Data rate set to: ");
  switch (lps22.getDataRate()) {
    case LPS22_RATE_ONE_SHOT: Serial.println("One Shot / Power Down"); break;
    case LPS22_RATE_1_HZ: Serial.println("1 Hz"); break;
    case LPS22_RATE_10_HZ: Serial.println("10 Hz"); break;
    case LPS22_RATE_25_HZ: Serial.println("25 Hz"); break;
    case LPS22_RATE_50_HZ: Serial.println("50 Hz"); break;
    case LPS22_RATE_75_HZ: Serial.println("75 Hz"); break;
  }
}

void readLPS22(DataPacket* pData)
{
  lps22.getEvent(&lps22_pressure, &lps22_temp);
  pData->lps22_temp = lps22_temp.temperature;
  pData->lps22_press = lps22_pressure.pressure;
}