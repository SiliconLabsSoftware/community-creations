#include "uart_driver.h"
#include "sl_status.h"
#include "uart_comm.h"
#include "stdbool.h"
#include "stddef.h"
#include "proto_frame.h"
#include "rsi_debug.h"

// =================== UART driver  =================
inline bool uart_rx_available(void)
{
  return (sl_si91x_usart_get_rx_data_count(usart_handle) > 0u);
}

static inline sl_status_t uart_receive (uint8_t *buffer, size_t length)
{
  if (buffer == NULL || length == 0) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return sl_si91x_usart_receive_data(usart_handle, buffer, length);
}

// Wrapper HAL
inline sl_status_t uart_send_raw(const uint8_t *data, uint16_t len)
{
  if ((data == NULL) || (len == 0u)) {
    return SL_STATUS_INVALID_PARAMETER; // hoặc một mã lỗi phù hợp
  }

  return sl_si91x_usart_send_data(usart_handle, data, len);
}

// =================== Public TX API ======================
sl_status_t uart_send_command(uint16_t type_le,
                              const uint8_t *payload,
                              uint16_t payload_len)
{
  // Dang cho gui
  if (tx_pending) {
    return SL_STATUS_BUSY;
  }

  const uint16_t n = proto_build(uart_tx_buffer,
                                       UART_TX_BUFFER_SIZE,
                                       type_le,
                                       payload,
                                       payload_len);
  if (n == 0u) {
    return SL_STATUS_ALLOCATION_FAILED;
  }

  uart_tx_len = n;
  tx_pending = true;
  usart_begin_transmission = true;
  current_state = SM_SEND;

  DEBUGOUT("Queued TX frame: TYPE=0x%04X, PAYLOAD_LEN=%u, FRAME_LEN=%u\n",
           type_le, payload_len, uart_tx_len);
  return SL_STATUS_OK;
}
