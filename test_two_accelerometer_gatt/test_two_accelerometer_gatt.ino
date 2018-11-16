#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// The two BNO055 modules, bnoB has the ADR pin wired to 3.3v to change its i2c address
// Both are wired: SCL to analog 5, SDA to analog 4, VIN to 5v, GRN to ground
Adafruit_BNO055 bnoA = Adafruit_BNO055(-1, BNO055_ADDRESS_A);
Adafruit_BNO055 bnoB = Adafruit_BNO055(-1, BNO055_ADDRESS_B);


#include <Arduino.h>
#include <SPI.h>

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"


#define IMU_SERVICE_UUID             "00-11-00-11-44-55-66-77-88-99-AA-BB-CC-DD-EE-FF"

#define IMU_1_x_CHARACTERISTIC_UUID   "A1-96-C8-76-DE-8C-4C-47-AB-5A-D7-AF-D5-AE-71-28"
#define IMU_1_y_CHARACTERISTIC_UUID   "A1-96-C8-76-DE-8C-4C-47-AB-5A-D7-AF-D5-AE-71-29"
#define IMU_1_z_CHARACTERISTIC_UUID   "A1-96-C8-76-DE-8C-4C-47-AB-5A-D7-AF-D5-AE-71-30"

#define IMU_2_x_CHARACTERISTIC_UUID   "A1-96-C8-76-DE-8C-4C-47-AB-5A-D7-AF-D5-AE-71-31"
#define IMU_2_y_CHARACTERISTIC_UUID   "A1-96-C8-76-DE-8C-4C-47-AB-5A-D7-AF-D5-AE-71-32"
#define IMU_2_z_CHARACTERISTIC_UUID   "A1-96-C8-76-DE-8C-4C-47-AB-5A-D7-AF-D5-AE-71-33"

#define LOCAL_NAME                 "EDUS"

#define MINIMUM_FIRMWARE_VERSION   "0.7.0"

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

int dataArray[6];

void setup() {
  Serial.begin(115200);
  if (!bnoA.begin()) {
    Serial.print("Ooops, BNO055(A) not detected");
    while (1);
  }
  bnoA.setExtCrystalUse(true);
  if (!bnoB.begin()) {
    Serial.print("Ooops, BNO055(B) not detected");
    while (1);
  }
  bnoB.setExtCrystalUse(true);

  initBluetooth();
  setupGatt();
  advertizeGatt();
  getGATTList();
  /* Set callbacks */
  ble.setConnectCallback(connected);
  ble.setDisconnectCallback(disconnected);

  ble.reset();
  Serial.println();

}

void loop() {

  imu::Vector<3> euler = bnoA.getVector(Adafruit_BNO055::VECTOR_EULER);
  dataArray[0] = euler.x();
  dataArray[1] = euler.y();
  dataArray[2] = euler.z();

  euler = bnoB.getVector(Adafruit_BNO055::VECTOR_EULER);
  dataArray[3] = euler.x();
  dataArray[4] = euler.y();
  dataArray[5] = euler.z();

  updateAccelerometerValues(dataArray[0],dataArray[1],dataArray[2],dataArray[3],dataArray[4],dataArray[5]);
  delay(100);
}
