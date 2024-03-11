#include "ICM20.h"

Adafruit_ICM20649 icm;

sensors_event_t icm_accel, icm_gyro, icm_temp;

void setupICM20() {

  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit ICM20649 test!");

  // Try to initialize!
  if (!icm.begin_I2C()) {
    // if (!icm.begin_SPI(ICM_CS)) {
    // if (!icm.begin_SPI(ICM_CS, ICM_SCK, ICM_MISO, ICM_MOSI)) {

    Serial.println("Failed to find ICM20649 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("ICM20649 Found!");
  // icm.setAccelRange(ICM20649_ACCEL_RANGE_4_G);
  Serial.print("Accelerometer range set to: ");
  switch (icm.getAccelRange()) {
  case ICM20649_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case ICM20649_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case ICM20649_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
    break;
  case ICM20649_ACCEL_RANGE_30_G:
    Serial.println("+-30G");
    break;
  }

  // icm.setGyroRange(ICM20649_GYRO_RANGE_500_DPS);
  Serial.print("Gyro range set to: ");
  switch (icm.getGyroRange()) {
  case ICM20649_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    break;
  case ICM20649_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    break;
  case ICM20649_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    break;
  case ICM20649_GYRO_RANGE_4000_DPS:
    Serial.println("4000 degrees/s");
    break;
  }

  //  icm.setAccelRateDivisor(4095);
  uint16_t accel_divisor = icm.getAccelRateDivisor();
  float accel_rate = 1125 / (1.0 + accel_divisor);

  Serial.print("Accelerometer data rate divisor set to: ");
  Serial.println(accel_divisor);
  Serial.print("Accelerometer data rate (Hz) is approximately: ");
  Serial.println(accel_rate);

  //  icm.setGyroRateDivisor(255);
  uint8_t gyro_divisor = icm.getGyroRateDivisor();
  float gyro_rate = 1100 / (1.0 + gyro_divisor);

  Serial.print("Gyro data rate divisor set to: ");
  Serial.println(gyro_divisor);
  Serial.print("Gyro data rate (Hz) is approximately: ");
  Serial.println(gyro_rate);
  Serial.println();
}

void readICM20(DataPacket* pData)
{
  icm.getEvent(&icm_accel, &icm_gyro, &icm_temp);

  /* Display the results (acceleration is measured in m/s^2) */
  pData->icm_accel[0] = icm_accel.acceleration.x;
  pData->icm_accel[1] = icm_accel.acceleration.y;
  pData->icm_accel[2] = icm_accel.acceleration.z;

  /* Display the results (rotation is measured in rad/s) */
  pData->icm_gyro[0] =icm_gyro.gyro.x;
  pData->icm_gyro[1] =icm_gyro.gyro.y;
  pData->icm_gyro[2] =icm_gyro.gyro.z;
}