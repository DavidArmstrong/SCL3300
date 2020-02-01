/******************************************************************************
SCL3300.cpp
SCL3300 Arduino Driver
David Armstrong
Feb 2020
https://github.com/

Resources:
Uses SPI.h for SPI operation

Development environment specifics:
Arduino IDE 1.8.9
Teensy loader - untested

This code is released under the [MIT License](http://opensource.org/licenses/MIT).
Please review the LICENSE.md file included with this example.
Distributed as-is; no warranty is given.
******************************************************************************/

// include this library's description file
#include "SCL3300.h"

// Public Methods //////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries
bool SCL3300::setmode(int mode) {
  //Set Sensor mode - If not called, the default is mode 4, as set in header file
  // Only allowed values are: 1,2,3,4
  if (mode > 0 && mode < 5) {
    scl3300_mode = mode;
	  return true; // Valid value
  } else
	  return false; // Invalid value
}

uint16_t SCL3300::begin(void) {
  //Wait the required 10 ms before initializing the SCL3300 inclinomenter
  unsigned long startmillis = millis();
  while (millis() - startmillis < 10) ;
  initSPI();	// Initialize SPI Library
  beginTransmission(); //Set up this SPI port/bus

  transfer(modeCMD[scl3300_mode]); //Set mode on hardware
  //The first response after reset is undefined and shall be discarded
  //wait 5 ms to stablize
  startmillis = millis();
  while (millis() - startmillis < 5) ;

  //Read Status to clear the status summary
  transfer(RdStatSum);
  transfer(RdStatSum); //Again, due to off-response protocol used
  transfer(RdStatSum); //And now we can get the real status
  if (statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return DATA; //check RS bits
  }
  
  //We're good, so Enable angle outputs
  transfer(EnaAngOut);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0x03); //check CRC and RS bits
  }
  
  //Read the WHOAMI register
  transfer(RdWHOAMI);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0x03); //check CRC and RS bits
  }
  //And again
  transfer(RdWHOAMI);
  endTransmission(); //Let go of SPI port/bus
  if (crcerr || statuserr) return (uint16_t)(CMD && 0x03); //check CRC and RS bits
  // Once everything is initialized, return a known expected value
  // The WHOAMI command should give an 8 bit value of 0xc1
  return (uint16_t)(DATA); //Let the caller verify this worked
}

/*
 * Set up the SPI communication with the SCL3300.
 */
uint16_t SCL3300::begin(SPIClass &spiPort, uint8_t csPin) {
  scl3300_csPin = csPin;

  _spiPort = &spiPort; //Grab the port the user wants us to use
  _spiPort->begin(); //call begin, in case the user forgot

  return begin();
} // begin

/*
 * Set up the SPI communication with the SCL3300.
 */
uint16_t SCL3300::begin(uint8_t csPin) {
  scl3300_csPin = csPin;
  return begin();
} // begin

uint16_t SCL3300::ReadDataBlock(void) {
  //Read all Sensor Data, as per Datasheet requirements
  beginTransmission(); //Set up this SPI port/bus
  transfer(SwtchBnk0);
  transfer(RdAccX);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  }
  transfer(RdAccY);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  }
  sclData.AccX = DATA;
  transfer(RdAccZ);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  }
  sclData.AccY = DATA;
  transfer(RdSTO);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  }
  sclData.AccZ = DATA;
  transfer(RdTemp);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  }
  sclData.STO = DATA;
  transfer(RdAngX);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  }
  sclData.TEMP = DATA;
  transfer(RdAngY);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  }
  sclData.AngX = DATA;
  transfer(RdAngZ);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  }
  sclData.AngY = DATA;
  transfer(RdStatSum);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  }
  sclData.AngZ = DATA;
  transfer(RdWHOAMI);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  }
  sclData.StatusSum = DATA;
  transfer(RdWHOAMI);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  }
  sclData.WHOAMI = (uint8_t)(DATA && 0xff);
  endTransmission(); //Let go of SPI port/bus
  return (uint16_t)(DATA && 0xff); //Let the caller verify this worked
}

