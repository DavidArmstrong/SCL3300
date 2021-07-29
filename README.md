# SCL3300


Arduino Library for Murata SCL3300 Inclinometer

  Version 3.1.0 - July 28, 2021

  By David Armstrong
  https://github.com/DavidArmstrong/Arduino-SCL3300
  See MIT LICENSE.md file


The Murata SCL3300 inclinometer sensor is a 3.3 volt device used to measure tilt in three axes simultaneously.

  Datasheet: https://www.murata.com/-/media/webrenewal/products/sensor/pdf/datasheet/datasheet_scl3300-d01.ashx?la=en-us

  Evaluation Board: https://www.murata.com/-/media/webrenewal/products/sensor/pdf/specification/pcbspec_scx3300.ashx?la=en-us

Notes:
  1) The SCL3300 inclinometer will require a bidrectional level shifter to interface the SPI pins to 5 volt devices, such as the Arduino Uno.
  2) A pull-up resistor may be required on the Chip/Slave Select line of the SCL3300.  A typical resistor value of 4.7k ohms should connect this pin to +3.3 volts.
  3) Be sure to connect the SCL3300 DVIO pin to +3.3 volts as well.  This pin powers the digital pins.
  4) There is a small, but significant, library change starting with Version 3.0.0.  This requires a one-line addition to any older sketches when using this updated SCL3300 library.  All the example sketches have been updated to include the addtional code.  (The backwards incompatibility is due to the improved error detection and handling design.)

For an Arduino Uno, the default SPI pins are as follows:
  * SCK - Digital Pin 13
  * SDO - Digital Pin 12 (MISO)
  * SDI - Digital Pin 11 (MOSI)
  * CS  - Digital Pin 10 (SS)

For a SAMD-type Arduino, such as the Sparkfun Redboard Turbo or Arduino Zero, the default SPI pins are only available on the ICSP connector:
  * pin 22 (MISO)
  * pin 23 (MOSI)
  * pin 24 (SCK)
  * Chip/Slave Select uses digital Pin 10 as the default.

======================================

Basic SCL3300 Library Functions:

begin()         -- This initializes the library and the SPI chip, and by default assigns the SPI Chip Select Pin to Digital Pin 10.

begin(csPinNum) -- This variation allows you to choose a different pin as the SPI Chip Select Pin.  Replace 'csPinNum' with your pin number.

isConnected()   -- Returns 'true' if the sensor is still responding as expected, and able to provide valid data.

available()     -- Reads the raw SCL3300 sensor data as a group so that all the data is consistent.  Call this first before using the functions below.  Starting with Version 3.0.0, this call should be the conditional in an 'if' statement, and an 'else' clause included to call reset() when available() returns false.  (See the example sketches in the library.)

getTiltLevelOffsetAngleX() -- Returns a double float of the tilt offset from level value in degrees for the X direction.

getTiltLevelOffsetAngleY() -- Returns a double float of the tilt offset from level value in degrees for the Y direction.

getTiltLevelOffsetAngleZ() -- Returns a double float of the tilt offset from level value in degrees for the Z direction.

getCalculatedAngleX() -- Returns a double float of the tilt value in degrees (0-360) for the X direction.

getCalculatedAngleY() -- Returns a double float of the tilt value in degrees (0-360) for the Y direction.

getCalculatedAngleZ() -- Returns a double float of the tilt value in degrees (0-360) for the Z direction.

getCalculatedAccelerometerX() -- Returns a double float of the accelerometer value in units of 'g' for the X direction.

getCalculatedAccelerometerY() -- Returns a double float of the accelerometer value in units of 'g' for the Y direction.

getCalculatedAccelerometerZ() -- Returns a double float of the accelerometer value in units of 'g' for the Z direction.

getTemperatureCelsius()   -- Returns a double float of the temperature in Celsius.

getTemperatureFarenheit() -- Returns a double float of the temperature in Farenheit.


Utility Functions available:

reset()           -- Does a software reset of the SCL3300 sensor.

getSerialNumber() -- Returns a long integer of the device Serial Number set by the manufacturer.

powerDownMode()   -- Puts the sensor in a power down mode to reduce power usage.

WakeMeUp()        -- Revives sensor from being powered down, so that it can start to generate sensor data.

setMode(modeNum) -- Sets the sensor mode to the number provided as modeNum.  The default mode is '4'.  Valid values are 1, 2, 3, and 4.

SetFastReadMode() -- Using Fast Read Mode in the library works by keeping the SPI connection continuously open.  This may or may not affect the behavior of other hardware interactions, depending on the sketch design.  Fast Read Mode is considered an advanced use case, and not recommended for the beginner.
