/*
 * DS3640.h - library for DS3640 RTC  
 
  Copyright (c) Michael Margolis 2009
  This library is intended to be uses with Arduino Time.h library functions

  The library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  30 Dec 2009 - Initial release
  5 Sep 2011 updated for Arduino 1.0
 */
 
 

#include <Wire.h>
#include "DS3640.h"

#define DS3640_CTRL_ID 0x50

DS3640::DS3640()
{
  Wire.begin();
}
  
// PUBLIC FUNCTIONS
time_t DS3640::get()   // Aquire data from buffer and convert to time_t
{
  tmElements_t tm;
  if (readtm(tm) == false) return 0;
  return(makeTime(tm));
}

bool DS3640::set(time_t t)
{
  tmElements_t tm;
  breakTime(t, tm);
  tm.Second |= 0x80;  // stop the clock 
  writetm(tm); 
  tm.Second &= 0x7f;  // start the clock
  writetm(tm); 
}

// Aquire data from the RTC chip in BCD format
bool DS3640::readtm(tmElements_t &tm)
{
  uint8_t sec;
  Wire.beginTransmission(DS3640_CTRL_ID);
#if ARDUINO >= 100  
  Wire.write((uint8_t)0x01); 
#else
  Wire.send(0x01);
#endif  
  if (Wire.endTransmission() != 0) {
    exists = false;
    return false;
  }
  exists = true;

  // request the 7 data fields   (secs, min, hr, dow, date, mth, yr)
  Wire.requestFrom(DS3640_CTRL_ID, tmNbrFields);
  if (Wire.available() < tmNbrFields) return false;
#if ARDUINO >= 100
  sec = Wire.read();
  tm.Second = bcd2dec(sec & 0x7f);   
  tm.Minute = bcd2dec(Wire.read() );
  tm.Hour =   bcd2dec(Wire.read() & 0x3f);  // mask assumes 24hr clock
  tm.Wday = bcd2dec(Wire.read() );
  tm.Day = bcd2dec(Wire.read() );
  tm.Month = bcd2dec(Wire.read() );
  tm.Year = y2kYearToTm((bcd2dec(Wire.read())));
#else
  sec = Wire.receive();
  tm.Second = bcd2dec(sec & 0x7f);   
  tm.Minute = bcd2dec(Wire.receive() );
  tm.Hour =   bcd2dec(Wire.receive() & 0x3f);  // mask assumes 24hr clock
  tm.Wday = bcd2dec(Wire.receive() );
  tm.Day = bcd2dec(Wire.receive() );
  tm.Month = bcd2dec(Wire.receive() );
  tm.Year = y2kYearToTm((bcd2dec(Wire.receive())));
#endif
  if (sec & 0x80) return false; // clock is halted
  return true;
}

bool DS3640::writetm(tmElements_t &tm)
{
  Wire.beginTransmission(DS3640_CTRL_ID);
#if ARDUINO >= 100  
  Wire.write((uint8_t)0x01); // reset register pointer  
  Wire.write(dec2bcd(tm.Second)) ;   
  Wire.write(dec2bcd(tm.Minute));
  Wire.write(dec2bcd(tm.Hour));      // sets 24 hour format
  Wire.write(dec2bcd(tm.Wday));   
  Wire.write(dec2bcd(tm.Day));
  Wire.write(dec2bcd(tm.Month));
  Wire.write(dec2bcd(tmYearToY2k(tm.Year))); 
#else  
  Wire.send(0x01); // reset register pointer  
  Wire.send(dec2bcd(tm.Second)) ;   
  Wire.send(dec2bcd(tm.Minute));
  Wire.send(dec2bcd(tm.Hour));      // sets 24 hour format
  Wire.send(dec2bcd(tm.Wday));   
  Wire.send(dec2bcd(tm.Day));
  Wire.send(dec2bcd(tm.Month));
  Wire.send(dec2bcd(tmYearToY2k(tm.Year)));   
#endif
  if (Wire.endTransmission() != 0) {
    exists = false;
    return false;
  }
  exists = true;
  return true;
}

  // request the 2 data fields used for temperature
void DS3640::temp()
{
  byte temp[2];
  int i;
  Wire.beginTransmission(DS3640_CTRL_ID);
  Wire.send(0x26); // reset register pointer 
  Wire.endTransmission();
  Wire.requestFrom(DS3640_CTRL_ID, 2);
  for (i=0; i<2; i++) {
	temp[i] = Wire.read();
	Serial.print(temp[i], HEX);
  }
  
}

  // request HW generated random number
