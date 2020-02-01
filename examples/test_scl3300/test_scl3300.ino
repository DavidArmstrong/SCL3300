/* Arduino Library for Murata SCL3300 Inclinometer
 *  Februrary 2020 - Version 1.0.0
 * See MIT LICENSE.md file and ReadMe.md file for essential information

NOTE: DVIO on SCL3300 MUST be tied to +3.3V

  Hardware connections:
  SCL3300 -> Arduino (UNO)
  GND -> GND
  3.3 -> 3.3
  CS -> 10
  MOSI -> 11
  MISO -> 12
  SCK -> 13

NOTE: Sparkfun Redboard Turbo MUST have pullup resistor on CS/SS
*/

#include <SPI.h>
#include <SCL3300.h>

SCL3300 inclinometer;
uint8_t cspin = 10; //SPI chip/slave select pin is D10
uint16_t whoami;
// Need the following define for the Sparkfun Redboard Turbo
#if defined (ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

void setup() {
  Serial.begin(9600);
  delay(2000); //Redboard Turbo may need a long time to init SerialUSB
  Serial.println("Reading basic values from SCL3300 Inclinometer");

  whoami = inclinometer.begin(cspin);
  if (whoami != 0xc1) {
    Serial.println("Murata SCL3300 not connected.");
    while(1); //Freeze
  } else {
    Serial.println("Murata SCL3300 initialized fine.");
  }
}

void loop() {
  inclinometer.ReadDataBlock();
  Serial.print("X Tilt: ");
  Serial.print(inclinometer.angle(inclinometer.sclData.AngX));
  Serial.print("  Y Tilt: ");
  Serial.print(inclinometer.angle(inclinometer.sclData.AngY));
  Serial.print("  Z Tilt: ");
  Serial.println(inclinometer.angle(inclinometer.sclData.AngZ));
  delay(250);
}