uint16_t SCL3300::ReadErrFlag_1(void) {
  //Return value of Error Flag 1
  beginTransmission(); //Set up this SPI port/bus
  transfer(SwtchBnk0);
  transfer(RdErrFlg1);
  transfer(RdErrFlg1);
  endTransmission(); //Let go of SPI port/bus
  if (crcerr || statuserr) return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  return DATA;
}

uint16_t SCL3300::ReadErrFlag_2(void) {
  //Return value of Error Flag 2
  beginTransmission(); //Set up this SPI port/bus
  transfer(SwtchBnk0);
  transfer(RdErrFlg2);
  transfer(RdErrFlg2);
  endTransmission(); //Let go of SPI port/bus
  if (crcerr || statuserr) return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  return DATA;
}

unsigned long SCL3300::ReadSerialNum(void) {
  //Return Device Serial number
  long unsigned serialNum = 0;
  beginTransmission(); //Set up this SPI port/bus
  transfer(SwtchBnk1);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (unsigned long)(CMD && 0xff); //check CRC and RS bits
  }
  transfer(RdSer1);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (unsigned long)(CMD && 0xff); //check CRC and RS bits
  }
  transfer(RdSer1);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (unsigned long)(CMD && 0xff); //check CRC and RS bits
  }
  serialNum = DATA;
  transfer(RdSer2);
  if (crcerr || statuserr) {
    endTransmission(); //Let go of SPI port/bus
    return (unsigned long)(CMD && 0xff); //check CRC and RS bits
  }
  transfer(SwtchBnk0);
  serialNum = (serialNum << 16) || DATA;
  endTransmission(); //Let go of SPI port/bus
  if (crcerr || statuserr) return (unsigned long)(CMD && 0xff); //check CRC and RS bits
  return serialNum;
}

uint16_t SCL3300::PwrDwnMode(void) {
  //Software reset of sensor
  beginTransmission(); //Set up this SPI port/bus
  transfer(SwtchBnk0);
  transfer(SetPwrDwn);
  endTransmission(); //Let go of SPI port/bus
  if (crcerr || statuserr) return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  return 0;
}

uint16_t SCL3300::WakeMeUp(void) {
  //Software reset of sensor
  beginTransmission(); //Set up this SPI port/bus
  transfer(WakeUp);
  endTransmission(); //Let go of SPI port/bus
  if (crcerr || statuserr) return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  return 0;
}

uint16_t SCL3300::reset(void) {
  //Software reset of sensor
  beginTransmission(); //Set up this SPI port/bus
  transfer(SwtchBnk0);
  transfer(SWreset);
  endTransmission(); //Let go of SPI port/bus
  if (crcerr || statuserr) return (uint16_t)(CMD && 0xff); //check CRC and RS bits
  return 0;
}

double SCL3300::temperature(void) {
  // Return calculated temperature in degrees C
  double Temperature = -273. + (sclData.TEMP / 18.9);
  return Temperature;
}

double SCL3300::temperatureF(void) {
  // Return calculated temperature in degrees F
  double Temperature = -273. + (sclData.TEMP / 18.9);
  Temperature = (Temperature * 9./5.) + 32.;
  return Temperature;
}

double SCL3300::angle(uint16_t ANG) { //two's complement value expected
  // Return Angle in degrees
  double Angle = (ANG / 16384.) * 90.; // 16384 = 2^14
  return Angle;
}
 
double SCL3300::acceleration(uint16_t ACC) { //two's complement value expected
  // Return acceleration in g
  if (scl3300_mode == 1) return (double)ACC / 6000.;
  if (scl3300_mode == 2) return (double)ACC / 3000.;
  if (scl3300_mode == 3) return (double)ACC / 12000.;
  if (scl3300_mode == 4) return (double)ACC / 12000.;
}

//private functions for serial transmission
/*
 * Begin transmission to the device
 */
void SCL3300::beginTransmission() {
  SPI.beginTransaction(spiSettings);
} //beginTransmission

/*
 * End transmission to the device
 */