byte DS3640::RNG(byte *values, byte nBytes)
{
  int i;
  Wire.beginTransmission(DS3640_CTRL_ID);
  Wire.send(0x7f); // reset register pointer 
  Wire.send(0x00); // set to slot 0
  Wire.endTransmission();
  Wire.beginTransmission(DS3640_CTRL_ID);
  Wire.send(0x80); // reset register pointer 
  Wire.endTransmission();
  Wire.requestFrom(DS3640_CTRL_ID, nBytes);
  for (i=0; i<nBytes; i++) {
	values[i] = Wire.read();
	//Serial.println("read ");
	Serial.print(values[i], HEX);
  }
  
}
 // read nBytes from DS3640 starting at register addr
byte DS3640::read(byte addr, byte *values, byte nBytes)
{
  int i;
  Wire.beginTransmission(DS3640_CTRL_ID);
  Wire.send(addr); // reset register pointer 
  Wire.endTransmission();
  Wire.requestFrom(DS3640_CTRL_ID, nBytes);
  for (i=0; i<nBytes; i++) {
	values[i] = Wire.read();
	//Serial.println("read ");
	Serial.print(values[i], HEX);
  }
  return 0;
}
// read a single byte to DS3640 register addr
//byte DS3640::read(byte addr)
//{
//    byte b;
    
//    readRTC(addr, &b, 1);
//    return b;
//}

 // write nBytes to DS3640 starting at register addr
byte DS3640::write(byte addr, byte *values, byte nBytes)
{
  //byte write[nBytes];
  int i;
  Wire.beginTransmission(DS3640_CTRL_ID);
  Wire.send(addr); // reset register pointer 
  for (i=0; i<nBytes; i++) {
	Wire.send(values[i]);
	//Serial.println("wrote ");
	Serial.print(values[i], HEX);
  }
  return Wire.endTransmission();
  }
 
// write a single byte to DS3640 register addr
//byte DS3640::writebyte(byte addr, byte value)
//{
//    return ( write(addr, &value, 1) );
//}

// PRIVATE FUNCTIONS

// Convert Decimal to Binary Coded Decimal (BCD)
uint8_t DS3640::dec2bcd(uint8_t num)
{
  return ((num/10 * 16) + (num % 10));
}

// Convert Binary Coded Decimal (BCD) to Decimal
uint8_t DS3640::bcd2dec(uint8_t num)
{
  return ((num/16 * 10) + (num % 16));
}

bool DS3640::exists = false;

DS3640 RTC = DS3640(); // create an instance for the user



/*
 * DS3640.h - library for DS3640 RTC  
 
  Copyright (c) Michael Margolis 2009
  This library is intended to be uses with Arduino Time.h library functions

  The library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  30 Dec 2009 - Initial release
  5 Sep 2011 updated for Arduino 1.0
 */
 
 

#include <Wire.h>
#include "DS3640.h"

#define DS3640_CTRL_ID 0x50

DS3640::DS3640()
{
  Wire.begin();
}
  
// PUBLIC FUNCTIONS
time_t DS3640::get()   // Aquire data from buffer and convert to time_t
{
  tmElements_t tm;
  if (readtm(tm) == false) return 0;
  return(makeTime(tm));
}

bool DS3640::set(time_t t)
{
  tmElements_t tm;
  breakTime(t, tm);
  tm.Second |= 0x80;  // stop the clock 
  writetm(tm); 
  tm.Second &= 0x7f;  // start the clock
  writetm(tm); 
}

// Aquire data from the RTC chip in BCD format
bool DS3640::readtm(tmElements_t &tm)
{
  uint8_t sec;
  Wire.beginTransmission(DS3640_CTRL_ID);
#if ARDUINO >= 100  
  Wire.write((uint8_t)0x01); 
#else
  Wire.send(0x01);
#endif  
  if (Wire.endTransmission() != 0) {
    exists = false;
    return false;
  }
  exists = true;

  // request the 7 data fields   (secs, min, hr, dow, date, mth, yr)
  Wire.requestFrom(DS3640_CTRL_ID, tmNbrFields);
  if (Wire.available() < tmNbrFields) return false;
#if ARDUINO >= 100
  sec = Wire.read();
  tm.Second = bcd2dec(sec & 0x7f);   
  tm.Minute = bcd2dec(Wire.read() );
  tm.Hour =   bcd2dec(Wire.read() & 0x3f);  // mask assumes 24hr clock
  tm.Wday = bcd2dec(Wire.read() );
  tm.Day = bcd2dec(Wire.read() );
  tm.Month = bcd2dec(Wire.read() );
  tm.Year = y2kYearToTm((bcd2dec(Wire.read())));
#else
  sec = Wire.receive();
  tm.Second = bcd2dec(sec & 0x7f);   
  tm.Minute = bcd2dec(Wire.receive() );
  tm.Hour =   bcd2dec(Wire.receive() & 0x3f);  // mask assumes 24hr clock
  tm.Wday = bcd2dec(Wire.receive() );
  tm.Day = bcd2dec(Wire.receive() );
  tm.Month = bcd2dec(Wire.receive() );
  tm.Year = y2kYearToTm((bcd2dec(Wire.receive())));
#endif
  if (sec & 0x80) return false; // clock is halted
  return true;
}

