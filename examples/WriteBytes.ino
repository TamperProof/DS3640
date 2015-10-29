
#include <Wire.h>
#include <Time.h>
#include <DS3640.h>

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS3640 Write Keys to SRAM");
  Serial.println("-------------------"); 
  
}
byte key1[128] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127};
//128 byte key = 1024bits

byte key2[64] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63};
//64 byte key = 512bits

byte key3[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
//32 byte key = 256bits



void loop() {
  Serial.println(); 
  Serial.println("Writing 1024 bit key to slot 1");
  byte slot = 0x01;
  RTC.write(0x7f, &slot, 1); //Select slot 1
  byte x = 0;
  for (x=0; x<8; x++) { 
    RTC.write(0x80, &key1[x], 8);
    break;
     }
  for (x=8; x<16; x++) { 
    RTC.write(0x88, &key1[x], 8);
    break;
    }
  for (x=16; x<24; x++) { 
    RTC.write(0x90, &key1[x], 8);
    break;
    }
  for (x=24; x<32; x++) { 
    RTC.write(0x98, &key1[x], 8);
    break;
    }
  for (x=32; x<40; x++) { 
    RTC.write(0xA0, &key1[x], 8);
    break;
    }
  for (x=40; x<48; x++) { 
    RTC.write(0xA8, &key1[x], 8);
    break;
    }
  for (x=48; x<56; x++) { 
    RTC.write(0xB0, &key1[x], 8);
    break;
    }
  for (x=56; x<64; x++) { 
    RTC.write(0xB8, &key1[x], 8);
    break;
    }
  for (x=64; x<72; x++) { 
    RTC.write(0xC0, &key1[x], 8);
    break;
    }
  for (x=72; x<80; x++) { 
    RTC.write(0xC8, &key1[x], 8);
    break;
    }
  for (x=80; x<88; x++) { 
    RTC.write(0xD0, &key1[x], 8);
    break;
    }
  for (x=88; x<96; x++) { 
    RTC.write(0xD8, &key1[x], 8);
    break;
    }
  for (x=96; x<104; x++) { 
    RTC.write(0xE0, &key1[x], 8);
    break;
    }
  for (x=104; x<112; x++) { 
    RTC.write(0xE8, &key1[x], 8);
    break;
    } 
  for (x=112; x<120; x++) { 
    RTC.write(0xF0, &key1[x], 8);
    break;
    } 
  for (x=120; x<128; x++) { 
    RTC.write(0xF8, &key1[x], 8);
    break;
    }
  delay(1000);
  Serial.println(); 
  Serial.println("Writing 512 bit key to slot 2");
  slot = 0x02;
  RTC.write(0x7f, &slot, 1); //Select slot 2
  x = 0;
  for (x=0; x<8; x++) { 
    RTC.write(0x80, &key2[x], 8);
    break;
     }
  for (x=8; x<16; x++) { 
    RTC.write(0x88, &key2[x], 8);
    break;
    }
  for (x=16; x<24; x++) { 
    RTC.write(0x90, &key2[x], 8);
    break;
    }
  for (x=24; x<32; x++) { 
    RTC.write(0x98, &key2[x], 8);
    break;
    }
  for (x=32; x<40; x++) { 
    RTC.write(0xA0, &key2[x], 8);
    break;
    }
  for (x=40; x<48; x++) { 
    RTC.write(0xA8, &key2[x], 8);
    break;
    }
  for (x=48; x<56; x++) { 
    RTC.write(0xB0, &key2[x], 8);
    break;
    }
  for (x=56; x<64; x++) { 
    RTC.write(0xB8, &key2[x], 8);
    break;
    }
  delay(1000);  
  Serial.println(); 
  Serial.println("Writing 256 bit key to slot 3");
  slot = 0x03;
  RTC.write(0x7f, &slot, 1); //Select slot 3
  x = 0;
  for (x=0; x<8; x++) { 
    RTC.write(0x80, &key3[x], 8);
    break;
     }
  for (x=8; x<16; x++) { 
    RTC.write(0x88, &key3[x], 8);
    break;
    }
  for (x=16; x<24; x++) { 
    RTC.write(0x90, &key3[x], 8);
    break;
    }
  for (x=24; x<32; x++) { 
    RTC.write(0x98, &key3[x], 8);
    break;
    }
  delay(1000);
}



