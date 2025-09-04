#ifndef OTA_CLIENT_H
#define OTA_CLIENT_H

#include "sl_bt_api.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "app_log.h"
#include "app_assert.h"
// ====== Log & Error ======
#define ERROR_EXIT(...) do { app_log(__VA_ARGS__); exit(EXIT_FAILURE); } while (0)

// ====== OTA State Machine ======
typedef enum {
  OTA_INIT = 0,
  OTA_READ_OTA_DATA_PROPERTIES,
  OTA_READ_APPLICATION_VERSION,
  OTA_BEGIN,
  OTA_UPLOAD_WITHOUT_RSP,
  OTA_UPLOAD_WITH_RSP,
  OTA_END,
  OTA_RESET_TO_DFU,
  IDLE
} ota_state_t;
// ====== Public APIs ======

void ota_change_state(ota_state_t new_state);
// ====== TODO: bạn hiện thực riêng các hàm này ======
int  dfu_read_size(void);
void sync_boot(void);
bool match_target(const sl_bt_evt_scanner_legacy_advertisement_report_t *rpt);
void parse_application_version(const uint8_t* p, uint16_t n);
void parse_ota_data_properties(const uint8_t* p, uint16_t n);
void send_dfu_block(void);
void send_dfu_packet_with_confirmation(void);
void init_ota_client(uint8_t connection, uint16_t control_char, uint16_t data_char, uint16_t app_ver_char);
uint8_t ota_client_get_connection(void);
ota_states ota_client_get_state(void);
#endif // OTA_CLIENT_H
