#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sl_status.h"
#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------- Configs có thể chỉnh --------- */
/* Thời gian chờ nhận mỗi byte ở từng state (ms) */
#ifndef UART_RX_BYTE_TIMEOUT_MS
#define UART_RX_BYTE_TIMEOUT_MS   50u 
#endif

/* Kích thước tối đa 1 frame (header + payload + footer) */
#ifndef UART_RX_MAX_FRAME
#define UART_RX_MAX_FRAME         1024u
#endif

/* Độ dài queue theo byte */
#ifndef UART_RX_MQ_LEN
#define UART_RX_MQ_LEN            1024u
#endif
#ifndef UART_TX_MQ_LEN
#define UART_TX_MQ_LEN            1024u
#endif

/* DMA chunk từ HAL */
#ifndef UART_RX_CHUNK
#define UART_RX_CHUNK             1u
#endif

 /* TX buffer để build packet */
 #ifndef UART_TX_BUFFER_SIZE
 #define UART_TX_BUFFER_SIZE       1024u
 #endif

// /* --------- Public API --------- */
 void        uart_init(void);
// sl_status_t uart_send_raw(const uint8_t *data, uint16_t len);
// sl_status_t uart_send_packet(uint16_t type_le, const uint8_t *payload, uint16_t payload_len);
// void        uart_flush_rx(void);
// void        uart_flush_tx(void);
// bool        uart_is_inited(void);

osStatus_t uart_read_byte(uint8_t *b, uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif
