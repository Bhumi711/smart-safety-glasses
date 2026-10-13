
#ifndef BLUETOOTH_MODULE_H
#define BLUETOOTH_MODULE_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "config.h"

class BluetoothModule {
public:
  bool begin(const char *deviceName) {
    BLEDevice::init(deviceName);

    server = BLEDevice::createServer();
    server->setCallbacks(new ServerCallbacks(this));

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

    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(BLE_SERVICE_UUID);
    advertising->start();

    Serial.println("BLE advertising started");
    return true;
  }

  void sendMessage(const String &message) {
    if (characteristic == nullptr) {
      Serial.println("BLE not initialized");
      return;
    }

    characteristic->setValue(message.c_str());

    if (deviceConnected) {
      characteristic->notify();
      Serial.println("BLE notification sent to connected device");
    } else {
      Serial.println("BLE message updated; no phone connected");
    }

    Serial.println(message);
  }

  bool isConnected() const {
    return deviceConnected;
  }

private:
  BLEServer *server = nullptr;
  BLECharacteristic *characteristic = nullptr;
  bool deviceConnected = false;

  class ServerCallbacks : public BLEServerCallbacks {
  public:
    explicit ServerCallbacks(BluetoothModule *module)
      : module(module) {}

    void onConnect(BLEServer *server) override {
      module->deviceConnected = true;
      Serial.println("BLE phone connected");
    }

    void onDisconnect(BLEServer *server) override {
      module->deviceConnected = false;
      Serial.println("BLE phone disconnected");
      BLEDevice::startAdvertising();
    }

  private:
    BluetoothModule *module;
  };
};

#endif
