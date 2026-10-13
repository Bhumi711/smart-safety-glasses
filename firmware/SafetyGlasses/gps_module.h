
#ifndef GPS_MODULE_H
#define GPS_MODULE_H

#include <Arduino.h>
#include <TinyGPSPlus.h>
#include "config.h"

struct GPSLocation {
  double latitude = 0.0;
  double longitude = 0.0;
  bool valid = false;
};

class GPSModule {
public:
  void begin(HardwareSerial &serialPort,
             uint32_t baudRate = GPS_BAUD_RATE) {
    gpsSerial = &serialPort;

    // GPS TX connects to ESP32 GPIO 1.
    gpsSerial->begin(
      baudRate,
      SERIAL_8N1,
      GPS_RX_PIN,
      -1
    );

    Serial.println("GPS serial started");
  }

  void update() {
    if (gpsSerial == nullptr) return;

    while (gpsSerial->available() > 0) {
      gps.encode(gpsSerial->read());
    }
  }

  GPSLocation getLocation() {
    GPSLocation location;

    if (gps.location.isValid() &&
        gps.location.age() < 10000) {
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
