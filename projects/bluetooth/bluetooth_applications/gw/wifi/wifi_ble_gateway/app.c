#include "uart_comm.h"
#include "rsi_debug.h"
void app_init(void)
{
   uart_init();
}


void app_process_action(void)
{
  uint8_t b;
  uart_read_byte(&b, osWaitForever);
  DEBUGOUT ("%c  \r\n", b);
}
