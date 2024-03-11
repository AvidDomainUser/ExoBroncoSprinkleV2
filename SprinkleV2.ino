#include "MS8607.h"
#include "LPS22.h"
#include "LSM6.h"
#include "ICM20.h"
#include "GPS.h"

#include "DataPacket.h"

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

DataPacket data;

// Define the mutex: remedies resource conflicts when
// multiple tasks try to write to the data packet
SemaphoreHandle_t mutex;

TaskHandle_t ms8607_handle = NULL;
TaskHandle_t lps22_handle = NULL;
TaskHandle_t lsm6_handle = NULL;
TaskHandle_t icm20_handle = NULL;
TaskHandle_t gps_handle = NULL;

// Task 1 - Pressure Sensor MS8607
// Uses about 1692 words in stack
void ms8607_task(void *pvParameters) {
  // Pointer to DataPacket data
  DataPacket* p_data = (DataPacket*)pvParameters;
  for (;;) {
    // Attempt to take the mutex
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      Serial.print("Pressure Sensor MS8607 running on core: ");
      Serial.println(xPortGetCoreID());

      readMS8607(p_data);

      Serial.print("Temp: "); Serial.println(p_data->ms8607_temp);
      Serial.print("Humidity: "); Serial.println(p_data->ms8607_humid);
      Serial.print("Pressure: "); Serial.println(p_data->ms8607_press);

      // Leave in to observe as peripherals are added
      Serial.print("# of words in stack currently used: ");
      Serial.println(uxTaskGetStackHighWaterMark(NULL));

      xSemaphoreGive(mutex); // Release the mutex
      
      vTaskDelay(pdMS_TO_TICKS(10)); // 10ms/100Hz Delay
    }
  }
}

// Task 2 - Pressure Sensor LPS22
// Uses about XXX words in stack
void lps22_task(void *pvParameters) {
  // Pointer to DataPacket data
  DataPacket* p_data = (DataPacket*)pvParameters;
  for (;;) {
    // Attempt to take the mutex
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      Serial.print("Pressure Sensor LPS22 running on core: ");
      Serial.println(xPortGetCoreID());

      readLPS22(p_data);

      Serial.print("Temp: "); Serial.println(p_data->lps22_temp);
      Serial.print("Pressure: "); Serial.println(p_data->lps22_press);

      // Leave in to observe as peripherals are added
      Serial.print("# of words in stack currently used: ");
      Serial.println(uxTaskGetStackHighWaterMark(NULL));

      xSemaphoreGive(mutex); // Release the mutex
      
      vTaskDelay(pdMS_TO_TICKS(10)); // 10ms/100Hz Delay
    }
  }
}

// Task 3 - IMU LSM6DS3TRC
// Uses about 376 words in stack
void lsm6_task(void *pvParameters) {
  // Pointer to DataPacket data
  DataPacket* p_data = (DataPacket*)pvParameters;
  for (;;) {
    // Attempt to take the mutex
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      Serial.print("IMU LSM6 running on core: ");
      Serial.println(xPortGetCoreID());

      readLSM6(p_data);

      Serial.print("LSM Ax: "); Serial.println(p_data->lsm_accel[0]);
      Serial.print("LSM Ay: "); Serial.println(p_data->lsm_accel[1]);
      Serial.print("LSM Az: "); Serial.println(p_data->lsm_accel[2]);

      Serial.print("LSM Gx: "); Serial.println(p_data->lsm_gyro[0]);
      Serial.print("LSM Gy: "); Serial.println(p_data->lsm_gyro[1]);
      Serial.print("LSM Gz: "); Serial.println(p_data->lsm_gyro[2]);

      // Leave in to observe as peripherals are added
      Serial.print("# of words in stack currently used: ");
      Serial.println(uxTaskGetStackHighWaterMark(NULL));

      xSemaphoreGive(mutex); // Release the mutex
      
      vTaskDelay(pdMS_TO_TICKS(10)); // 10ms/100Hz Delay
    }
  }
}

