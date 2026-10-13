
#ifndef AUDIO_MODULE_H
#define AUDIO_MODULE_H

#include <Arduino.h>
#include <ESP_I2S.h>
#include "SD_MMC.h"

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

  bool recordToSD(const char *filename, uint32_t seconds = 10) {
    const uint32_t sampleRate = 16000;
    const uint32_t bytesPerSample = 2;
    const size_t audioBytes =
        (size_t)sampleRate * seconds * bytesPerSample;

    uint8_t *buffer = (uint8_t *)ps_malloc(audioBytes + 44);

    if (!buffer) {
      Serial.println("Audio buffer allocation FAILED");
      return false;
    }

    uint8_t *pcm = buffer + 44;

    Serial.printf("Recording audio for %lu seconds...\n",
                  (unsigned long)seconds);

    for (size_t i = 0; i < audioBytes; i += 2) {
      int32_t sample = i2s.read();
      int16_t pcmSample = (int16_t)(sample >> 16);
      pcm[i] = pcmSample & 0xFF;
      pcm[i + 1] = (pcmSample >> 8) & 0xFF;
    }

    write16(buffer + 20, 1);
    write16(buffer + 22, 1);
    write32(buffer + 24, sampleRate);
    write32(buffer + 28, sampleRate * bytesPerSample);
    write16(buffer + 32, bytesPerSample);
    write16(buffer + 34, 16);

    memcpy(buffer, "RIFF", 4);
    write32(buffer + 4, audioBytes + 36);
    memcpy(buffer + 8, "WAVEfmt ", 8);
    write32(buffer + 16, 16);
    memcpy(buffer + 36, "data", 4);
    write32(buffer + 40, audioBytes);

    File file = SD_MMC.open(filename, FILE_WRITE);

    if (!file) {
      Serial.println("Failed to open audio file on SD");
      free(buffer);
      return false;
    }

    size_t written = file.write(buffer, audioBytes + 44);
    file.close();
    free(buffer);

    if (written != audioBytes + 44) {
      Serial.println("Audio file write incomplete");
      return false;
    }

    Serial.printf("Audio saved: %s (%u bytes)\n",
                  filename, (unsigned)written);
    return true;
  }

private:
  I2SClass i2s;

  void write16(uint8_t *p, uint16_t v) {
    p[0] = v & 0xFF;
    p[1] = (v >> 8) & 0xFF;
  }

  void write32(uint8_t *p, uint32_t v) {
    p[0] = v & 0xFF;
    p[1] = (v >> 8) & 0xFF;
    p[2] = (v >> 16) & 0xFF;
    p[3] = (v >> 24) & 0xFF;
  }
};

#endif
