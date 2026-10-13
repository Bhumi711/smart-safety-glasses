
#include <Arduino.h>
#include "config.h"
#include "gps_module.h"
#include "bluetooth_module.h"

enum SystemState {
  SYSTEM_READY,
  SYSTEM_EMERGENCY
};

SystemState systemState = SYSTEM_READY;

HardwareSerial GPSSerial(1);
GPSModule gps;
BluetoothModule bluetooth;

bool lastRawButtonState = HIGH;
bool stableButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long lastGPSUpdate = 0;

void activateSOS() {
  if (systemState == SYSTEM_EMERGENCY) return;

  systemState = SYSTEM_EMERGENCY;
  GPSLocation location = gps.getLocation();

  String message;

  if (location.valid) {
    message = "SOS,LAT:";
    message += String(location.latitude, 6);
    message += ",LON:";
    message += String(location.longitude, 6);
  } else {
    message = "SOS,GPS_FIX_UNAVAILABLE";
  }

  Serial.println("SOS ACTIVATED");
  bluetooth.sendMessage(message);
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  delay(500);

  Serial.println();
  Serial.println("Smart Safety Glasses");
  Serial.println("Starting GPS + BLE + SOS...");

  pinMode(SOS_BUTTON_PIN, INPUT_PULLUP);

  gps.begin(GPSSerial, GPS_BAUD_RATE);
  bluetooth.begin(DEVICE_NAME);

  Serial.println("Setup complete.");
  Serial.println("Press the SOS button to test.");
}

void loop() {
  gps.update();

  unsigned long now = millis();

  if (now - lastGPSUpdate >= GPS_UPDATE_INTERVAL_MS) {
    lastGPSUpdate = now;

    GPSLocation location = gps.getLocation();

    if (location.valid) {
      String message = "LAT:";
      message += String(location.latitude, 6);
      message += ",LON:";
      message += String(location.longitude, 6);
      Serial.println(message);
    } else {
      Serial.println("GPS_WAITING_FOR_FIX");
    }
  }

  bool rawButtonState = digitalRead(SOS_BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastDebounceTime = now;
    lastRawButtonState = rawButtonState;
  }

  if (now - lastDebounceTime >= BUTTON_DEBOUNCE_MS) {
    if (rawButtonState != stableButtonState) {
      stableButtonState = rawButtonState;

      if (stableButtonState == LOW &&
          systemState == SYSTEM_READY) {
        activateSOS();
      }
    }
  }

  delay(5);
}
