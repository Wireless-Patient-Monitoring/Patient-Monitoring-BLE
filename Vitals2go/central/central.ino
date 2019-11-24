/*
  LED Control

  This example scans for BLE peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
  it will remotely control the BLE Peripheral's LED, when the button is pressed or released.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - Button with pull-up resistor connected to pin 2.

  You can use it with another board that is compatible with this library and the
  Peripherals -> LED example.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

// variables for button
const int buttonPin = 2;
int buttonState = LOW;
long previousMillis = 0; 
int ledVal = LOW;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT);

  // initialize the BLE hardware
  BLE.begin();

  Serial.println("BLE Central - LED control");

  // start scanning for peripherals
  BLE.scanForUuid("19b1181C-e8f2-537e-4f6c-d104768a1214");
}

void switchLED() {
  digitalWrite(LED_BUILTIN, ledVal);
  ledVal = !ledVal;
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "Vitals 2 Go") {
      Serial.println("Couldn't find Service");
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("19b1181C-e8f2-537e-4f6c-d104768a1214");
  }
//  long currentMillis = millis();
//  if (currentMillis - previousMillis >= 200) {
//    previousMillis = currentMillis;
//    switchLED();
//  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");
  
  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic pulseOxChar = peripheral.characteristic("19B12A59-E8F2-537E-4F6C-D104768A1214");
//  BLECharacteristic bloodPressureChar = peripheral.characteristic("2A35");
//  BLECharacteristic heartRateChar = peripheral.characteristic("2A37");


  if (!pulseOxChar) {
    Serial.println("Peripheral does not have the characteristics!");
    peripheral.disconnect();
    return;
  } else if (!pulseOxChar.canRead()) {
    Serial.println("Peripheral cannot be read!");
    peripheral.disconnect();
    return;
  }
  
  digitalWrite(LED_BUILTIN, HIGH);
  while (peripheral.connected()) {
    
    // while the peripheral is connected
    delay(200);
//    byte newVal;
//    pulseOxChar.readValue(newVal);
//    buttonState = !buttonState;
    Serial.print("Val = ");
    Serial.println("123");
    
  }
  
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Peripheral disconnected");
}
