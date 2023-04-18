////part one.

#include <Arduino.h>
#include <SparkFunLSM6DSO.h>
#include <BLEDevice.h>

int sdapin = GPIO_NUM_21;
int sclpin = GPIO_NUM_22;
int led = GPIO_NUM_25;
 
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
 
class MyCallbacks: public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *pCharacteristic) {
     std::string value = pCharacteristic->getValue();
      if(value == "on"){
        digitalWrite(led, HIGH);
      }else if(value == "off"){
        digitalWrite(led, LOW);
      } 
 
     if (value.length() > 0) {
       Serial.println("*********");
       Serial.print("New value: ");
       for (int i = 0; i < value.length(); i++)
         Serial.print(value[i]);
 
       Serial.println();
       Serial.println("*********");
     }
   }
};
 
void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
 
  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");
  
  BLEDevice::init("SDSUCS");
  BLEServer *pServer = BLEDevice::createServer();
  
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                          CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                        );
  
  pCharacteristic->setCallbacks(new MyCallbacks());
  
  pCharacteristic->setValue("Hello World");
  pService->start();
  
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}
 
void loop() {

  delay(2000);
}