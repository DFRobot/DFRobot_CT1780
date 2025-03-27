/*!
 * @file  cascading.ino
 * @brief This demo shows the cascading functionality.
 * @n     This demo can cascade 16 modules with different user configuration addresses at the same time.
 * @n     The method of changing the user's configuration address: according to the DIP switch of the module, select different combinations, and the range of the configuration address is 0-15.
 * @n     According to the silk screen of the DIP switch: A3-A0 are the 0-3 bits of binary respectively. There are a total of 16 combinations
 * @copyright   Copyright (c) 2025 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author      [fary](feng.yang@dfrobot.com)
 * @version     V1.0
 * @date        2025-03-22
 * @url         https://github.com/DFRobor/DFRobot_CT1780
 */

#include "DFRobot_CT1780.h"

#define MAX_DEVICES 16
 
struct sCT1780Device_t {
  uint8_t address[8];
  uint8_t configAddr;
};

struct sCT1780DeviceInfo_t {
  int count;
  sCT1780Device_t devices[MAX_DEVICES];
};

DFRobot_CT1780 CT1780(2);
sCT1780DeviceInfo_t sensorCt1780;
 
/*
* Search for CT1780 devices on the bus and save the number of parts, the unique address of the device,
* and DIP switch configuration address; If there is a CT1780 with the same configuration address on the bus,
* The number of devices returned is -1.
*/
void searchCT780(sCT1780DeviceInfo_t *result) {
  uint8_t addr[8];
  int foundCount = 0;
  result->count = 0;

  // Phase 1: Search for the device
  CT1780.reset_search();
  delay(10);
  
  while(CT1780.searchDevice(addr)) {
    if(foundCount >= MAX_DEVICES) break;
    memcpy(result->devices[foundCount].address, addr, 8);
    foundCount++;
  }

  // Stage 2: Check the configuration address
  for(int i=0; i<foundCount; i++){
    /**
     * -----------------------------------------
     *   A3  |  A2  |  A1  |  A0  |  config addr
     * -----------------------------------------
     *    0  |   0  |   0  |   0  |      0x00
     * -----------------------------------------
     *    1  |   0  |   0  |   0  |      0x01
     * -----------------------------------------
     *    0  |   1  |   0  |   0  |      0x02
     * -----------------------------------------     
     *    1  |   1  |   0  |   0  |      0x03
     * -----------------------------------------
     *    0  |   0  |   1  |   0  |      0x04
     * -----------------------------------------
     *    1  |   0  |   1  |   0  |      0x05
     * -----------------------------------------
     *    0  |   1  |   1  |   0  |      0x06
     * -----------------------------------------
     *    1  |   1  |   1  |   0  |      0x07
     * -----------------------------------------
     *    0  |   0  |   0  |   1  |      0x08
     * -----------------------------------------     
     *    1  |   0  |   0  |   1  |      0x09
     * -----------------------------------------    
     *    0  |   1  |   0  |   1  |      0x0A
     * -----------------------------------------  
     *    1  |   1  |   0  |   1  |      0x0B
     * -----------------------------------------
     *    0  |   0  |   1  |   1  |      0x0C
     * -----------------------------------------     
     *    1  |   0  |   1  |   1  |      0x0D
     * -----------------------------------------     
     *    0  |   1  |   1  |   1  |      0x0E
     * -----------------------------------------    
     *    1  |   1  |   1  |   1  |      0x0F
     * -----------------------------------------        
     */
    int ret = CT1780.getConfigAddr(result->devices[i].address);
    if(ret == -1){
      result->count = -1;
      return;
    }
    
    uint8_t cfg = (uint8_t)ret;
    result->devices[i].configAddr = cfg;

    // Check for duplicate addresses
    for(int j=0; j<i; j++){
      if(result->devices[j].configAddr == cfg){
        result->count = -1;
        return;
      }
    }
  }
  result->count = foundCount;
}
void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  Serial.print("Searching CT1780...");
  while(1){
    searchCT780(&sensorCt1780);
    if(sensorCt1780.count == -1){
      Serial.println("Duplicate config addresses found!");
      delay(1000);
    } else if(sensorCt1780.count == 0){
      Serial.println("No devices found");
      delay(1000);
    } else {
      Serial.print("\nFound ");
      Serial.print(sensorCt1780.count);
      Serial.println(" devices:");
      break;
    }
  }
}

void loop() {
  for(int i=0; i<sensorCt1780.count; i++){
    sCT1780Device_t &dev = sensorCt1780.devices[i];
    
    Serial.print("Device ");
    Serial.print(i+1);
    Serial.print(" (Config: 0x");
    Serial.print(dev.configAddr, HEX);
    Serial.print(") Address: ");
    
    for(int j=0; j<8; j++){
      if(dev.address[j] < 0x10) Serial.print('0');
      Serial.print(dev.address[j], HEX);
      if(j < 7) Serial.print(':');
    }
    
    Serial.print(" - Temp: ");
    Serial.print(CT1780.getCelsius(dev.address));
    Serial.println("°C");
  }
  Serial.println();
  delay(2000);

}
