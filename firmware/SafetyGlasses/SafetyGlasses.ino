#include "bluetooth_module.h"
#include <Arduino.h>
#include "config.h"

enum SystemState {
  SYSTEM_READY,
  SYSTEM_EMERGENCY
};

SystemState systemState = SYSTEM_READY;

bool lastRawButtonState = HIGH;
bool stableButtonState = HIGH;

unsigned long lastDebounceTime = 0;

void handleSOS() {
  systemState = SYSTEM_EMERGENCY;

  Serial.println("================================");
  Serial.println("SOS ACTIVATED!");
  Serial.println("Emergency workflow started.");
  Serial.println("GPS, recording, storage and BLE");
  Serial.println("will be integrated in later steps.");
  Serial.println("================================");
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
    Serial.println("SOS button GPIO not configured yet.");
  }

  Serial.println("System ready.");
}

void loop() {
  // Do not access a GPIO until the board pin is verified.
  if (SOS_BUTTON_PIN < 0) {
    delay(100);
    return;
  }

  bool rawButtonState = digitalRead(SOS_BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastDebounceTime = millis();
    lastRawButtonState = rawButtonState;
  }

  if ((millis() - lastDebounceTime) >= BUTTON_DEBOUNCE_MS) {
    if (rawButtonState != stableButtonState) {
      stableButtonState = rawButtonState;

      // INPUT_PULLUP: LOW means pressed.
      if (stableButtonState == LOW &&
          systemState == SYSTEM_READY) {
        handleSOS();
      }
    }
  }

  delay(5);
}
