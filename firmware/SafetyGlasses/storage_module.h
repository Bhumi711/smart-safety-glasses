
#ifndef STORAGE_MODULE_H
#define STORAGE_MODULE_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

class StorageModule {
public:
  bool begin(uint8_t chipSelectPin) {
    ready = SD.begin(chipSelectPin);

    if (ready) {
      Serial.println("microSD initialized.");
    } else {
      Serial.println("microSD initialization failed.");
    }

    return ready;
  }

  bool saveText(const char *path, const String &content) {
    if (!ready) return false;

    File file = SD.open(path, FILE_APPEND);
    if (!file) {
      Serial.println("Could not open file.");
      return false;
    }

    file.println(content);
    file.close();

    return true;
  }

  bool isReady() const {
    return ready;
  }

private:
  bool ready = false;
};

#endif
