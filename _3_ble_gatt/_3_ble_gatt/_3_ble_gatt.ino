/*
    Based on Alexander Lavrushko, - BLEProof Peripheral ESP32
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <stdio.h>

// --------
// Constants
// --------
#define SERVICE_UUID        BLEUUID((uint16_t)0x181A)   //Enviromental Sensing
#define CHAR_READ_UUID      BLEUUID((uint16_t)0x2A6E)   //Temperature

// --------
// Global variables
// --------
static BLEServer* g_pServer = nullptr;
static BLECharacteristic* g_pCharRead = nullptr;
static bool g_centralConnected = false;
static std::string g_cmdLine;
char temperature[10];
uint16_t value = 0;

// --------
// Bluetooth event callbacks
// --------
class MyServerCallbacks: public BLEServerCallbacks
{
    void onConnect(BLEServer* pServer) override
    {
      Serial.println("onConnect");
      g_centralConnected = true;
    }

    void onDisconnect(BLEServer* pServer) override
    {
      Serial.println("onDisconnect, will start advertising");
      g_centralConnected = false;
      BLEDevice::startAdvertising();
    }
};

class MyCharPrintingCallbacks: public BLECharacteristicCallbacks
{
  public:
    explicit MyCharPrintingCallbacks(const char* name) : m_name(name) {}

  private:
    void PrintEvent(const char* event, const char* value)
    {
      Serial.print(event);
      Serial.print("(");
      Serial.print(m_name.c_str());
      Serial.print(")");
      if (value)
      {
        Serial.print(" value='");
        Serial.print(value);
        Serial.print("'");
      }
      Serial.println();
    }

  private:
    void onRead(BLECharacteristic* pCharacteristic) override
    {
      PrintEvent("onRead", pCharacteristic->getValue().c_str());
    }

    void onWrite(BLECharacteristic* pCharacteristic) override
    {
      PrintEvent("onWrite", pCharacteristic->getValue().c_str());
    }

    void onNotify(BLECharacteristic* pCharacteristic) override
    {
      PrintEvent("onNotify", pCharacteristic->getValue().c_str());
    }

    void onStatus(BLECharacteristic* pCharacteristic, Status status, uint32_t code) override
    {
      std::string event("onStatus:");
      switch (status)
      {
        case SUCCESS_INDICATE: event += "SUCCESS_INDICATE"; break;
        case SUCCESS_NOTIFY: event += "SUCCESS_NOTIFY"; break;
        case ERROR_INDICATE_DISABLED: event += "ERROR_INDICATE_DISABLED"; break;
        case ERROR_NOTIFY_DISABLED: event += "ERROR_NOTIFY_DISABLED"; break;
        case ERROR_GATT: event += "ERROR_GATT"; break;
        case ERROR_NO_CLIENT: event += "ERROR_NO_CLIENT"; break;
        case ERROR_INDICATE_TIMEOUT: event += "ERROR_INDICATE_TIMEOUT"; break;
        case ERROR_INDICATE_FAILURE: event += "ERROR_INDICATE_FAILURE"; break;
      }
      event += ":";
      event += String(code).c_str();
      PrintEvent(event.c_str(), nullptr);
    }

  private:
    std::string m_name;
};


void ble_task(void *pvParameters) {
  const TickType_t xDelay1s = pdMS_TO_TICKS (1000);
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  //BLE Peripheral
  BLEDevice::init("ESP32-JSA");
  g_pServer = BLEDevice::createServer();
  g_pServer->setCallbacks(new MyServerCallbacks());    //onConnect, onDisconnect
  BLEService* pService = g_pServer->createService(SERVICE_UUID);

  // characteristic for read
  {
    uint32_t propertyFlags = BLECharacteristic::PROPERTY_READ;
    BLECharacteristic* pCharRead = pService->createCharacteristic(CHAR_READ_UUID, propertyFlags);
    pCharRead->setCallbacks(new MyCharPrintingCallbacks("CharRead"));
    pCharRead->setValue("20");
    g_pCharRead = pCharRead;
  }

  pService->start();
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  // this fixes iPhone connection issue (don't know how it works)
  {
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
  }
  BLEDevice::startAdvertising();

  Serial.println("BLE Peripheral setup done, advertising");
  Serial.println("");

  while (1) {
    value = rand() % 100 + 1;
    snprintf(temperature, sizeof(temperature), "%d", value);
    g_pCharRead->setValue(temperature);
    vTaskDelayUntil(&xLastWakeTime, xDelay1s);
  }
}


void setup() {
  Serial.begin(115200);
  xTaskCreate(ble_task, "ble_task", 5000, NULL, 2, NULL);

}


void loop() {
}
