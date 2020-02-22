/* Read Raw sensor data from Murata SCL3300 Inclinometer
 * Version 2.0.0 - February 22, 2020
 * Example4_RawDataReading
*/

#include <SPI.h>
#include <SCL3300.h>

SCL3300 inclinometer;
//Default SPI chip/slave select pin is D10

// Need the following define for SAMD processors
#if defined (ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

void setup() {
  Serial.begin(9600);
  delay(2000); //SAMD boards may need a long time to init SerialUSB
  Serial.println("Reading Raw register values from SCL3300 Inclinometer");

  if (inclinometer.begin() == false) {
    Serial.println("Murata SCL3300 inclinometer not connected.");
    while(1); //Freeze
  }
}

void loop() {
  if (inclinometer.isConnected()) Serial.println("Inclinometer is still alive...");
  
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
    Serial.print(inclinometer.getSerialNumber(), HEX);
    Serial.print("\t");
    Serial.print("SL3300 Status Summary Register: ");
    Serial.println(inclinometer.sclData.StatusSum, HEX);
    delay(1000);
  }
}
