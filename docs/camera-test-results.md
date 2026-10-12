
# Camera Compatibility Test

- Board: ESP32-S3-WROOM-N16R8 development board
- Arduino board: ESP32S3 Dev Module
- USB serial port: COM12
- Camera ribbon marking: RHYX M21-45
- Detected sensor PID: 0x2145
- PSRAM: Enabled and detected
- Camera initialization: Successful
- Wi-Fi connection: Successful
- Live camera stream: Successful on phone
- Still image capture: Pending confirmation
- microSD image storage: Not tested
- SOS integration: Not tested

## Notes
- Working camera pin configuration:
  CAMERA_MODEL_ESP32S3_EYE
- Preserve the working camera test before changing settings.
- The selected pin mapping has not been independently verified
  against the board's actual circuit.
