/*
 * comm_types.h
 *
 *  Created on: 17 thg 9, 2025
 *      Author: nguye
 */

#ifndef UART_TYPES_COMM_TYPES_H_
#define UART_TYPES_COMM_TYPES_H_


// UART frame TYPE definitions
#define UART_TYPE_PING             0x0210  // PING request
#define UART_TYPE_PING_ACK         0x0211  // PING response (ACK)

#define UART_TYPE_WIFI_REQUEST     0x0230  // Wi-Fi info request
#define UART_TYPE_WIFI_RESPONSE    0x0231  // Wi-Fi info response (1 byte status)

// TODO: thêm OTA_REQUEST, OTA_RESPONSE, SENSOR_DATA... theo SoW sau này

#endif /* UART_TYPES_COMM_TYPES_H_ */
