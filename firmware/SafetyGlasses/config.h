
#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_BAUD_RATE 115200

// Temporary placeholder.
// Replace with a verified, available GPIO after checking the board pinout.
#define SOS_BUTTON_PIN -1

// Button uses INPUT_PULLUP, so pressed = LOW.
#define BUTTON_DEBOUNCE_MS 50

// GPS update interval
#define GPS_UPDATE_INTERVAL_MS 5000

// BLE device name
#define DEVICE_NAME "SafetyGlasses"

#endif
