#include "uart_comm.h"
#include "rsi_debug.h"
#include "string.h"

// Macro to enable/disable mock test
#define MOCK_TEST_UART 0

void mock_test_uart(void);

void app_init(void)
{
  uart_init();
}

/***************************************************************************/ /**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
#if MOCK_TEST_UART
  mock_test_uart();
#endif /*MOCK_TEST_UART*/
}

#if MOCK_TEST_UART
void mock_test_uart(void)
{
  // 1. Test uart_read_byte
  uint8_t received_data;
  uart_read_byte(&received_data, osWaitForever);


  // 2. Test uart_write_bytes
  static bool sent = true;
  if (sent) {
    uint8_t msg[] = "Hello from UART!\r\n";
    uint32_t len = strlen((char*)msg);
    uart_write_bytes(msg, len, osWaitForever);
    sent = false; // Stop sending
  }
}
#endif /*MOCK_TEST_UART*/
