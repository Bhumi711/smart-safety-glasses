
#ifndef BLUETOOTH_MODULE_H
#define BLUETOOTH_MODULE_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Custom UUIDs for this project.
// Use these same UUIDs in the Android app.
#define BLE_SERVICE_UUID \
  "6e400001-b5a3-f393-e0a9-e50e24dcca9e"

#define BLE_CHARACTERISTIC_UUID \
  "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

class BluetoothModule {
public:
  bool begin(const char *deviceName) {
    BLEDevice::init(deviceName);

    BLEServer *server = BLEDevice::createServer();

    BLEService *service =
      server->createService(BLE_SERVICE_UUID);

    characteristic = service->createCharacteristic(
      BLE_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_NOTIFY
    );

    characteristic->addDescriptor(new BLE2902());

    characteristic->setValue("READY");
    service->start();

    BLEAdvertising *advertising =
      BLEDevice::getAdvertising();

    advertising->addServiceUUID(BLE_SERVICE_UUID);
    advertising->start();

    return true;
  }

  void sendMessage(const String &message) {
    if (characteristic == nullptr) return;

    characteristic->setValue(message.c_str());
    characteristic->notify();
  }

private:
  BLECharacteristic *characteristic = nullptr;
};

#endif
