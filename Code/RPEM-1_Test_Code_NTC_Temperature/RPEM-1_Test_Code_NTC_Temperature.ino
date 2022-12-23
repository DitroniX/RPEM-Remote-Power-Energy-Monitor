/*
  Dave Williams, DitroniX 2019-2022 (ditronix.net)
  RPEM Remote Power Energy Monitor v1.0
  Features include ESP8266 Four CT Clamp Inputs AC DC Voltage Detectors NTC I2C OLED Opto-In GPIO EEPROM MOSFET RS-485 DEEP SLEEP
  PCA v1.00 - Test Code Firmware v1 - 3rd December 2022
  .
  Simplified Board Bring Up Test - Read NTC and display Temperature
  .
  Remember!
  - Set the BOARD to ESP8266 DEV Module (Any Wemos D1 or similar).
  - You can also set the BAUD rate up to 921600 to speed up flashing.
  - The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  .
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
  .
  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and github.com/DitroniX
*/

// Libraries
#include "Wire.h"
#include "i2c_adc_ads7828.h"  // https://github.com/4-20ma/i2c_adc_ads7828 (ADC ADS7828 - Doc Walker)

// ****************  VARIABLES / DEFINES / STATIC ****************

// ADS7828 Address: A1=0, A0=0 0x48 | Command: SD=1, PD1=1, PD0=1
ADS7828 device(0, SINGLE_ENDED | REFERENCE_ON | ADC_ON, 0xFF);
ADS7828* adc = &device;
ADS7828Channel* NTC = adc->channel(7);

// Constants
const int LoopDelay = 1;  // Loop Delay in Seconds

// NTC. Steinhart-Hart Thermistor Equation Constants:
const double A = 0.001129148;
const double B = 0.000234125;
const double C = 0.0000000876741;
const double R2 = 10000;  // 10k Series Resistor

// **************** IO ****************

// Define I2C (Expansion Port)
#define I2C_SDA 4
#define I2C_SCL 5

// **************** OUTPUTS ****************
#define LED_Blue 2  // Blue LED

void setup() {

  // Stabalise
  delay(250);

  // Initialize UART:
  Serial.begin(115200, SERIAL_8N1);  //115200
  while (!Serial)
    ;

  // Initialise I2C
  Wire.begin(I2C_SDA, I2C_SCL);
}

void loop() {

  // Update all registered ADS7828 devices/unmasked channels
  ADS7828::updateAll();

  double Vout, Rth, temperature, adc_value;
  adc_value = NTC->value();
  Vout = (adc_value * 5.0) / 4095;
  Rth = (5.0 * R2 / Vout) - R2;

  //  Steinhart-Hart Thermistor Equation:
  //  Temperature in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3).  Where A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8
  temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)), 3))));  // Temperature in kelvin
  temperature -= 273.15;                                                // Temperature in degree celsius

  Serial.print("NTC: ");
  Serial.print(temperature);
  Serial.println(" ºC");

  // Heatbeat Blue Status LED
  digitalWrite(LED_Blue, LOW);
  delay(100);
  digitalWrite(LED_Blue, HIGH);

  // Loop Delay
  delay(LoopDelay * 1000);
}
