#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sl_status.h"
#include "sl_si91x_usart.h"
#include "rsi_debug.h"

#include "packet_proto.h"
#include "log_frame.h"
#include "message_handler_router.h"
#include "crc16.h"
#include "uart_comm.h"
#include "cmsis_os2.h"

/* ======================= Config ========================== */
#define UART_INSTANCE              USART_0
#define NON_UC_DEFAULT_CONFIG      1

/* ======================= Globals ========================= */
static sl_usart_handle_t usart_handle = NULL;
static uint8_t rx_chunk;

/* Queues + thread */
static osMessageQueueId_t rx_mq = NULL; 

/* ======================= USART callback ================== */
void usart_callback_event(uint32_t event)
{
  switch (event) {
    case SL_USART_EVENT_SEND_COMPLETE: {
      break;
    }

    case SL_USART_EVENT_RECEIVE_COMPLETE: {
      osMessageQueuePut(rx_mq, (void *)&rx_chunk, 0U, 0U);
      sl_si91x_usart_receive_data(usart_handle, &rx_chunk, UART_RX_CHUNK);
      break;
    }

    default:
      break;
  }
}

/* ======================= Driver ==================== */
osStatus_t uart_read_byte(uint8_t *b, uint32_t timeout_ms)
{
  return osMessageQueueGet(rx_mq, b, NULL, timeout_ms);
}

/* ======================= Init ============================ */
void uart_init(void)
{
  sl_status_t status;
  sl_si91x_usart_control_config_t usart_config;
  sl_si91x_usart_control_config_t get_config;


  rx_mq = osMessageQueueNew(UART_RX_MQ_LEN, sizeof(uint8_t), NULL);
  if ((rx_mq == NULL)){
    DEBUGOUT("UART: create RX message queue failed\n");
    return;
  } else {
    DEBUGOUT("UART: RX message queue created\n");
  }

  memset(&usart_config, 0, sizeof(usart_config));
  usart_config.baudrate      = 115200;
  usart_config.mode          = SL_USART_MODE_ASYNCHRONOUS;
  usart_config.parity        = SL_USART_NO_PARITY;
  usart_config.stopbits      = SL_USART_STOP_BITS_1;
  usart_config.hwflowcontrol = SL_USART_FLOW_CONTROL_NONE;
  usart_config.databits      = SL_USART_DATA_BITS_8;
  usart_config.misc_control  = SL_USART_MISC_CONTROL_NONE;
  usart_config.usart_module  = USART_0;
  usart_config.config_enable = ENABLE;
  usart_config.synch_mode    = DISABLE;

  status = sl_si91x_usart_init(USART_0, &usart_handle);
  if (status != SL_STATUS_OK) {
    DEBUGOUT("sl_si91x_usart_init: %lu\n", status);
    return;
  }
  DEBUGOUT("USART initialization is successful\n");

  status = sl_si91x_usart_set_configuration(usart_handle, &usart_config);
  if (status != SL_STATUS_OK) {
    DEBUGOUT("sl_si91x_usart_set_configuration: %lu\n", status);
    return;
  }
  DEBUGOUT("USART configuration is successful\n");

  status = sl_si91x_usart_multiple_instance_register_event_callback(USART_0, usart_callback_event);                                                                 
  if (status != SL_STATUS_OK) {
    DEBUGOUT("register_event_callback: %lu\n", status);
    return;
  }
  DEBUGOUT("USART user event callback registered successfully\n");


  // Start to receive first byte
  status = sl_si91x_usart_receive_data(usart_handle, &rx_chunk, 1);
  if (status != SL_STATUS_OK) {
  DEBUGOUT("Initial receive failed: %lu\n", status);
  return;
  } 

  sl_si91x_usart_get_configurations(USART_0, &get_config);
  DEBUGOUT("Baud Rate = %ld\n", get_config.baudrate);
}
