



/*

  This example creates a BLE peripheral and
  characteristic. The A2 pin is used to detect the analog signal.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic BLE central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

BLEService patientService("19B1181C-E8F2-537E-4F6C-D104768A1214");


// BLE Characteristics
BLEByteCharacteristic analogChar("19B12A59-E8F2-537E-4F6C-D104768A1214", BLERead);

const int maxBytes = 512;
int sensorPin = 16;
int outputPin = 14;
int oldBatteryLevel = 0; 
long previousMillis = 0; 
int ledVal = LOW;

void setup() {
//  Serial.begin(9600);    // initialize serial communication
//  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected
  pinMode(sensorPin, INPUT);
  // begin initialization
  if (!BLE.begin()) {
//    Serial.println("starting BLE failed!");

    while (1);
  }

  /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setLocalName("Vitals 2 Go");
  BLE.setAdvertisedService(patientService); 
  patientService.addCharacteristic(analogChar); 
  BLE.addService(patientService); 


  /* Start advertising BLE.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */

  BLE.advertise();
}

void loop() {
  // wait for a BLE central
  BLEDevice central = BLE.central();

  if (central) {
    digitalWrite(LED_BUILTIN, HIGH);
    while (central.connected()) {
      long currentMillis = millis();
      if (currentMillis - previousMillis >= 10) {
        previousMillis = currentMillis;
        updateAnalog();
      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
  }
  //blink while not connected
  long currentMillis = millis();
  if (currentMillis - previousMillis >= 200) {
    previousMillis = currentMillis;
    switchLED();
  }
}

void switchLED() {
  digitalWrite(LED_BUILTIN, ledVal);
  ledVal = !ledVal;
}

void updateAnalog() {
  // Read the current voltage level on the A2 analog input pin
  
  int analogIn = analogRead(sensorPin);
  int analogOut = map(analogIn, 0, 1023, 0, 255);
  analogWrite(outputPin, analogOut);
  analogChar.writeValue((byte)analogOut); 
  
//  Serial.print("Analog in: "); // print it
//  Serial.println(analogIn);
//  Serial.print("Analog out: "); // print it
//  Serial.println((byte)analogOut);
  
//    byte tempval = 0xa4;
//    pulseOxChar.writeValue(analogOut);  // and update the battery level characteristics
    
}













///*
//  LED
//
//  This example creates a BLE peripheral with service that contains a
//  characteristic to control an LED.
//
//  The circuit:
//  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
//    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
//
//  You can use a generic BLE central app, like LightBlue (iOS and Android) or
//  nRF Connect (Android), to interact with the services and characteristics
//  created in this sketch.
//
//  This example code is in the public domain.
//*/
//
//#include <ArduinoBLE.h>
//
//BLEService ledService("19B1181C-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service
//
//// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
//BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
//
//const int ledPin = LED_BUILTIN; // pin to use for the LED
//
//void setup() {
//  Serial.begin(9600);
//  while (!Serial);
//
//  // set LED pin to output mode
//  pinMode(ledPin, OUTPUT);
//
//  // begin initialization
//  if (!BLE.begin()) {
//    Serial.println("starting BLE failed!");
//
//    while (1);
//  }
//
//  // set advertised local name and service UUID:
//  BLE.setLocalName("Vitals 2 Go");
//  BLE.setAdvertisedService(ledService);
//
//  // add the characteristic to the service
//  ledService.addCharacteristic(switchCharacteristic);
//
//  // add service
//  BLE.addService(ledService);
//
//  // set the initial value for the characeristic:
//  switchCharacteristic.writeValue(0);
//
//  // start advertising
//  BLE.advertise();
//
//  Serial.println("BLE LED Peripheral");
//}
//
//void loop() {
//  // listen for BLE peripherals to connect:
//  BLEDevice central = BLE.central();
//
//  // if a central is connected to peripheral:
//  if (central) {
//    Serial.print("Connected to central: ");
//    // print the central's MAC address:
//    Serial.println(central.address());
//
//    // while the central is still connected to peripheral:
//    while (central.connected()) {
//      // if the remote device wrote to the characteristic,
//      // use the value to control the LED:
//      if (switchCharacteristic.written()) {
//        if (switchCharacteristic.value()) {   // any value other than 0
//          Serial.println("LED on");
//          digitalWrite(ledPin, HIGH);         // will turn the LED on
//        } else {                              // a 0 value
//          Serial.println(F("LED off"));
//          digitalWrite(ledPin, LOW);          // will turn the LED off
//        }
//      }
//    }
//
//    // when the central disconnects, print it out:
//    Serial.print(F("Disconnected from central: "));
//    Serial.println(central.address());
//  }
//}