// Task 4 - IMU ICM20649
// Uses about 170 words in stack
void icm20_task(void *pvParameters) {
  // Pointer to DataPacket data
  DataPacket* p_data = (DataPacket*)pvParameters;
  for (;;) {
    // Attempt to take the mutex
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      Serial.print("IMU ICM20 running on core: ");
      Serial.println(xPortGetCoreID());

      readICM20(p_data);

      Serial.print("ICM Ax: "); Serial.println(p_data->icm_accel[0]);
      Serial.print("ICM Ay: "); Serial.println(p_data->icm_accel[1]);
      Serial.print("ICM Az: "); Serial.println(p_data->icm_accel[2]);

      Serial.print("ICM Gx: "); Serial.println(p_data->icm_gyro[0]);
      Serial.print("ICM Gy: "); Serial.println(p_data->icm_gyro[1]);
      Serial.print("ICM Gz: "); Serial.println(p_data->icm_gyro[2]);

      // Leave in to observe as peripherals are added
      Serial.print("# of words in stack currently used: ");
      Serial.println(uxTaskGetStackHighWaterMark(NULL));

      xSemaphoreGive(mutex); // Release the mutex
      
      vTaskDelay(pdMS_TO_TICKS(10)); // 10ms/100Hz Delay
    }
  }
}

// Task 5 - GPS Currently using 1792 words in stack
void gps_task(void *pvParameters) {
  DataPacket* p_data = (DataPacket*)pvParameters;
  for (;;) {
    // Attempt to take the mutex
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      Serial.print("GPS running on core: ");
      Serial.println(xPortGetCoreID());

      readGPS(p_data);

      Serial.print("Latitude: "); Serial.println(p_data->latitude);
      Serial.print("Longitude: "); Serial.println(p_data->longitude);
      Serial.print("Altitude: "); Serial.println(p_data->altitude);

      // Leave in to observe as peripherals are added
      Serial.print("# of words in stack currently used: ");
      Serial.println(uxTaskGetStackHighWaterMark(NULL));

      xSemaphoreGive(mutex); // Release the mutex
      
      vTaskDelay(pdMS_TO_TICKS(10)); // 1s Delay
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Beginning Sprinkle");
  setupMS8607();
  // setupLPS22(); NOT CONNECTED PROPERLY
  setupLSM6();
  setupICM20();
  setupGPS();

  mutex = xSemaphoreCreateMutex();

  // Create Pressure Sensor (MS8607) on Core 0
  xTaskCreatePinnedToCore(
    ms8607_task,     // Function to execute
    "MS8607 Task",      // Name of the task
    2000,            // Stack size (words)
    &data,           // Task parameters
    1,               // Priority
    &ms8607_handle,  // Task handle
    0                // Core to run on (Core 0)
  );

  // // Create Pressure Sensor (LPS22) on Core 0
  // xTaskCreatePinnedToCore(
  //   lps22_task,     // Function to execute
  //   "LPS22 Task",      // Name of the task
  //   2000,            // Stack size (words)
  //   &data,           // Task parameters
  //   1,               // Priority
  //   &lps22_handle,  // Task handle
  //   0                // Core to run on (Core 0)
  // );

  // IMU LSM6 Task
  xTaskCreatePinnedToCore(
    lsm6_task,     // Function to execute
    "LSM6 Task",      // Name of the task
    2000,            // Stack size (words)
    &data,           // Task parameters
    1,               // Priority
    &lsm6_handle,  // Task handle
    0                // Core to run on (Core 0)
  );

  // IMU ICM20 Task
  xTaskCreatePinnedToCore(
    icm20_task,     // Function to execute
    "ICM20 Task",      // Name of the task
    2000,            // Stack size (words)
    &data,           // Task parameters
    1,               // Priority
    &icm20_handle,  // Task handle
    0                // Core to run on (Core 0)
  );

  // GPS Task
  xTaskCreatePinnedToCore(
    gps_task,         // Function to execute
    "GPS Task",       // Name of the task
    6000,          // Stack size (words)
    &data,          // Task parameters
    1,             // Priority
    &gps_handle,  // Task handle
    0              // Core to run on (Core 0)
  );
}

void loop() {
}
