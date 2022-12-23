/*
  Dave Williams, DitroniX 2019-2022 (ditronix.net)
  RPEM Remote Power Energy Monitor v1.0
  Features include ESP8266 Four CT Clamp Inputs AC DC Voltage Detectors NTC I2C OLED Opto-In GPIO EEPROM MOSFET RS-485 DEEP SLEEP
  PCA v1.00 - Test Code Firmware v1 - 3rd December 2022
  
  Simplified Board Bring Up Test - Wifi Scanner (Displays SSID and Flashes Status LED on Loop)
   
  Remember!
  - Set the BOARD to ESP8266 DEV Module (Any Wemos D1 or similar).
  - You can also set the BAUD rate up to 921600 to speed up flashing.
  - The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
  
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and github.com/DitroniX
*/

#include <ESP8266WiFi.h>

// Constants
const int LoopDelay = 1; // Loop Delay in Seconds

// **************** OUTPUTS ****************
#define LED_Blue 2 // Blue LED

WiFiClient client; // Initialize the client library

void setup() {

  // Stabalise
  delay(250);

  // Initialize UART:
  Serial.begin(115200, SERIAL_8N1);  //115200
  while (!Serial);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  //  Configures the specified LED GPIO as outputs
  pinMode(LED_Blue, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Blue, HIGH);
}

void loop() {

  Serial.println("ESP8266 WiFi MAC \t " + WiFi.macAddress());
  Serial.println("Scanning for Access Points or Routers.  Please wait....");
  delay(100);

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.print("Scan Complete.  ");
  if (n == 0) {
    Serial.println("No Access Points or Routers Found.");
  } else {
    Serial.print(n);
    Serial.println(" Access Points or Routers Found.");
    Serial.println("");

    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": \t");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.println(")");
      delay(100);
    }
  }
  Serial.println("");

  // Heatbeat Blue Status LED
  digitalWrite(LED_Blue, LOW);
  delay(100);
  digitalWrite(LED_Blue, HIGH);

  // Loop Delay
  delay(LoopDelay * 1000);
}
