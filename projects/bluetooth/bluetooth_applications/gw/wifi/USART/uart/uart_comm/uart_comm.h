#ifndef UART_COMM_UART_COMM_H_
#define UART_COMM_UART_COMM_H_

#include <stdint.h>
#include <stdbool.h>
#include "sl_si91x_usart.h"     


// State machine
typedef enum {
  SM_SEND,
  SM_RECEIVE,
  SM_IDLE
} sm_state_t;

void uart_init(void);
void usart_process_action(void);


#endif /* UART_COMM_UART_COMM_H_ */
