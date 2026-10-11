#include <Arduino.h>
#include "config.h"

#include "gps_module.h"
#include "bluetooth_module.h"
#include "storage_module.h"
#include "camera_module.h"
#include "audio_module.h"

enum SystemState {
  SYSTEM_READY,
  SYSTEM_EMERGENCY
};

SystemState systemState = SYSTEM_READY;

BluetoothModule bluetooth;
GPSModule gps;
StorageModule storage;
CameraModule camera;
AudioModule audio;

bool lastRawButtonState = HIGH;
bool stableButtonState = HIGH;
unsigned long lastDebounceTime = 0;

void activateSOS() {
  if (systemState == SYSTEM_EMERGENCY) return;

  systemState = SYSTEM_EMERGENCY;

  Serial.println("SOS ACTIVATED");
  Serial.println("Emergency workflow started.");

  // TODO: Read current GPS location.
  // TODO: Send SOS message through BLE.
  // TODO: Save event to microSD.
  // TODO: Start evidence capture when implemented.
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  delay(500);

  Serial.println();
  Serial.println("Smart Safety Glasses");
  Serial.println("Firmware starting...");

  if (SOS_BUTTON_PIN >= 0) {
    pinMode(SOS_BUTTON_PIN, INPUT_PULLUP);
    Serial.println("SOS button configured.");
  } else {
    Serial.println("SOS GPIO pending board verification.");
  }

  // Peripheral initialization will be added after
  // the exact board pin mapping is verified.
  Serial.println("System setup complete.");
}

void loop() {
  if (SOS_BUTTON_PIN < 0) {
    delay(100);
    return;
  }

  bool rawButtonState = digitalRead(SOS_BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastDebounceTime = millis();
    lastRawButtonState = rawButtonState;
  }

  if (millis() - lastDebounceTime >= BUTTON_DEBOUNCE_MS) {
    if (rawButtonState != stableButtonState) {
      stableButtonState = rawButtonState;

      if (stableButtonState == LOW &&
          systemState == SYSTEM_READY) {
        activateSOS();
      }
    }
  }

  // Non-blocking GPS/BLE/storage tasks will be added here.
  delay(5);
}
