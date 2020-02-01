scl3300_readall

Februrary 2020 - Version 1.0.0 - 
By B David Armstrong

This is an example test sketch that demonstrates the use of the SCL3300 Inclinometer library with the Arduino IDE.  

It is recommended that one use a terminal program such as Tera-Term to access your Arduino-compatible board via USB.  You may need to press the Reset button to see the program start from the beginning.
https://learn.sparkfun.com/tutorials/terminal-basics/tera-term-windows

One can obtain a Murata inclinometer from on-line retailers.  Mounting the bare surface-mount integrated circuit onto a PCB requires special handling though. For this reason, for the usual one-off use, I'd recommend obtaining Murata's own Evaluation PCB as sold by Mouser Electronics.  Here are two references I've found:

SPI Murata SCL3300-D01-1 inclinometer: https://www.mouser.com/ProductDetail/Murata-Electronics/SCL3300-D01-PCB?qs=vLWxofP3U2yrcJsOYvcWaQ%3D%3D
https://www.digikey.com/product-detail/en/murata-electronics-north-america/SCL3300-D01-10/490-18218-1-ND/9950619
Note: Yeah, it's a pricey little item.  But there really isn't anything else that will do the job.

Datasheet: https://www.murata.com/-/media/webrenewal/products/sensor/pdf/datasheet/datasheet_scl3300-d01.ashx?la=en-us
Evaluation Board: https://www.murata.com/-/media/webrenewal/products/sensor/pdf/specification/pcbspec_scx3300.ashx?la=en-us

==========================

IMPORTANT!

The Murata SPI inclinometer SCL3300 is a 3.3 volt device.  As such, it must NOT be connected directly to the SPI pins on an Arduino UNO or compatible board, as these systems are designed with 5 volt interface pins.  For all 5 volt systems, it is required that a bidirectional level shifter be used to handle the electrical signal translation.

Here is a reference for how to provide a compatible electrical setup:

https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi

Sparkfun bi-directional level shifter: https://www.sparkfun.com/products/12009

NOTE: Careful reading of SPI interface documentation suggests that it is a good idea to have a pull-up resistor to +3.3 volts installed on the Chip/Slave Select line of the SPI interface.  A typical value would be 4.7k ohms.  The reason for this is because latch timing can be related to stable chip select levels.  The Sparkfun Redboard Turbo requires this pull-up resistor to be able communicate with the SCL3300 at all.

For an Arduino Uno, use these pins:
SCK - Digital Pin 13
SDO - Digital Pin 12 (MISO)
SDI - Digital Pin 11 (MOSI)
CS - Digital Pin 10  (SS)

For a SAMD-type Arduino, such as the Sparkfun Redboard Turbo, the default SPI pins are only on the ICSP connector:
pins 22 (MISO), 23 (MOSI), and 24 (SCK) on ICSP header, and for Chip/Slave Select use digital Pin 10

======================================
