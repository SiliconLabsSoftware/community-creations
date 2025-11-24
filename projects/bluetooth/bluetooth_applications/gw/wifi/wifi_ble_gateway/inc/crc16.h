#ifndef CRC16_H
#define CRC16_H

#include <stdint.h>

uint16_t crc16_ccitt(const uint8_t *data, uint16_t length);

#endif 
