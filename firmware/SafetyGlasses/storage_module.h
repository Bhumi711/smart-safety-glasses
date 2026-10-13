
#ifndef STORAGE_MODULE_H
#define STORAGE_MODULE_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

class StorageModule {
public:
  // Call only after the correct SD interface and pin mapping
  // have been verified for this specific ESP32-S3 board.
  bool begin(uint8_t chipSelectPin) {
    ready = false;

    Serial.println(
      "SD initialization requested. "
      "Verify board-specific wiring before use."
    );

    // Generic SPI initialization is intentionally disabled
    // until the board's SD pin mapping is confirmed.
    (void)chipSelectPin;

    Serial.println("microSD initialization skipped.");
    return false;
  }

  bool saveText(const char *path, const String &content) {
    if (!ready) {
      Serial.println("microSD unavailable; text not saved.");
      return false;
    }

    File file = SD.open(path, FILE_APPEND);

    if (!file) {
      Serial.println("Could not open SD file.");
      return false;
    }

    size_t written = file.println(content);
    file.close();

    return written > 0;
  }

  bool isReady() const {
    return ready;
  }

private:
  bool ready = false;
};

#endif
