



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
BLELongCharacteristic analogChar("19B12A59-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//Waveform is scaled for a 12-bit D/A converter (0 .. 4096)

const int y_data[] = {
939, 940, 941, 942, 944, 945, 946, 947, 951, 956, 
962, 967, 973, 978, 983, 989, 994, 1000, 1005, 1015, 
1024, 1034, 1043, 1053, 1062, 1075, 1087, 1100, 1112, 1121, 
1126, 1131, 1136, 1141, 1146, 1151, 1156, 1164, 1172, 1179, 
1187, 1194, 1202, 1209, 1216, 1222, 1229, 1235, 1241, 1248, 
1254, 1260, 1264, 1268, 1271, 1275, 1279, 1283, 1287, 1286, 
1284, 1281, 1279, 1276, 1274, 1271, 1268, 1266, 1263, 1261, 
1258, 1256, 1253, 1251, 1246, 1242, 1237, 1232, 1227, 1222, 
1218, 1215, 1211, 1207, 1203, 1199, 1195, 1191, 1184, 1178, 
1171, 1165, 1159, 1152, 1146, 1141, 1136, 1130, 1125, 1120, 
1115, 1110, 1103, 1096, 1088, 1080, 1073, 1065, 1057, 1049, 
1040, 1030, 1021, 1012, 1004, 995, 987, 982, 978, 974, 
970, 966, 963, 959, 955, 952, 949, 945, 942, 939, 
938, 939, 940, 941, 943, 944, 945, 946, 946, 946, 
946, 946, 946, 946, 946, 947, 950, 952, 954, 956, 
958, 960, 962, 964, 965, 965, 965, 965, 965, 965, 
963, 960, 957, 954, 951, 947, 944, 941, 938, 932, 
926, 920, 913, 907, 901, 894, 885, 865, 820, 733, 
606, 555, 507, 632, 697, 752, 807, 896, 977, 1023, 
1069, 1127, 1237, 1347, 1457, 2085, 2246, 2474, 2549, 2595, 
2641, 2695, 3083, 3135, 3187, 3217, 3315, 3403, 3492, 3581, 
3804, 3847, 3890, 3798, 3443, 3453, 3297, 3053, 2819, 2810, 
2225, 2258, 1892, 1734, 1625, 998, 903, 355, 376, 203, 
30, 33, 61, 90, 119, 160, 238, 275, 292, 309, 
325, 343, 371, 399, 429, 484, 542, 602, 652, 703, 
758, 802, 838, 856, 875, 895, 917, 938, 967, 1016, 
1035, 1041, 1047, 1054, 1060, 1066, 1066, 1064, 1061, 1058, 
1056, 1053, 1051, 1048, 1046, 1043, 1041, 1038, 1035, 1033, 
1030, 1028, 1025, 1022, 1019, 1017, 1014, 1011, 1008, 1006, 
1003, 1001, 999, 998, 996, 994, 993, 991, 990, 988, 
986, 985, 983, 981, 978, 976, 973, 971, 968, 966, 
963, 963, 963, 963, 963, 963, 963, 963, 963, 963, 
963, 963, 963, 963, 963, 963, 963, 963, 963, 963, 
964, 965, 966, 967, 968, 969, 970, 971, 972, 974, 
976, 978, 980, 983, 985, 987, 989, 991, 993, 995, 
997, 999, 1002, 1006, 1011, 1015, 1019, 1023, 1028, 1032, 
1036, 1040, 1045, 1050, 1055, 1059, 1064, 1069, 1076, 1082, 
1088, 1095, 1101, 1107, 1114, 1120, 1126, 1132, 1141, 1149, 
1158, 1166, 1173, 1178, 1183, 1188, 1193, 1198, 1203, 1208, 
1214, 1221, 1227, 1233, 1240, 1246, 1250, 1254, 1259, 1263, 
1269, 1278, 1286, 1294, 1303, 1309, 1315, 1322, 1328, 1334, 
1341, 1343, 1345, 1347, 1349, 1351, 1353, 1355, 1357, 1359, 
1359, 1359, 1359, 1359, 1358, 1356, 1354, 1352, 1350, 1347, 
1345, 1343, 1341, 1339, 1336, 1334, 1332, 1329, 1327, 1324, 
1322, 1320, 1317, 1315, 1312, 1307, 1301, 1294, 1288, 1281, 
1275, 1270, 1265, 1260, 1256, 1251, 1246, 1240, 1233, 1227, 
1221, 1214, 1208, 1201, 1194, 1186, 1178, 1170, 1162, 1154, 
1148, 1144, 1140, 1136, 1131, 1127, 1123, 1118, 1114, 1107, 
1099, 1090, 1082, 1074, 1069, 1064, 1058, 1053, 1048, 1043, 
1038, 1034, 1029, 1025, 1021, 1017, 1013, 1009, 1005, 1001, 
997, 994, 990, 991, 992, 994, 996, 997, 999, 998, 
997, 996, 995, 994, 993, 991, 990, 989, 989, 989, 
989, 989, 989, 989, 988, 986, 984, 983, 981, 980, 
982, 984, 986, 988, 990, 993, 995, 997, 999, 1002, 
1005, 1008, 1012};



const int maxBytes = 512;

int whitePin = 15;
int blackPin = 16;
int redPin = 17;

int outputPin = 14;
 
long previousMillis = 0; 
long blinkMillis = 0;
int ledVal = LOW;

void setup() {
  Serial.begin(9600);    // initialize serial communication
  while (!Serial);
  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected
  pinMode(whitePin, INPUT);
  pinMode(blackPin, INPUT);
  pinMode(redPin, INPUT);
  pinMode(outputPin, OUTPUT);
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
  BLE.setLocalName("Vitals 2 Go");
  BLE.setAdvertisedService(patientService); 
  patientService.addCharacteristic(analogChar); 
  BLE.addService(patientService); 


  /* Start advertising BLE.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */
   Serial.println("Advertising");

  BLE.advertise();
}

void loop() {
  // wait for a BLE central
  BLEDevice central = BLE.central();
  Serial.println("searching for Central");
  if (central) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("connected");
    while (central.connected()) {
      Serial.println("Staying connected");
      updateAnalog();
    }
    Serial.println("disconnected");
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
  }
  //blink while not connected
  long currentMillis = millis();
  if (currentMillis - blinkMillis >= 500) {
    blinkMillis = currentMillis;
    switchLED();
  }
}

