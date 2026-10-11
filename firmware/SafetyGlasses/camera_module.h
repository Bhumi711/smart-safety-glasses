
#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

#include <Arduino.h>
#include "esp_camera.h"

class CameraModule {
public:
  bool begin() {
    // Camera GPIO configuration depends on the exact board.
    // Do not initialize until the board pin mapping is verified.
    Serial.println("Camera module prepared.");
    Serial.println("Waiting for verified camera pin configuration.");

    return false;
  }

  camera_fb_t* captureFrame() {
    if (!initialized) return nullptr;

    return esp_camera_fb_get();
  }

  void releaseFrame(camera_fb_t* frame) {
    if (frame != nullptr) {
      esp_camera_fb_return(frame);
    }
  }

  bool isReady() const {
    return initialized;
  }

private:
  bool initialized = false;
};

#endif