bool DS3640::writetm(tmElements_t &tm)
{
  Wire.beginTransmission(DS3640_CTRL_ID);
#if ARDUINO >= 100  
  Wire.write((uint8_t)0x01); // reset register pointer  
  Wire.write(dec2bcd(tm.Second)) ;   
  Wire.write(dec2bcd(tm.Minute));
  Wire.write(dec2bcd(tm.Hour));      // sets 24 hour format
  Wire.write(dec2bcd(tm.Wday));   
  Wire.write(dec2bcd(tm.Day));
  Wire.write(dec2bcd(tm.Month));
  Wire.write(dec2bcd(tmYearToY2k(tm.Year))); 
#else  
  Wire.send(0x01); // reset register pointer  
  Wire.send(dec2bcd(tm.Second)) ;   
  Wire.send(dec2bcd(tm.Minute));
  Wire.send(dec2bcd(tm.Hour));      // sets 24 hour format
  Wire.send(dec2bcd(tm.Wday));   
  Wire.send(dec2bcd(tm.Day));
  Wire.send(dec2bcd(tm.Month));
  Wire.send(dec2bcd(tmYearToY2k(tm.Year)));   
#endif
  if (Wire.endTransmission() != 0) {
    exists = false;
    return false;
  }
  exists = true;
  return true;
}

  // request the 2 data fields used for temperature
void DS3640::temp()
{
  byte temp[2];
  int i;
  Wire.beginTransmission(DS3640_CTRL_ID);
  Wire.send(0x26); // reset register pointer 
  Wire.endTransmission();
  Wire.requestFrom(DS3640_CTRL_ID, 2);
  for (i=0; i<2; i++) {
	temp[i] = Wire.read();
	Serial.print(temp[i], HEX);
  }
  
}

  // request HW generated random number
byte DS3640::RNG(byte *values, byte nBytes)
{
  int i;
  Wire.beginTransmission(DS3640_CTRL_ID);
  Wire.send(0x7f); // reset register pointer 
  Wire.send(0x00); // set to slot 0
  Wire.endTransmission();
  Wire.beginTransmission(DS3640_CTRL_ID);
  Wire.send(0x80); // reset register pointer 
  Wire.endTransmission();
  Wire.requestFrom(DS3640_CTRL_ID, nBytes);
  for (i=0; i<nBytes; i++) {
	values[i] = Wire.read();
	//Serial.println("read ");
	Serial.print(values[i], HEX);
  }
  
}
 // read nBytes from DS3640 starting at register addr
byte DS3640::read(byte addr, byte *values, byte nBytes)
{
  int i;
  Wire.beginTransmission(DS3640_CTRL_ID);
  Wire.send(addr); // reset register pointer 
  Wire.endTransmission();
  Wire.requestFrom(DS3640_CTRL_ID, nBytes);
  for (i=0; i<nBytes; i++) {
	values[i] = Wire.read();
	//Serial.println("read ");
	Serial.print(values[i], HEX);
  }
  return 0;
}
// read a single byte to DS3640 register addr
//byte DS3640::read(byte addr)
//{
//    byte b;
    
//    readRTC(addr, &b, 1);
//    return b;
//}

 // write nBytes to DS3640 starting at register addr
byte DS3640::write(byte addr, byte *values, byte nBytes)
{
  //byte write[nBytes];
  int i;
  Wire.beginTransmission(DS3640_CTRL_ID);
  Wire.send(addr); // reset register pointer 
  for (i=0; i<nBytes; i++) {
	Wire.send(values[i]);
	//Serial.println("wrote ");
	Serial.print(values[i], HEX);
  }
  return Wire.endTransmission();
  }
 
// write a single byte to DS3640 register addr
//byte DS3640::writebyte(byte addr, byte value)
//{
//    return ( write(addr, &value, 1) );
//}

// PRIVATE FUNCTIONS

// Convert Decimal to Binary Coded Decimal (BCD)
uint8_t DS3640::dec2bcd(uint8_t num)
{
  return ((num/10 * 16) + (num % 10));
}

// Convert Binary Coded Decimal (BCD) to Decimal
uint8_t DS3640::bcd2dec(uint8_t num)
{
  return ((num/16 * 10) + (num % 16));
}

bool DS3640::exists = false;

DS3640 RTC = DS3640(); // create an instance for the user



