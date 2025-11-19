/*
 * log.h
 *
 *  Created on: 17 thg 9, 2025
 *      Author: nguye
 */

#ifndef UTILS_LOG_H_
#define UTILS_LOG_H_
#include "stdint.h"

void hexdump(const char *tag, const uint8_t *buf, uint16_t len);

#endif /* UTILS_LOG_H_ */