void switchLED() {
  digitalWrite(LED_BUILTIN, ledVal);
  ledVal = !ledVal;
}

int scaleVal(int val) {
  return map(val, 0, 4096, 0, 255);
}

void updateAnalog() {
    
//  for (int j = 0; j < 457; j++) {
//    Serial.println(938);
//    analogChar.writeValue((int)938);
//    delay(200);
//  }
  // should be the other 543 data points
//  for (byte i = 0; i < sizeof(y_data) - 1; i++) {
//    analogChar.writeValue((int)y_data[i]);
//    Serial.println(y_data[i]);
//    delay(200);
//  }

//  // Read the current voltage level on the A2 analog input pin
//  
  int whiteVal = map(analogRead(whitePin), 0, 1023, 0, 255);
  int blackVal = map(analogRead(blackPin), 0, 1023, 0, 255);
  int redVal = map(analogRead(redPin), 0, 1023, 0, 255);
  
//  whiteVal = 0x01;
//  blackVal = 0x35;
//  redVal = 0xFF;
//  int analogOut = map(analogIn, 0, 1023, 0, 255);

  int combined = (whiteVal << 16) | (blackVal << 8) | redVal;

//   analogChar.writeValue((long)y_data[3]); 
  analogChar.writeValue((long)combined); 
//  Serial.println(testSignal);
//  analogWrite(outputPin, analogOut);
//  analogChar.writeValue((byte)analogOut); 
  
//  Serial.print("Analog in: "); // print it
//  Serial.println(analogIn);
//  Serial.print("Analog out: "); // print it
//  Serial.println((byte)analogOut);
  
//    byte tempval = 0xa4;
//    pulseOxChar.writeValue(analogOut);  // and update the battery level characteristics
    
}
