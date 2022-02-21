/**************************************************************************
  Accelerometer data via bluetooth LE
  Uses Arduino LSM6DS3 library example (by Riccardo Rizzo): https://www.arduino.cc/en/Reference/ArduinoLSM6DS3
  and Tom Igoe's AS7341_Spectrometer_BLE: https://github.com/tigoe/LightProjects/blob/35061270d781354512571693dad21837ac4fc612/spectrometers/AS7341/AS7341_Spectrometer_BLE/AS7341_Spectrometer_BLE.ino
  created 20 Feb 2022
  modified 20 Feb 2022
  by I-Jon Hsieh
 **************************************************************************/

#include <Arduino_LSM6DS3.h>
#include <ArduinoBLE.h>

/*
  generate uuid for service in terminal by uuidgen
  replace 0000 with higher values (0001 ...)
*/
char serviceUUID[] = "EB39FE3F-0000-45F1-9034-BB4FED33A133";  //[]????
char characteristicUUID[] = "EB39FE3F-0001-45F1-9034-BB4FED33A133";

//name the device
char bleName[] = "accelerometer";
const int readingLength = 17; //-0.00,-0.00,-0.00
String xyz;

BLEService acceleroService(serviceUUID);
/*
  BLERead: ask the peripheral to send back the current value of the characteristic. Often used for characteristics that don't change very often, for example characteristics used for configuration, version numbers, etc.
  BLEWrite: modify the value of the characteristic. Often used for things that are like commands, for example telling the peripheral to turn a motor on or off.
  BLEIndicate and Notify: ask the peripheral to continuously send updated values of the characteristic, without the central having to constantly ask for it.
*/
BLECharacteristic acceleroCharacteristic(characteristicUUID, BLERead | BLENotify,  readingLength); //Do we really need readingLength?


void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("IMU failed");
    while (1); //Why 1?
  }
  if (!BLE.begin()) {
    Serial.println("BLE failed");
    while (1);
  }

  xyz = "0.00,0.00,0.00";
  
  BLE.setLocalName(bleName);
  BLE.setAdvertisedService(acceleroService);
  acceleroService.addCharacteristic(acceleroCharacteristic);
  BLE.addService(acceleroService);
  acceleroCharacteristic.writeValue(xyz.c_str());
  BLE.advertise();

  Serial.println("Bluetooth device ready, waiting for connections!");
}

void loop() {
  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
  }

  String Sx = (String) x;
  String Sy = (String) y;
  String Sz = (String) z;
  xyz = Sx + "," + Sy + "," + Sz;

  
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(",  y: ");
  Serial.print(y);
  Serial.print(",  z: ");
  Serial.println(z);
  

  BLEDevice central = BLE.central(); //Does it have to be named as "central"?
  if (central) {
    while (central.connected()) {
      acceleroCharacteristic.writeValue(xyz.c_str());
    }
  }


}
