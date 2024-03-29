#ifndef DATAPACKET_H
#define DATAPACKET_H

struct DataPacket {
  String timestamp = "";
  String datestamp = "";
  volatile float ms8607_temp = 0.0;
  volatile float ms8607_press = 0.0;
  volatile float ms8607_humid = 0.0;
  volatile float lps22_temp = 0.0;
  volatile float lps22_press = 0.0;
  volatile float lsm_accel[3];
  volatile float lsm_gyro[3];
  volatile float icm_accel[3];
  volatile float icm_gyro[3];
  volatile float latitude = 0.0;
  volatile float longitude = 0.0;
  volatile float speed = 0.0;
  volatile float angle = 0.0;
  volatile float altitude = 0.0;
};

#endif 
