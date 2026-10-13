
#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_BAUD_RATE 115200

// SOS push button: GPIO 3 to GND
// INPUT_PULLUP means pressed = LOW
#define SOS_BUTTON_PIN 3
#define BUTTON_DEBOUNCE_MS 50

// GPS module: GPS TX -> ESP32 GPIO 1
#define GPS_RX_PIN 1
#define GPS_BAUD_RATE 9600

// GPS update interval
#define GPS_UPDATE_INTERVAL_MS 5000

// BLE device name
#define DEVICE_NAME "SafetyGlasses"

// BLE service and characteristic UUIDs
#define BLE_SERVICE_UUID \
  "6e400001-b5a3-f393-e0a9-e50e24dcca9e"

#define BLE_CHARACTERISTIC_UUID \
  "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

#endif
