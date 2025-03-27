/*!
 * @file  DFRobot_CT1780.cpp
 * @brief  This is the method implementation file of CT1780
 * @copyright   Copyright (c) 2025 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author      [fary](feng.yang@dfrobot.com)
 * @version     V1.0
 * @date        2025-03-07
 * @url         https://github.com/DFRobor/DFRobot_CT1780
 */
#include "DFRobot_CT1780.h"
bool isAllEqual(const uint8_t *data, size_t len) {
  for (size_t i = 1; i < len; i++) {
    if (data[i] != data[0]) {
      return false;
    }
  }
  return true;
}

int DFRobot_CT1780::searchDevice(uint8_t *newAddr) {
  if(newAddr==NULL){
    return 0;
  }
  if (search(newAddr)) {
    if( (crc8(newAddr, 7) == newAddr[7]) && (newAddr[0] == 0x3B) ) {
      return 1;
    }
  }
  return 0;
}

float DFRobot_CT1780::getCelsius(uint8_t *newAddr){
  if(newAddr==NULL){
    return NAN;
  }
  uint8_t data[9];
  reset();
  select(newAddr);
  write(CMD_CONVERT_T);
  delay(750); // Wait for the conversion to complete
  
  reset();
  select(newAddr);
  write(CMD_READ_SCRATCHPAD);
  
  for(int i = 0; i < 9; i++) {
    data[i] = read();
  }
  
  if(isAllEqual(data,9)||crc8(data, 8) != data[8]) {
    return NAN;
  }
  
  // Analytic temperature data
  int16_t rawTemperature = (data[1] << 8) | data[0];
  rawTemperature >>=2;
  if (rawTemperature & 0x2000) {
    rawTemperature |= 0xC000;
  }
  float temp = rawTemperature * 0.25f;// The temperature resolution is 0.25°C
  if(temp>1768 || temp<(-270)){
      return NAN;
  }
  return temp; 
}

int DFRobot_CT1780::getConfigAddr(uint8_t *newAddr){
  if(newAddr==NULL){
    return -1;
  }
  uint8_t data[9];
  reset();
  select(newAddr);
  write(CMD_READ_SCRATCHPAD);
  for(int i = 0; i < 9; i++) {
    data[i] = read();
  }
  
  if(isAllEqual(data,9)||crc8(data, 8) != data[8]) {
    return -1;
  }
  // Extract AD0-AD3 information
  uint8_t configRegister = data[4];
  uint8_t adPins = configRegister & 0x0F; // The lower 4 bits are AD0-AD3
  return adPins;
}