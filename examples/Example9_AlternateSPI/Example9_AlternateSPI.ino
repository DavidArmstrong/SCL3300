/* Alternate SPI interface used to Read Tilt angles from Murata SCL3300 Inclinometer
 * Version 3.3.0 - September 13, 2021
 * Example9_AlternateSPI
*/

#include <SPI.h>
#include <SCL3300.h>

SCL3300 inclinometer;

// Need the following define for SAMD processors
#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

void setup() {
  Serial.begin(9600);
  delay(2000); //SAMD boards may need a long time to init SerialUSB
  Serial.println("Define a specific SPI interface for the SCL3300 Inclinometer");

  // Change the begin() parameters here, as needed, to reference the SPI
  // interface being used for the SCL3300 inclinometer.
  // The default Arduino interface is "SPI", and is shown here
  if (inclinometer.begin(SPI, 10) == false) {
    Serial.println("Murata SCL3300 inclinometer not connected.");
    while(1); //Freeze
  } else
    Serial.println("Murata SCL3300 inclinometer is connected.");
}

void loop() {
  if (inclinometer.isConnected()) {
    Serial.println("Inclinometer is still alive...");
  } else {
    Serial.println("Inclinometer error detected...");
    delay(1000);
  }
  if (inclinometer.available()) { //Get next block of data from sensor
    Serial.print("Raw X Tilt: ");
    Serial.print(inclinometer.sclData.AngX);
    Serial.print("\t");
    Serial.print("Y Tilt: ");
    Serial.print(inclinometer.sclData.AngY);
    Serial.print("\t");
    Serial.print("Z Tilt: ");
    Serial.println(inclinometer.sclData.AngZ);
    Serial.print("Raw X Accel: ");
    Serial.print(inclinometer.sclData.AccX);
    Serial.print("\t");
    Serial.print("Y Accel: ");
    Serial.print(inclinometer.sclData.AccY);
    Serial.print("\t");
    Serial.print("Z Accel: ");
    Serial.println(inclinometer.sclData.AccZ);
    Serial.print("SCL3300 STO register: ");
    Serial.print((inclinometer.sclData.STO >> 8) & 0xff, HEX);
    Serial.print("\t");
    Serial.print("WHOAMI register: ");
    Serial.println(inclinometer.sclData.WHOAMI, HEX);
    Serial.print("Raw Temperature: ");
    Serial.println(inclinometer.sclData.TEMP);
    Serial.print("Serial Number Register: ");
    Serial.print(inclinometer.getSerialNumber());
    Serial.print("B33");
    Serial.print("\t");
    Serial.print("SL3300 Status Summary Register: ");
    Serial.println(inclinometer.sclData.StatusSum, HEX);
    delay(1000);
  } else inclinometer.reset();
}
