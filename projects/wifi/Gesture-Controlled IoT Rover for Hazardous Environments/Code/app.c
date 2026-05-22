/******************************************************************************/
/* app.c - Wi-Fi + ICM40627 + HTTP server example (SIWG917 using sl_http_server) */
/******************************************************************************/

#include "sl_board_configuration.h"
#include "cmsis_os2.h"
#include "sl_wifi.h"
#include "sl_net.h"
#include "sl_http_server.h"
#include <string.h>
#include <stdio.h>
#include "icm40627_example.h"

/********************* Thread Attributes ************************************/
const osThreadAttr_t wifi_thread_attributes = {
  .name       = "wifi_app",
  .stack_size = 3072,
  .priority   = osPriorityLow,
};

const osThreadAttr_t icm_thread_attributes = {
  .name       = "icm_app",
  .stack_size = 1024,
  .priority   = osPriorityLow,
};

/********************* Wi-Fi configuration **********************************/
static const sl_wifi_device_configuration_t wifi_configuration = {
  .boot_option = LOAD_NWP_FW,
  .mac_address = NULL,
  .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
  .boot_config = {
    .oper_mode                  = SL_SI91X_CLIENT_MODE,
    .coex_mode                  = SL_SI91X_WLAN_ONLY_MODE,
    .feature_bit_map            = SL_SI91X_FEAT_SECURITY_OPEN,
    .tcp_ip_feature_bit_map     = (SL_SI91X_TCP_IP_FEAT_DHCPV4_CLIENT),
    .custom_feature_bit_map     = SL_SI91X_CUSTOM_FEAT_EXTENTION_VALID,
    .ext_custom_feature_bit_map = SL_SI91X_EXT_FEAT_XTAL_CLK,
  },
};

/********************* Forward declarations *********************************/
static void wifi_thread_func(void *arg);
static void icm_thread_func(void *arg);
static sl_status_t http_sensor_callback(sl_http_server_t *handle,
                                        sl_http_server_request_t *request);

/********************* Wi-Fi + HTTP server thread ***************************/
static void wifi_thread_func(void *arg)
{
  (void)arg;
  sl_status_t status;

  // Initialize Wi-Fi
  status = sl_net_init(SL_NET_WIFI_CLIENT_INTERFACE, &wifi_configuration, NULL, NULL);
  if (status != SL_STATUS_OK) {
    printf("Wi-Fi init failed: 0x%lx\r\n", status);
    return;
  }
  printf("Wi-Fi Init Success\r\n");

  status = sl_net_up(SL_NET_WIFI_CLIENT_INTERFACE, 0);
  if (status != SL_STATUS_OK) {
    printf("Wi-Fi up failed: 0x%lx\r\n", status);
    return;
  }
  printf("Wi-Fi Connected\r\n");

  // Configure HTTP server
  sl_http_server_t server;
  sl_http_server_handler_t handlers[] = {
    { "/sensor", http_sensor_callback }
  };

  sl_http_server_config_t config = {0};
  config.port = 80;
  config.handlers_list = handlers;
  config.handlers_count = sizeof(handlers)/sizeof(handlers[0]);
  config.default_handler = NULL;
  config.client_idle_time = 30;

  status = sl_http_server_init(&server, &config);
  if (status != SL_STATUS_OK) {
    printf("HTTP server init failed: 0x%lx\r\n", status);
    return;
  }

  status = sl_http_server_start(&server);
  if (status != SL_STATUS_OK) {
    printf("HTTP server start failed: 0x%lx\r\n", status);
    return;
  }

  printf("HTTP Server Running on port 80\r\n");

  while (1) {
    osDelay(1000); // keep thread alive
  }
}

/********************* HTTP /sensor callback *******************************/
static sl_status_t http_sensor_callback(sl_http_server_t *handle,
                                        sl_http_server_request_t *request)
{
  (void)request; // unused for GET
  sl_http_server_response_t response;
  float accel[3], gyro[3], temp;
  char json_buf[256];

  // Read sensor data
  icm40627_example_get_accel(accel);
  icm40627_example_get_gyro(gyro);
  icm40627_example_get_temperature(&temp);

  // Prepare JSON response
  snprintf(json_buf, sizeof(json_buf),
           "{\"ax\":%.2f,\"ay\":%.2f,\"az\":%.2f,"
           "\"gx\":%.2f,\"gy\":%.2f,\"gz\":%.2f,"
           "\"t\":%.2f}",
           accel[0], accel[1], accel[2],
           gyro[0], gyro[1], gyro[2],
           temp);

  // Fill response structure
  response.response_code = SL_HTTP_RESPONSE_OK;
  response.content_type = SL_HTTP_CONTENT_TYPE_APPLICATION_JSON;
  response.data = (uint8_t *)json_buf;
  response.current_data_length = strlen(json_buf);
  response.expected_data_length = strlen(json_buf);
  response.headers = NULL;
  response.header_count = 0;

  // Send response
  return sl_http_server_send_response(handle, &response);
}

/********************* ICM sensor thread ***********************************/
static void icm_thread_func(void *arg)
{
  (void)arg;
  icm40627_example_init();

  while (1) {
    icm40627_example_process_action();
    osDelay(2000);
  }
}

/******************************************************************************/
// Application init - create threads
void app_init(void)
{
  osThreadNew(wifi_thread_func, NULL, &wifi_thread_attributes);
  osThreadNew(icm_thread_func, NULL, &icm_thread_attributes);
}

/******************************************************************************/
// Called when no RTOS present (kept for compatibility)
void app_process_action(void)
{
  // nothing (RTOS used)
}
