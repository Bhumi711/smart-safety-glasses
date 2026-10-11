
#ifndef GPS_MODULE_H
#define GPS_MODULE_H

#include <Arduino.h>
#include <TinyGPSPlus.h>

struct GPSLocation {
  double latitude = 0.0;
  double longitude = 0.0;
  bool valid = false;
};

class GPSModule {
public:
  void begin(HardwareSerial &serialPort,
             uint32_t baudRate = 9600) {
    gpsSerial = &serialPort;
    gpsSerial->begin(baudRate);
  }

  void update() {
    if (gpsSerial == nullptr) return;

    while (gpsSerial->available() > 0) {
      gps.encode(gpsSerial->read());
    }
  }

  GPSLocation getLocation() {
    GPSLocation location;

    if (gps.location.isValid()) {
      location.latitude = gps.location.lat();
      location.longitude = gps.location.lng();
      location.valid = true;
    }

    return location;
  }

private:
  TinyGPSPlus gps;
  HardwareSerial *gpsSerial = nullptr;
};

#endif
