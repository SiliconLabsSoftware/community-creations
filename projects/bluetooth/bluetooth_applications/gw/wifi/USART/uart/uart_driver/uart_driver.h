#ifndef UART_DRIVER_UART_DRIVER_H_
#define UART_DRIVER_UART_DRIVER_H_
#include <sl_status.h>
#include <stdbool.h>
#include <stddef.h>
#include "sl_si91x_usart.h"
#include "uart_comm.h"  // Để sử dụng sm_state_t




bool uart_rx_available(void); // Kiểm tra có dữ liệu nhận được không
sl_status_t uart_receive (uint8_t *buffer, size_t length); // Nhận dữ liệu vào buffer

sl_status_t uart_send_raw
(const uint8_t *data, uint16_t len); // Gửi dữ liệu thô qua UART

sl_status_t uart_send_command
(uint16_t type_le, const uint8_t *payload, uint16_t payload_len); 
// Gửi lệnh đã build qua UART


#endif /* UART_DRIVER_UART_DRIVER_H_ */
