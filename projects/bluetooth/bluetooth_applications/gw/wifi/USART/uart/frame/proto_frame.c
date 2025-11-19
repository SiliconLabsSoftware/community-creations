#ifndef PROTOCOL_PROTO_FRAME_C_
#define PROTOCOL_PROTO_FRAME_C_

#include <frame/proto_frame.h>
#include <rsi_debug.h>
#include "comm_types.h"

uint16_t crc16_ccitt(const uint8_t *data, uint16_t len)
{
  uint16_t crc = CRC16_INIT;
  for (uint16_t i = 0; i < len; i++) {
    crc ^= (uint16_t)((uint16_t)data[i] << 8);
    for (uint8_t j = 0; j < 8; j++) {
      crc = (crc & 0x8000u) ? (uint16_t)((crc << 1) ^ CRC16_POLY)
                            : (uint16_t)(crc << 1);
    }
  }
  return crc;
}

uint16_t proto_build(uint8_t *dst,
                     uint16_t cap,
                     uint16_t type_le,
                     const uint8_t *payload,
                     uint16_t payload_len)
{
  if (!dst) return 0;
  if (payload_len > 0 && !payload) return 0;

  const uint16_t need = (uint16_t)(LEN_HEADER + payload_len + LEN_FOOTER);
  if (cap < need) return 0;

  uint16_t idx = 0;
  dst[idx++] = (uint8_t)SOF;

  put_u16_le(&dst[idx], type_le);    idx += LEN_TYPE;
  put_u16_le(&dst[idx], payload_len);idx += LEN_PLEN;

  if (payload_len) { memcpy(&dst[idx], payload, payload_len); idx += payload_len; }

  dst[idx++] = (uint8_t)ENDCODE;

  const uint16_t crc = crc16_ccitt(&dst[1], (uint16_t)(LEN_TYPE + LEN_PLEN + payload_len + LEN_ENDCODE));
  put_u16_le(&dst[idx], crc); idx += LEN_CRC;

  dst[idx++] = (uint8_t)TAIL;

  return idx;
}

bool proto_parse(const uint8_t *frame, // Con tro toi frame nhan duo
                 uint16_t len,         // Chieu dai thuc te cua frame nhan duoc
                 uint16_t *out_type,   // Con tro tra ve TYPE
                 const uint8_t **out_payload, // Con tro toi payload trong frame
                 uint16_t *out_plen)          // Con tro luu do dai payload
{
  if ((frame == NULL) || (len < MIN_FRAME_LEN)) {
    DEBUGOUT("Frame invalid: too short (len=%u)\n", len);
    return false;
  }
  if (frame[0] != (uint8_t)SOF) {
      DEBUGOUT("Frame invalid: bad SOF (0x%02X)\n", frame[0]);
      return false;
    }
  if (frame[len - 1u] != (uint8_t)TAIL) {
      DEBUGOUT("Frame invalid: bad TAIL (0x%02X)\n", frame[len - 1u]);
      return false;
    }

  const uint16_t type        = u16_le(frame[1], frame[2]);
  const uint16_t payload_len = u16_le(frame[3], frame[4]);
  const uint16_t expect_len  = (uint16_t)(LEN_HEADER + payload_len + LEN_FOOTER);

  if (len != expect_len) {
    DEBUGOUT("Frame invalid: length mismatch (len=%u expect=%u payload=%u)\n",
             len, expect_len, payload_len);
    return false;
  }

  const uint16_t end_pos = (uint16_t)(LEN_HEADER + payload_len);
  if (frame[end_pos] != (uint8_t)ENDCODE) return false;

  const uint16_t rx_crc  = u16_le(frame[end_pos + 1], frame[end_pos + 2]);
  const uint16_t cal_crc = crc16_ccitt(&frame[1], (uint16_t)(LEN_TYPE + LEN_PLEN + payload_len + LEN_ENDCODE));
  if (rx_crc != cal_crc) {
    DEBUGOUT("CRC mismatch: rx=%04X calc=%04X\n", rx_crc, cal_crc);
    return false;
  }

  if (out_type)    *out_type    = type;
  // Neu type khac NULL thi gan gia tri vao bien type

  if (out_payload) *out_payload = &frame[LEN_HEADER];
  // Neu payload khac NULL thi gan gia tri cua con tro vao vi tri frame[5]
  // Sau header (1Byte), type (2Byte), length (2Byte)

  // Neu do dai cua payload != NULL thi gan gia tri vao payload_len
  if (out_plen)    *out_plen    = payload_len;

  DEBUGOUT("RX OK: TYPE=0x%04X, PAYLOAD(%u): ", type, payload_len);
  for (uint16_t i = 0; i < payload_len; i++) {
    DEBUGOUT("%02X", (unsigned int) out_payload[i]);
  }
  DEBUGOUT("\n");

  return true;
}


#endif /* PROTOCOL_PROTO_FRAME_C_ */
