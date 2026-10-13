
#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

#include <Arduino.h>
#include "esp_camera.h"
#include "SD_MMC.h"

class CameraModule {
public:
  bool begin() {
    camera_config_t config = {};

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;

    config.pin_d0 = 11;
    config.pin_d1 = 9;
    config.pin_d2 = 8;
    config.pin_d3 = 10;
    config.pin_d4 = 12;
    config.pin_d5 = 18;
    config.pin_d6 = 17;
    config.pin_d7 = 16;

    config.pin_xclk = 15;
    config.pin_pclk = 13;
    config.pin_vsync = 6;
    config.pin_href = 7;
    config.pin_sccb_sda = 4;
    config.pin_sccb_scl = 5;
    config.pin_pwdn = -1;
    config.pin_reset = -1;

    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_RGB565;
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

    if (esp_camera_init(&config) != ESP_OK) {
      Serial.println("Camera initialization FAILED");
      return false;
    }

    Serial.println("Camera initialized");
    return true;
  }

  bool captureToSD(const char *filename = "/sos_photo.jpg") {
    camera_fb_t *frame = esp_camera_fb_get();

    if (frame == nullptr) {
      Serial.println("Camera capture FAILED");
      return false;
    }

    SD_MMC.remove(filename);
    File file = SD_MMC.open(filename, FILE_WRITE);

    if (!file) {
      Serial.println("Failed to open photo file");
      esp_camera_fb_return(frame);
      return false;
    }

    size_t expected = frame->len;
    size_t written = file.write(frame->buf, expected);
    file.close();
    esp_camera_fb_return(frame);

    if (written != expected) {
      Serial.println("Photo write incomplete");
      return false;
    }

    Serial.printf("Photo saved: %s (%u bytes)\n",
                  filename, (unsigned)written);
    return true;
  }


  bool recordVideoFrames(const char *prefix,
                       uint8_t frameCount = 6,
                       uint16_t intervalMs = 500) {
    for (uint8_t n = 0; n < frameCount; n++) {
      camera_fb_t *frame = esp_camera_fb_get();

      if (!frame) {
        Serial.println("Video frame capture FAILED");
        return false;
      }

      uint32_t w = frame->width;
      uint32_t h = frame->height;
      uint32_t rowSize = (w * 3 + 3) & ~3;
      uint32_t imageSize = rowSize * h;
      uint8_t header[54] = {};

      header[0] = 'B';
      header[1] = 'M';

      auto put16 = [&](int p, uint16_t v) {
        header[p] = v & 255;
        header[p + 1] = v >> 8;
      };

      auto put32 = [&](int p, uint32_t v) {
        header[p] = v & 255;
        header[p + 1] = (v >> 8) & 255;
        header[p + 2] = (v >> 16) & 255;
        header[p + 3] = (v >> 24) & 255;
      };

      put32(2, 54 + imageSize);
      put32(10, 54);
      put32(14, 40);
      put32(18, w);
      put32(22, h);
      put16(26, 1);
      put16(28, 24);
      put32(34, imageSize);

      char filename[40];
      snprintf(filename, sizeof(filename),
         "/%s_frame%03u.bmp", prefix, n);
      SD_MMC.remove(filename);
      File file = SD_MMC.open(filename, FILE_WRITE);

      if (!file) {
        esp_camera_fb_return(frame);
        Serial.println("Cannot create video frame file");
        return false;
      }

      bool ok = file.write(header, 54) == 54;
      uint8_t *row = (uint8_t *)malloc(rowSize);

      if (!row) {
        file.close();
        esp_camera_fb_return(frame);
        Serial.println("Video row buffer allocation FAILED");
        return false;
      }

      if (frame->format != PIXFORMAT_RGB565 ||
          frame->len < (size_t)w * h * 2) {
        free(row);
        file.close();
        esp_camera_fb_return(frame);
        Serial.println("Unexpected camera frame format/size");
        return false;
      }

      for (int y = (int)h - 1; y >= 0 && ok; y--) {
        memset(row, 0, rowSize);

        for (uint32_t x = 0; x < w; x++) {
          uint16_t pixel =
              ((uint16_t)frame->buf[(y * w + x) * 2 + 1] << 8) |
              frame->buf[(y * w + x) * 2];

          uint8_t r = ((pixel >> 11) & 31) * 255 / 31;
          uint8_t g = ((pixel >> 5) & 63) * 255 / 63;
          uint8_t b = (pixel & 31) * 255 / 31;

          row[x * 3] = b;
          row[x * 3 + 1] = g;
          row[x * 3 + 2] = r;
        }

        if (file.write(row, rowSize) != rowSize) ok = false;
      }

      free(row);
      file.close();
      esp_camera_fb_return(frame);

      if (!ok) {
        Serial.println("Video frame write FAILED");
        return false;
      }

      Serial.printf("Saved video frame %u/%u\n", n + 1, frameCount);
      delay(intervalMs);
    }

    Serial.println("Video frames saved as BMP files");
    return true;
  }


};

#endif
