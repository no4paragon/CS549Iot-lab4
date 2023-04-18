
//please comment out the whole block in order to test part A and B separately.
//Part A code Start

#include <Arduino.h>
#include <SparkFunLSM6DSO.h>
#include <BLEDevice.h>

// Set the I2C communication pins(not used for part A)
//int sdapin = GPIO_NUM_21;
//int sclpin = GPIO_NUM_22;

// Set the LED pin
int led = GPIO_NUM_25;

// Set the UUIDs for the service and characteristic
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

// Create a callback function for when the characteristic is written to
class MyCallbacks: public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *pCharacteristic) {
     std::string value = pCharacteristic->getValue();

     // If the value is "on", turn the LED on, if the values is "off", turn the LED off.
      if(value == "on"){
        digitalWrite(led, HIGH);
      }else if(value == "off"){
        digitalWrite(led, LOW);
      } 

     // Print the new value to the serial monitor. when I type on, it shows on and vice versa.
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

  // Set the LED pin as an output
  pinMode(led, OUTPUT);
 
  // Print instructions for connecting to the device
  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to SDSUCS");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");
  
  // Initialize the BLE device and server
  BLEDevice::init("SDSUCS");
  BLEServer *pServer = BLEDevice::createServer();
  
  // Create a service and characteristic
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                          CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                        );
  
  // Set the callback for the characteristic
  pCharacteristic->setCallbacks(new MyCallbacks());
  
  // Set the initial value for the characteristic
  pCharacteristic->setValue("Hello World");
  pService->start();
  
  // Start advertising the device
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}
 
void loop() {
  delay(2000); // Wait for 2 seconds before repeating the loop
}

//Part A code End



//Part B code Start

#include <Arduino.h>
#include <SparkFunLSM6DSO.h>
#include <BLEDevice.h>
#include <Wire.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;  // Declare a pointer to a BLECharacteristic object
uint32_t stepCount = 0; // Declare a variable to store the step count
LSM6DSO lsm; // Declare an object of the LSM6DSO class to interface with the IMU sensor

float x_offset = 0.0; // Declare a variable to store the x-axis calibration offset
float y_offset = 0.0; // Declare a variable to store the y-axis calibration offset
float x_sum = 0.0; // Declare a variable to store the sum of x-axis readings during calibration
float y_sum = 0.0; // Declare a variable to store the sum of y-axis readings during calibration
int num_samples = 100; // Define the number of samples to use for calibration
float calibratedThreshold = 0.0; // Declare a variable to store the calibrated threshold value


unsigned long previousTime = 0; // Declare a variable to store the previous time at which a step was detected
const unsigned long interval = 300; // Define the interval at which to read the accelerometer (in milliseconds)


void calibrateAccelerometer() {
  // This function takes multiple samples and calculates the average RMS value for each axis, then sets the calibration offsets and calculates the calibrated threshold value.
  for (int i = 0; i < num_samples; i++) {
    float x = lsm.readFloatAccelX(); // Read the x-axis acceleration from the IMU sensor
    float y = lsm.readFloatAccelY(); // Read the y-axis acceleration from the IMU sensor
    float x_squared = x * x; // Square the x-axis reading
    float y_squared = y * y; // Square the y-axis reading
    x_sum += sqrt(x_squared + y_squared); // Add the RMS value of the x-y vector to the x_sum variable
    y_sum += sqrt(x_squared + y_squared); // Add the RMS value of the x-y vector to the y_sum variable
    delay(100); // Wait for a short period between readings to reduce noise
  }

  // Calculate the average RMS values for each axis
  float x_avg = x_sum / num_samples;
  float y_avg = y_sum / num_samples;

  // Set the calibration offsets
  x_offset = x_avg;
  y_offset = y_avg;
  
  // Calculate the calibrated threshold value
  calibratedThreshold = sqrt((x_offset * x_offset) + (y_offset * y_offset));
  Serial.println(calibratedThreshold); // Output the calibrated threshold value to the serial monitor
}

void setup()
{
  Serial.begin(9600); // Initialize the serial communication at a baud rate of 9600
  Wire.begin(SDA, SCL); // Initialize the I2C communication pins
  delay(500);

  if( lsm.begin() )
    Serial.println("Ready.");
  else { 
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }

  if( lsm.initialize(BASIC_SETTINGS) )
    Serial.println("Loaded Settings.");

  // Calibrate the accelerometer to measure step count
  calibrateAccelerometer();
  Serial.println("calibrateAccelerometer finished.");

  // Code given within the lab
  BLEDevice::init("Step Counter");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_NOTIFY
                                      );

  pCharacteristic->setValue("CSIoT");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x0); 
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}
 

void loop() {
  // Get the current time
  unsigned long currentTime = millis();

  // Read the accelerometer values
  float x = lsm.readFloatAccelX();
  float y = lsm.readFloatAccelY();

  // Square the x and y values and calculate the root mean square (RMS) value
  float x_squared = x * x;
  float y_squared = y * y;
  float rms = sqrt(x_squared + y_squared);

  // If the RMS value is above the calibrated threshold and enough time has passed since the last step was detected
  if ( (rms > calibratedThreshold) && (currentTime - previousTime > interval)) {
    // Increase the step count
    stepCount++;

    // Print the step count to the serial monitor
    Serial.print("Step count: ");
    Serial.println(stepCount);

    // Set the value of the BLE characteristic to the current step count and notify any connected devices
    pCharacteristic->setValue(stepCount);
    pCharacteristic->notify();

    // Update the previous time to the current time
    previousTime = currentTime;
  }

  // Delay for a short amount of time to prevent the loop from running too frequently
  delay(20);
}
//Part B code End


