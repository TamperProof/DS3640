/*
 * DS3640.h - library for DS340 RTC
 * This library is intended to be uses with Arduino Time.h library functions
 */

#ifndef DS3640_h
#define DS3640_h

#include <Time.h>

// library interface description
class DS3640
{
  // user-accessible "public" interface
  public:
    DS3640();
    static time_t get();
    static bool set(time_t t);
    static bool readtm(tmElements_t &tm);
    static bool writetm(tmElements_t &tm);
    static bool chipPresent() { return exists; }
	byte read(byte addr, byte *values, byte nBytes);
	//byte read(byte addr, byte value);
	byte write(byte addr, byte *values, byte nBytes);
	//byte writebyte(byte addr, byte value);
	byte RNG(byte *values, byte nBytes);
	void temp();

  private:
    static bool exists;
    static uint8_t dec2bcd(uint8_t num);
    static uint8_t bcd2dec(uint8_t num);
};

#ifdef RTC
#undef RTC // workaround for Arduino Due, which defines "RTC"...
#endif

extern DS3640 RTC;

#endif
 

