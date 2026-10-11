
#ifndef AUDIO_MODULE_H
#define AUDIO_MODULE_H

#include <Arduino.h>
#include "driver/i2s_std.h"

class AudioModule {
public:
  bool begin() {
    // I2S pin mapping and initialization will be
    // completed after verifying the exact board wiring.
    Serial.println("Audio module prepared.");
    Serial.println("Waiting for verified I2S pin configuration.");

    return false;
  }

  bool isReady() const {
    return initialized;
  }

private:
  bool initialized = false;
  i2s_chan_handle_t rxHandle = nullptr;
};

#endif
