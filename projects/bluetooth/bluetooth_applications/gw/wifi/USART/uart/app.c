/***************************************************************************/ /**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************/ /**
 * Initialize application.
 ******************************************************************************/
#include <frame/proto_frame.h>
#include "uart_comm.h"
#include "uart_driver.h"
#include "comm_types.h"
void app_init(void)
{
  uart_init();

  static const uint8_t demo_payload[] = { 'P','i','n','g' };
  uart_send_command(UART_TYPE_PING, demo_payload, sizeof(demo_payload));
}

/***************************************************************************/ /**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  usart_process_action();
}
