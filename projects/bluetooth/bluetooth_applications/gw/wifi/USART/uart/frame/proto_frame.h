
#ifndef FRAME_PROTO_FRAME_H_
#define FRAME_PROTO_FRAME_H_

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ------------ Protocol constants (giữ đơn giản cho UART test) ----------- */
#define SOF                         0xF0u
#define ENDCODE                     0xFFu
#define TAIL                        0x0Fu

#define LEN_SOF       1u
#define LEN_TYPE      2u
#define LEN_PLEN      2u
#define LEN_ENDCODE   1u
#define LEN_CRC       2u
#define LEN_TAIL      1u
#define LEN_HEADER    (LEN_SOF + LEN_TYPE + LEN_PLEN)
#define LEN_FOOTER    (LEN_ENDCODE + LEN_CRC + LEN_TAIL)
#define MIN_FRAME_LEN (LEN_HEADER + LEN_FOOTER)

#define UART_RX_BUFFER_SIZE         256u
#define UART_TX_BUFFER_SIZE         1024u

/* CRC16-CCITT */
#define CRC16_POLY                  0x1021u
#define CRC16_INIT                  0xFFFFu

/* ---------------- Small helpers ---------------- */
static inline uint16_t u16_le(uint8_t lo, uint8_t hi) {
  return (uint16_t)((uint16_t)lo | ((uint16_t)hi << 8));
}
static inline void put_u16_le(uint8_t *dst, uint16_t v) {
  dst[0] = (uint8_t)(v & 0xFFu);
  dst[1] = (uint8_t)((v >> 8) & 0xFFu);
}

/* ---------------- Public API ------------------- */
uint16_t crc16_ccitt(const uint8_t *data, uint16_t len);

/* Build frame: SOF | TYPE(LE) | LEN(LE) | PAYLOAD | ENDCODE | CRC(LE) | TAIL.
 * Trả về tổng số byte của frame ( >0 ), hoặc 0 nếu lỗi. */
uint16_t proto_build(uint8_t *dst,
                     uint16_t dst_cap,
                     uint16_t type_le,
                     const uint8_t *payload,
                     uint16_t payload_len);

/* Parse frame & kiểm tra CRC; trả về true nếu hợp lệ.
 * Nếu true: điền out_type / out_payload / out_plen. */
bool proto_parse(const uint8_t *frame,
                 uint16_t len,
                 uint16_t *out_type,
                 const uint8_t **out_payload,
                 uint16_t *out_plen);


#endif /* FRAME_PROTO_FRAME_H_ */
