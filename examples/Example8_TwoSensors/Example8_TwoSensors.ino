/* Read Tilt angles from Two Murata SCL3300 Inclinometers
 * Version 3.2.0 - September 3, 2021
 * Example8_TwoSensors
*/

#include <SPI.h>
#include <SCL3300.h>

SCL3300 inclinometerA, inclinometerB;
//First SPI chip (A) select pin is D10
//Second SPI chip (B) select pin is D9

// Need the following define for SAMD processors
#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

void setup() {
  Serial.begin(9600);
  delay(2000); //SAMD boards may need a long time to init SerialUSB
  Serial.println("Reading basic Tilt Level Offset values from two SCL3300 Inclinometers");

  if (inclinometerA.begin(10) == false) {
    Serial.println("Murata SCL3300 inclinometer A not connected.");
    while(1); //Freeze
  }
    if (inclinometerB.begin(9) == false) {
    Serial.println("Murata SCL3300 inclinometer B not connected.");
    while(1); //Freeze
  }
}

void loop() {
  if (inclinometerA.available()) { //Get next block of data from sensor
    Serial.print("Dev A: X Tilt: ");
    Serial.print(inclinometerA.getTiltLevelOffsetAngleX());
    Serial.print("\t");
    Serial.print("Y Tilt: ");
    Serial.print(inclinometerA.getTiltLevelOffsetAngleY());
    Serial.print("\t");
    Serial.print("Z Tilt: ");
    Serial.println(inclinometerA.getTiltLevelOffsetAngleZ());
  } else inclinometerA.reset();
  
  if (inclinometerB.available()) { //Get next block of data from sensor
    Serial.print("Dev B: X Tilt: ");
    Serial.print(inclinometerB.getTiltLevelOffsetAngleX());
    Serial.print("\t");
    Serial.print("Y Tilt: ");
    Serial.print(inclinometerB.getTiltLevelOffsetAngleY());
    Serial.print("\t");
    Serial.print("Z Tilt: ");
    Serial.println(inclinometerB.getTiltLevelOffsetAngleZ());
  } else inclinometerB.reset();
  delay(100); //Allow a little time to see the output
}
