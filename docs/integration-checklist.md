
# Hardware Integration Checklist

## Stage 1: Board verification
- [ ] Inspect the ESP32-S3 board and camera.
- [ ] Confirm USB connection and serial output.
- [ ] Verify the board pinout and microSD configuration.
- [ ] Check the power supply.

## Stage 2: Individual peripherals
- [ ] Test SOS button.
- [ ] Test NEO-6M GPS outdoors.
- [ ] Test BLE connection with Android app.
- [ ] Initialize camera and capture an image.
- [ ] Initialize microSD and save a test file.
- [ ] Test INMP441 audio capture.

## Stage 3: Integration
- [ ] Send SOS event to the phone.
- [ ] Include valid GPS coordinates when available.
- [ ] Save emergency logs to microSD.
- [ ] Test evidence capture.
- [ ] Test phone-mediated trusted-contact alert.
- [ ] Test location updates.
- [ ] Test the complete workflow.

## Reliability checks
- [ ] Handle unavailable GPS.
- [ ] Handle failed microSD initialization.
- [ ] Handle Bluetooth disconnection.
- [ ] Check power stability.
- [ ] Verify recorded files can be opened.
