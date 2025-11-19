#include "log.h"
#include <stdint.h>
#include <rsi_debug.h>

inline void hexdump(const char *tag, const uint8_t *buf, uint16_t len)
{
  if (!tag) tag = "HEX";
  DEBUGOUT("%s (%u): ", tag, (unsigned)len);
  for (uint16_t i = 0; i < len; i++) {
    DEBUGOUT("%02X ", buf[i]);
  }
  DEBUGOUT("\n");
}
