/* Read Temperature Sensor from Murata SCL3300 Inclinometer
 * Version 3.0.0 - August 8, 2020
 * Example4_BasicTemperatureReading
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
  Serial.println("Reading basic Temperature values from SCL3300 Inclinometer");

  if (inclinometer.begin() == false) {
    Serial.println("Murata SCL3300 inclinometer not connected.");
    while(1); //Freeze
  }
}

void loop() {
  if (inclinometer.available()) { //Get next block of data from sensor
    Serial.print("Celsius Temperature: ");
    Serial.print(inclinometer.getCalculatedTemperatureCelsius());
    Serial.print("\t");
    Serial.print("Farenheit Temperature: ");
    Serial.println(inclinometer.getCalculatedTemperatureFarenheit());
    delay(250); //Allow a little time to see the output
  } else inclinometer.reset();
}
