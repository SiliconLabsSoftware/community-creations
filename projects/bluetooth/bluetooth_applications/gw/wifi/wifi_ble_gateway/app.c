#include "uart_comm.h"
#include "rsi_debug.h"
#include "string.h"
#include "decode.h"

// Macro to enable/disable mock test
#define MOCK_TEST_UART 0
#define UART_DECODER_MOCK_TEST 0

static uart_fsm_decoder_t test_decoder_init;


#if UART_DECODER_MOCK_TEST
void mock_test_decoder(void)
{
  // packet "Hello, this is decoder" giống script Python
  uint8_t test_packet[] = {
      0xF0, 0x01, 0x00, 0x16, 0x00,
      0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20,
      0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73,
      0x20, 0x64, 0x65, 0x63, 0x6F, 0x64, 0x65, 0x72,
      0xFF,
      0x72, 0x5E,
      0x0F
  };

  for (size_t i = 0; i < sizeof(test_packet); i++) {
      uart_decode_fsm(&test_decoder_init, test_packet[i]);
  }
}
#endif /*MOCK_DECODER_MOCK_TEST*/

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

void app_init(void)
{
//  uart_init();
  uart_fsm_decoder_init(&test_decoder_init);
}

/***************************************************************************/ /**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
#if UART_DECODER_MOCK_TEST
static bool ran = false;
if (!ran) {
  mock_test_decoder();
  ran = true;
}
#endif /*UART_DECODER_MOCK_TEST*/

#if MOCK_TEST_UART
  mock_test_uart();
#endif /*MOCK_TEST_UART*/
}




