/*!
 * @file  DFRobot_CT1780.h
 * @brief  This is the user manual of CT1780
 * @copyright   Copyright (c) 2025 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author      [fary](feng.yang@dfrobot.com)
 * @version     V1.0
 * @date        2025-03-07
 * @url         https://github.com/DFRobor/DFRobot_CT1780
 */
#ifndef _DFRobot_CT1780_H_ 
#define _DFRobot_CT1780_H_
#include "OneWire.h"
#define CMD_CONVERT_T 0x44
#define CMD_READ_SCRATCHPAD 0xBE
class DFRobot_CT1780 :public OneWire
{
public:
  /**
   * @fn: DFRobot_CT1780
   * @brief: Constructor, passing in the data pin of the CT1780 connection
   * @param pin: The pin of the OneWire data cable connection
   */
  DFRobot_CT1780(uint8_t pin):OneWire(pin){};

  /**
   * @fn: searchDevice
   * @brief: Search for CT1780 device connected to the bus
   * @param newAddr:If a new device is retrieved, the 64-bit unique ID of the device is stored at that address
   * @return: Returns 1 if a new address has been returned. 0 might mean that the bus is shorted, there are no devices, or you have already retrieved all of them.
   */
  int searchDevice(uint8_t *newAddr );

  /**
   * @fn: getCelsius
   * @brief: Read probe temperature data
   * @param newAddr:device address
   * @return: float Temperature value (in degrees Celsius),Return NAN on failure.
   */
  float getCelsius(uint8_t *newAddr);

  /**
   * @fn: getConfigAddr
   * @brief: Get the user-configured address of CT1780 (in ScratchPad)
   * @param newAddr:device address
   * @return: address data,Return -1 on failure.
   */
  int getConfigAddr(uint8_t *newAddr);

};
#endif