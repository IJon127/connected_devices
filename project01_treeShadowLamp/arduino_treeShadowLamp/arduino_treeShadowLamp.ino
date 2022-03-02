/**************************************************************************
  Accelerometer data via bluetooth LE
  Uses Arduino LSM6DS3 library example (by Riccardo Rizzo): https://www.arduino.cc/en/Reference/ArduinoLSM6DS3
  and Tom Igoe's AS7341_Spectrometer_BLE: https://github.com/tigoe/LightProjects/blob/35061270d781354512571693dad21837ac4fc612/spectrometers/AS7341/AS7341_Spectrometer_BLE/AS7341_Spectrometer_BLE.ino
  created 20 Feb 2022
  modified 21 Feb 2022
  by I-Jon Hsieh
 **************************************************************************/

#include <Arduino_LSM6DS3.h>
#include <ArduinoBLE.h>

/*
  generate uuid for service in terminal by uuidgen
  replace 0000 with higher values (0001 ...)
*/
char serviceUuid[] = "3922BB2E-0000-4C31-B690-5EE7074ABC1F";
char characteristicUuidAccelerometer[] = "3922BB2E-0001-4C31-B690-5EE7074ABC1F";
char characteristicUuidGyroscope[] = "3922BB2E-0002-4C31-B690-5EE7074ABC1F";

//name the device
char bleName[] = "treeShadowLamp";
const int readingLengthAccelerometer = 17; //-0.00,-0.00,-0.00
const int readingLengthGyroscope = 23; //-180.00,-180.00,-180.00
String axyz; //acceleroemeter data in string
String gxyz; //gyroscope data in string

int breathing = 0;
int breathingSpeed = 1;


/*
  BLERead: ask the peripheral to send back the current value of the characteristic. Often used for characteristics that don't change very often, for example characteristics used for configuration, version numbers, etc.
  BLEWrite: modify the value of the characteristic. Often used for things that are like commands, for example telling the peripheral to turn a motor on or off.
  BLEIndicate and Notify: ask the peripheral to continuously send updated values of the characteristic, without the central having to constantly ask for it.
*/
BLEService bleService(serviceUuid);
BLECharacteristic acceleroCharacteristic(characteristicUuidAccelerometer, BLERead | BLENotify,  readingLengthAccelerometer);
BLECharacteristic gyroCharacteristic(characteristicUuidGyroscope, BLERead | BLENotify,  readingLengthGyroscope);

void setup() {
  Serial.begin(9600);
  //  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("IMU failed");
    while (1);
  }
  if (!BLE.begin()) {
    Serial.println("BLE failed");
    while (1);
  }

  axyz = "0.00,0.00,0.00";
  gxyz = "0.00,0.00,0.00";


  BLE.setLocalName(bleName);
  BLE.setAdvertisedService(bleService);
  bleService.addCharacteristic(acceleroCharacteristic);
  bleService.addCharacteristic(gyroCharacteristic);
  BLE.addService(bleService);
  acceleroCharacteristic.writeValue(axyz.c_str());
  gyroCharacteristic.writeValue(gxyz.c_str());
  BLE.advertise();

  Serial.println("Bluetooth device ready, waiting for connections!");
}

void loop() {

  float ax, ay, az; //accelerometer data
  float gx, gy, gz; //gyroscope data

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
  }

  //Gyriscope control LEDs
  float absGx = abs(gx) * 100;
  float absGy = abs(gy) * 100;
  float absGz = abs(gz) * 100;


  breathing += breathingSpeed;
  if (breathing >= 255 | breathing <= 0) {
    breathingSpeed = breathingSpeed * -1;
  }

  int ledX = breathing;
  int ledY = breathing;
  int ledZ = breathing;


  Serial.print("Accelerometer>> ");
  Serial.print("x: ");
  Serial.print(ax);
  Serial.print(",  y: ");
  Serial.print(ay);
  Serial.print(",  z: ");
  Serial.println(az);
  Serial.print("Gyroscope>> ");
  Serial.print("x: ");
  Serial.print(gx);
  Serial.print(",  y: ");
  Serial.print(gy);
  Serial.print(",  z: ");
  Serial.println(gz);

  //connect to ble===================
  BLEDevice central = BLE.central();
  if (central) {
    if (central.connected()) {

      //Accelerometer data---------
      String sAx = (String) ax;
      String sAy = (String) ay;
      String sAz = (String) az;
      axyz = sAx + "," + sAy + "," + sAz;

      //Gyriscope data---------
      String sGx = (String) gx;
      String sGy = (String) gy;
      String sGz = (String) gz;
      gxyz = sGx + "," + sGy + "," + sGz;

      acceleroCharacteristic.writeValue(axyz.c_str());
      gyroCharacteristic.writeValue(gxyz.c_str());

      ledX = map(absGx, 0, 3000, 5, 255);
      ledY = map(absGy, 0, 3000, 5, 255);
      ledZ = map(absGz, 0, 3000, 5, 255);
    }
  }

  analogWrite(2, ledX);
  analogWrite(3, ledY);
  analogWrite(4, ledZ);



}
