
#include <Arduino.h>
#include "config.h"
#include "gps_module.h"
#include "bluetooth_module.h"
#include "audio_module.h"
#include "SD_MMC.h"
#include "camera_module.h"


enum SystemState {
  SYSTEM_READY,
  SYSTEM_EMERGENCY
};

SystemState systemState = SYSTEM_READY;

HardwareSerial GPSSerial(1);
GPSModule gps;
BluetoothModule bluetooth;
AudioModule audio;
bool audioReady = false;
CameraModule camera;
bool cameraReady = false;

bool lastRawButtonState = HIGH;
bool stableButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long lastGPSUpdate = 0;
unsigned long lastSOSCompleted = 0;
const unsigned long SOS_COOLDOWN_MS = 5000;

void activateSOS() {
  
if (systemState == SYSTEM_EMERGENCY) return;

systemState = SYSTEM_EMERGENCY;

if (SD_MMC.cardType() == CARD_NONE) {
  Serial.println("SD card unavailable; cannot save evidence.");
  return;
}

static uint16_t eventNumber = 1;
char eventPrefix[16];
char photoFilename[32];
char audioFilename[32];
char firstFrameFilename[40];

while (eventNumber <= 999) {
  snprintf(eventPrefix, sizeof(eventPrefix),
           "sos%03u", eventNumber);
  snprintf(photoFilename, sizeof(photoFilename),
           "/%s_photo.jpg", eventPrefix);
  snprintf(audioFilename, sizeof(audioFilename),
           "/%s_audio.wav", eventPrefix);
  snprintf(firstFrameFilename, sizeof(firstFrameFilename),
           "/%s_frame000.bmp", eventPrefix);

  if (!SD_MMC.exists(photoFilename) &&
      !SD_MMC.exists(audioFilename) &&
      !SD_MMC.exists(firstFrameFilename)) {
    break;
  }

  eventNumber++;
}

if (eventNumber > 999) {
  Serial.println("SOS event filename limit reached.");
  systemState = SYSTEM_READY;
  return;
}

Serial.printf("Starting event: %s\n", eventPrefix);
eventNumber++;

  GPSLocation location = gps.getLocation();

  String message;

  if (location.valid) {
    message = "SOS,LAT:";
    message += String(location.latitude, 6);
    message += ",LON:";
    message += String(location.longitude, 6);
  } else {
    message = "SOS,GPS_FIX_UNAVAILABLE";
  }

  Serial.println("SOS ACTIVATED");
  bluetooth.sendMessage(message);

  if (cameraReady && SD_MMC.cardType() != CARD_NONE) {
  bool photoSaved = camera.captureToSD(photoFilename);
  Serial.println(photoSaved
      ? "SOS photo saved"
      : "SOS photo save FAILED");
} else {
  Serial.println("Camera or SD card not ready");
}

if (cameraReady && SD_MMC.cardType() != CARD_NONE) {
  bool videoSaved =
    camera.recordVideoFrames(eventPrefix, 6, 500);
  Serial.println(videoSaved
      ? "Video frames saved"
      : "Video recording FAILED");
}

  if (audioReady && SD_MMC.cardType() != CARD_NONE) {
bool saved = audio.recordToSD(audioFilename, 10);
  Serial.println(saved ? "SOS audio saved" : "SOS audio save FAILED");
} else {
  Serial.println("Audio or SD card not ready");
}
lastSOSCompleted = millis();
systemState = SYSTEM_READY;
Serial.println("SOS workflow complete. Ready for next SOS.");

}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  delay(500);

  Serial.println();
  Serial.println("Smart Safety Glasses");
  Serial.println("Starting GPS + BLE + SOS...");

  pinMode(SOS_BUTTON_PIN, INPUT_PULLUP);

  gps.begin(GPSSerial, GPS_BAUD_RATE);
  bluetooth.begin(DEVICE_NAME);
  audioReady = audio.begin();
  cameraReady = camera.begin();

  if (!SD_MMC.setPins(39, 38, 40)) {
  Serial.println("Failed to set SD pins");
} else if (!SD_MMC.begin("/sdcard", true)) {
  Serial.println("SD initialization FAILED");
} else {
  Serial.println("SD card ready");
}

Serial.printf("Free PSRAM: %u bytes\n", ESP.getFreePsram());
Serial.printf("Largest PSRAM block: %u bytes\n", ESP.getMaxAllocPsram());

  Serial.println("Setup complete.");
  Serial.println("Press the SOS button to test.");
}

void loop() {
  gps.update();

  unsigned long now = millis();

  if (now - lastGPSUpdate >= GPS_UPDATE_INTERVAL_MS) {
    lastGPSUpdate = now;

    GPSLocation location = gps.getLocation();

    if (location.valid) {
      String message = "LAT:";
      message += String(location.latitude, 6);
      message += ",LON:";
      message += String(location.longitude, 6);
      Serial.println(message);
    } else {
      Serial.println("GPS_WAITING_FOR_FIX");
    }
  }

  bool rawButtonState = digitalRead(SOS_BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastDebounceTime = now;
    lastRawButtonState = rawButtonState;
  }

  if (now - lastDebounceTime >= BUTTON_DEBOUNCE_MS) {
    if (rawButtonState != stableButtonState) {
      stableButtonState = rawButtonState;

      if (stableButtonState == LOW &&
    systemState == SYSTEM_READY &&
    (lastSOSCompleted == 0 ||
     now - lastSOSCompleted >= SOS_COOLDOWN_MS)) {
  activateSOS();
}
    }
  }

  delay(5);
}
