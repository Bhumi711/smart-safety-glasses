
#include <Arduino.h>
#include "esp_camera.h"
#include <WiFi.h>
#include "board_config.h"

// Enter your 2.4 GHz Wi-Fi credentials here
const char *ssid = "Bhumi's A55";
const char *password = "bbbbbbbb";

void startCameraServer();
void setupLedFlash();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(1500);

  Serial.println("\nESP32-S3 camera test");
  Serial.printf("PSRAM detected: %s\n", psramFound() ? "YES" : "NO");

  camera_config_t config = {};
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_RGB565;
  config.frame_size = FRAMESIZE_QQVGA; // 160 x 120
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_count = 1;
  config.fb_location = psramFound()
                         ? CAMERA_FB_IN_PSRAM
                         : CAMERA_FB_IN_DRAM;

  Serial.println("Initializing camera...");
  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x\n", err);
    Serial.println("Check sensor support, GPIO mapping, and memory.");
    return;
  }

  sensor_t *sensor = esp_camera_sensor_get();
  if (sensor != nullptr) {
    Serial.printf("Camera detected. PID: 0x%04X\n", sensor->id.PID);
  }

  Serial.println("Camera initialized successfully.");

  // Continue to Wi-Fi only after successful camera initialization.
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("Connecting to Wi-Fi");
  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - start < 20000) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection failed; camera initialized.");
    return;
  }

  Serial.println("Wi-Fi connected");
  startCameraServer();

  Serial.print("Camera URL: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(1000);
}
