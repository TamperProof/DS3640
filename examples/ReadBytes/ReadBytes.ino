
#include <Wire.h>
#include <Time.h>
#include <DS3640.h>

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS3640 RTC Read Bytes");
  Serial.println("-------------------");
}

void loop() {
  byte read;
  //Read Serial Number
  Serial.println("DS3640 Serial Number is ");
  RTC.read(0x10, &read, 8); //Address to start reading from and number of bytes
  Serial.println();
  delay(2000); 
  Serial.println("DS3640 Current Temperature is ");
  RTC.read(0x26, &read, 2); //Address to start reading from and number of bytes
  Serial.println();
  delay(2000); 
  Serial.println("DS3640 Tamper Status is (Anything other than 00 is tampered) ");
  RTC.read(0x0e, &read, 2); //Address to start reading from and number of bytes
  Serial.println();
  delay(2000);
  Serial.println("DS3640 Current Battery Voltage is ");
  RTC.read(0x24, &read, 2); //Address to start reading from and number of bytes
  Serial.println();
  delay(2000);
  Serial.println("Creating 128 bit HW Generated Random Number ");
  RTC.RNG(&read, 16);
  Serial.println();
  delay(2000);
  Serial.println("Creating 256 bit HW Generated Random Number ");
  RTC.RNG(&read, 32);
  Serial.println();
  delay(2000);
}

