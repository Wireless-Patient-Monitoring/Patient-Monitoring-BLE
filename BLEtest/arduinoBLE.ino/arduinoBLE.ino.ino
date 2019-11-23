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

BLEService patientService("181C");


// BLE Battery Level Characteristic
BLEUnsignedCharCharacteristic pulseOxChar("2A59", BLERead | BLENotify);
BLEUnsignedCharCharacteristic bloodPressureChar("2A35", BLERead | BLENotify);
BLEUnsignedCharCharacteristic heartRateChar("2A37", BLERead | BLENotify);

int sensorPin = 16;
int oldBatteryLevel = 0;  // last battery level reading from analog input
long previousMillis = 0;  // last time the battery level was checked, in ms

void setup() {
  Serial.begin(9600);    // initialize serial communication
//  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected
  pinMode(sensorPin, INPUT);
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setLocalName("Patient Monitor");
  BLE.setAdvertisedService(patientService); 
  patientService.addCharacteristic(pulseOxChar); 
  patientService.addCharacteristic(bloodPressureChar);
  patientService.addCharacteristic(heartRateChar);
  BLE.addService(patientService); 
  pulseOxChar.writeValue(0); 
  bloodPressureChar.writeValue(1); 
  heartRateChar.writeValue(2); 

  /* Start advertising BLE.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    // check the battery level every 200ms
    // while the central is connected:
    while (central.connected()) {
      long currentMillis = millis();
      // if 200ms have passed, check the battery level:
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        updateAnalog();
      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void updateAnalog() {
  /* Read the current voltage level on the A0 analog input pin.
     This is used here to simulate the charge level of a battery.
  */
  int analogIn = analogRead(sensorPin);
  int analogOut = map(analogIn, 0, 1023, 0, 255);
  analogWrite(sensorPin-2, analogOut);
//
//  if (batteryLevel != oldBatteryLevel) {      // if the battery level has changed
    Serial.print("Analog in: "); // print it
    Serial.println(analogIn);
    
//    Serial.println(batteryLevel);
    pulseOxChar.writeValue(analogIn);  // and update the battery level characteristics
//
//    oldBatteryLevel = batteryLevel;           // save the level for next comparison
//  }
}
