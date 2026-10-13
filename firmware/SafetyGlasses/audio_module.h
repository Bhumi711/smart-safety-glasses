
#ifndef AUDIO_MODULE_H
#define AUDIO_MODULE_H

#include <Arduino.h>
#include <ESP_I2S.h>

class AudioModule {
public:
  bool begin() {
    i2s.setPins(14, 47, -1, 48, -1);

    if (!i2s.begin(
          I2S_MODE_STD,
          16000,
          I2S_DATA_BIT_WIDTH_32BIT,
          I2S_SLOT_MODE_MONO)) {
      Serial.println("Microphone initialization FAILED");
      return false;
    }

    Serial.println("Microphone initialized");
    return true;
  }

  uint32_t readSoundLevel() {
    uint64_t total = 0;
    const int count = 200;

    for (int i = 0; i < count; i++) {
      int32_t sample = i2s.read();

      total += sample < 0
          ? -(int64_t)sample
          : (int64_t)sample;
    }

    return total / count;
  }

private:
  I2SClass i2s;
};

#endif
