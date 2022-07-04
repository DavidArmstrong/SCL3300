/* Read Tilt angles from Murata SCL3300 Inclinometer using Mode 1
 * Version 3.3.1 - July 4, 2022
 * Example10_Mode1
 *
 * Modes 3 and 4 (default for library) are designed to be used when the inclinometer
 * will be kept within +-10 degrees of level.
 * However, if the inclinometer might be used outside of that range, then
 * it is required that either Mode 1 or 2 be used instead.
 * This sketch demonstrates how to use Mode 1 when that is needed.
*/

#include <SPI.h>
#include <SCL3300.h>

SCL3300 inclinometer;
//Default SPI chip/slave select pin is D10

// Need the following define for SAMD processors
#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

void setup() {
  Serial.begin(9600);
  delay(2000); //SAMD boards may need a long time to init SerialUSB
  Serial.println("Reading basic Tilt Level Offset values from SCL3300 Inclinometer");
  Serial.println("using Mode 1.");

  if (inclinometer.begin() == false) {
    Serial.println("Murata SCL3300 inclinometer not connected.");
    while(1); //Freeze
  } else {
    Serial.println("Murata SCL3300 inclinometer detected.");
    // Put this inclinometer into Mode 1, since we are using it to measure 0-90 Degree angles
    inclinometer.setMode(1);
    if (inclinometer.begin()) {
      Serial.println("Murata SCL3300 inclinometer now in Mode 1.");
	} else {
      Serial.println("Tube Murata SCL3300 inclinometer failed to transition to Mode 1.");
	  while(1); //Freeze
	}
  }
}

void loop() {
  if (inclinometer.available()) { //Get next block of data from sensor
    Serial.print("X Tilt: ");
    Serial.print(inclinometer.getTiltLevelOffsetAngleX());
    Serial.print("\t");
    Serial.print("Y Tilt: ");
    Serial.print(inclinometer.getTiltLevelOffsetAngleY());
    Serial.print("\t");
    Serial.print("Z Tilt: ");
    Serial.println(inclinometer.getTiltLevelOffsetAngleZ());
    delay(100); //Allow a little time to see the output
  } else inclinometer.reset();
}