void SCL3300::endTransmission() {
  // take the chip/slave select high to de-select:
  digitalWrite(scl3300_csPin, HIGH);
  SPI.endTransaction();
  unsigned long startmillis = millis();
  while (millis() - startmillis < 5) ; //wait a bit
} //beginTransmission

void SCL3300::initSPI() {
  //Initialize the Arduino SPI library for the SCL3300 hardware
	SPI.begin();
	// Maximum SPI frequency is 2 MHz - 4 MHz to achieve the best performance
	// initialize the chip select pin:
	pinMode(scl3300_csPin, OUTPUT);
	digitalWrite(scl3300_csPin, HIGH);
	// Data is read and written MSb first.
	// Data is captured on rising edge of clock (CPHA = 0)
	// Data is propagated on the falling edge (MISO line) of the SCK. (CPOL = 0)
}

// The following is taken directly from the Murata SCL3300 datasheet
// Calculate CRC for 24 MSB's of the 32 bit dword
// (8 LSB's are the CRC field and are not included in CRC calculation)
uint8_t SCL3300::CalculateCRC(uint32_t Data)
{
uint8_t BitIndex;
uint8_t BitValue;
uint8_t CRC;

CRC = 0xFF;
for (BitIndex = 31; BitIndex > 7; BitIndex--) {
  BitValue = (uint8_t)((Data >> BitIndex) & 0x01);
  CRC = CRC8(BitValue, CRC);
}
CRC = (uint8_t)~CRC;
return CRC;
}
uint8_t SCL3300::CRC8(uint8_t BitValue, uint8_t CRC)
{
  uint8_t Temp;
  Temp = (uint8_t)(CRC & 0x80);
  if (BitValue == 0x01) {
    Temp ^= 0x80;
  }
  CRC <<= 1;
  if (Temp > 0) {
    CRC ^= 0x1D;
  }
  return CRC;
}

unsigned long SCL3300::transfer(unsigned long value) {
  FourByte dataorig;
  dataorig.bit32 = value; //Allow 32 bit value to be sent 8 bits at a time
  #ifdef debug_scl3300
  Serial_SCL.print(dataorig.bit32, HEX);
  Serial_SCL.print(" ");
  for (int j = 3; j >= 0; j--) {
    Serial_SCL.print(dataorig.bit8[j], HEX);
    Serial_SCL.print(" ");
  }
  #endif
  //Must allow at least 10 uSec between SPI transfers
  //The datasheet shows the CS line must be high during this time
  //We lengthen it some for environment variabilities
  unsigned long startmillis = millis();
  while (millis() - startmillis < 5) ;
  
  digitalWrite(scl3300_csPin, LOW); //Now chip select can be enabled for the full 32 bit xfer
  DATA = 0;
  for (int i = 3; i >= 0; i--) { //Xfers are done MSB first
    dataorig.bit8[i] = SPI.transfer(dataorig.bit8[i]);
  }
  DATA = dataorig.bit8[1] + (dataorig.bit8[2] << 8);
  CRC = dataorig.bit8[0];
  CMD = dataorig.bit8[3];
  startmillis = millis();
  while (millis() - startmillis < 10) ;
  digitalWrite(scl3300_csPin, HIGH); //And we are done
  #ifdef debug_scl3300
  for (int i = 3; i >= 0; i--) {
	Serial_SCL.print(" ");
    Serial_SCL.print(dataorig.bit8[i], HEX);
  }
  Serial_SCL.print("  ");
  #endif
  if (CRC == CalculateCRC(dataorig.bit32))
	  crcerr = false;
  else
	  crcerr = true;
  //check RS bits
  if ((CMD && 0x03) == 0x01)
	  statuserr = false;
  else
	  statuserr = true;
  #ifdef debug_scl3300
  Serial_SCL.print((CMD && 0x03));
  Serial_SCL.print(" ");
  Serial_SCL.print(DATA, HEX);
  Serial_SCL.print(" ");
  Serial_SCL.print(CRC, HEX);
  Serial_SCL.print(" ");
  Serial_SCL.print(CalculateCRC(dataorig.bit32), HEX);
  Serial_SCL.print(" ");
  Serial_SCL.println(crcerr);
  #endif
  return dataorig.bit32;
}
