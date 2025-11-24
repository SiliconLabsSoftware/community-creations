#include "crc16.h"
uint16_t crc16_ccitt(const uint8_t *data, uint16_t length)
{
  uint16_t crc = 0xFFFF;  /* Initial value */
  
  for (uint16_t i = 0; i < length; i++) {
    crc ^= (uint16_t)data[i] << 8; 
    
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x8000) {
        crc = (crc << 1) ^ 0x1021;  /* Polynomial */
      } else {
        crc = crc << 1;
      }
      crc &= 0xFFFF;
    }
  }
  
  return crc;
}