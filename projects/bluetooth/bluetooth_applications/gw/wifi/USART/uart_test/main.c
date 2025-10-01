// ======================= Includes =======================
#include "sl_component_catalog.h"
#include "sl_main_init.h"

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif

#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_main_kernel.h"
#else
#include "sl_main_process_action.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "sl_si91x_usart.h"
#include "rsi_debug.h"

#define UNUSED(x) (void)(x) // Đây là để tránh cảnh báo unused variable
#define LOG_UART_FRAMES 1

// ========================= TYPES =========================
// Ping-Pong kiểm tra kết nối
#define UART_TYPE_PING               0x0210u
#define UART_TYPE_PING_ACK           0x0211u

// ---- Version / Health / Lỗi chung ----
#define UART_TYPE_GET_VERSION        0x0212u
#define UART_TYPE_VERSION_RESP       0x0213u
#define UART_TYPE_HEARTBEAT          0x0214u
#define UART_TYPE_ERROR              0x021Fu

// ---- Wi-Fi ----
#define UART_TYPE_WIFI_REQUEST       0x0230u
#define UART_TYPE_WIFI_RESPONSE      0x0231u
#define UART_TYPE_WIFI_SCAN_REQ      0x0232u
#define UART_TYPE_WIFI_SCAN_RESP     0x0233u
#define UART_TYPE_WIFI_CONNECT_REQ   0x0234u
#define UART_TYPE_WIFI_CONNECT_RES   0x0235u
#define UART_TYPE_WIFI_STATUS_REQ    0x0236u
#define UART_TYPE_WIFI_STATUS_RESP   0x0237u
#define UART_TYPE_WIFI_CRED_SET      0x0238u
#define UART_TYPE_WIFI_CRED_ACK      0x0239u
#define UART_TYPE_IP_INFO            0x023Au

// ---- MQTT / Cloud bridge ----
#define UART_TYPE_MQTT_PUB           0x0248u
#define UART_TYPE_MQTT_PUB_ACK       0x0249u
#define UART_TYPE_MQTT_CFG_SET       0x024Au
#define UART_TYPE_MQTT_CFG_ACK       0x024Bu
#define UART_TYPE_MQTT_STATUS        0x024Cu

// ---- Sensor / Parameters ----
#define UART_TYPE_SENSOR_REPORT      0x0240u
#define UART_TYPE_SENSOR_READ_REQ    0x0241u
#define UART_TYPE_SENSOR_READ_RESP   0x0242u
#define UART_TYPE_PARAM_SET_REQ      0x0243u
#define UART_TYPE_PARAM_SET_ACK      0x0244u

// ---- OTA ----
#define UART_TYPE_OTA_QUERY_REQ      0x0250u
#define UART_TYPE_OTA_QUERY_RESP     0x0251u
#define UART_TYPE_OTA_START          0x0252u
#define UART_TYPE_OTA_CHUNK          0x0253u
#define UART_TYPE_OTA_CHUNK_ACK      0x0254u
#define UART_TYPE_OTA_COMPLETE       0x0255u
#define UART_TYPE_OTA_RESULT         0x0256u

// ---- Multi-node mgmt ----
#define UART_TYPE_NODE_LIST_REQ      0x0260u
#define UART_TYPE_NODE_LIST_RESP     0x0261u
#define UART_TYPE_NODE_IDENTIFY      0x0262u

// ---- Logging / Diagnostics ----
#define UART_TYPE_LOG                0x0270u
#define UART_TYPE_STATS_REQ          0x0271u
#define UART_TYPE_STATS_RESP         0x0272u

// ==== Mã lỗi cho UART_TYPE_ERROR ====
enum {
  ERR_NONE               = 0,
  ERR_BAD_FRAME          = 1,
  ERR_CRC                = 2,
  ERR_UNSUPPORTED_TYPE   = 3,
  ERR_BUSY               = 4,
  ERR_TIMEOUT            = 5,
  ERR_NO_MEM             = 6,
  ERR_BAD_STATE          = 7
}; // Những mã này là để gửi qua payload của gói ERROR

// ==== Wi-Fi status ====
typedef enum {
  WIFI_DISCONNECTED = 0,
  WIFI_CONNECTING   = 1,
  WIFI_CONNECTED    = 2,
  WIFI_AUTH_FAILED  = 3,
  WIFI_AP_NOT_FOUND = 4,
  WIFI_DHCP_FAILED  = 5
} wifi_status_t; 

// ==== OTA result/status ====
typedef enum {
  OTA_OK               = 0,
  OTA_ERR_HASH_MISMATCH= 1,
  OTA_ERR_STORAGE      = 2,
  OTA_ERR_BAD_OFFSET   = 3,
  OTA_ERR_APPLY_FAILED = 4
} ota_status_t;

// ======================= Config ==========================
#define UART_INSTANCE              USART_0
#define NON_UC_DEFAULT_CONFIG      1

// Protocol markers & lengths
#define SOF                        0xF0u
#define ENDCODE                    0xFFu
#define TAIL                       0x0Fu

#define LEN_SOF                    1u
#define LEN_TYPE                   2u
#define LEN_PLEN                   2u
#define LEN_ENDCODE                1u
#define LEN_CRC                    2u
#define LEN_TAIL                   1u
#define LEN_HEADER                 (LEN_SOF + LEN_TYPE + LEN_PLEN)
#define LEN_FOOTER                 (LEN_ENDCODE + LEN_CRC + LEN_TAIL)

#define PROTO_MAX_FRAME            1024u
#define UART_TX_BUFFER_SIZE        1024u

// RX ring & DMA chunk
#define UART_RX_RING_SIZE          1024u
#define UART_RX_CHUNK              64u

// ================ Debug ===========================
// Bật tắt log frame UART

static inline void log_frame(const char *dir, const uint8_t *f, uint16_t n)
{
#if LOG_UART_FRAMES
  DEBUGOUT("%s (%u): ", dir, (unsigned)n);
  for (uint16_t i = 0; i < n; i++) DEBUGOUT("%02X ", f[i]);
  DEBUGOUT("\n");
#else
  (void)dir; (void)f; (void)n;
#endif
}

// ======================= Globals =========================
static sl_usart_handle_t usart_handle = NULL;

// TX
static uint8_t  uart_tx_buffer[UART_TX_BUFFER_SIZE]; // buffer dựng gói
static volatile bool usart_send_complete = true; // cờ báo đã gửi xong

// RX ring
static uint8_t  rx_ring[UART_RX_RING_SIZE]; // vòng đệm nhận
static volatile uint16_t rx_w = 0, rx_r = 0; // head và tail

static inline bool rb_empty(void){ return rx_w == rx_r; } // 
static inline bool rb_full(void) { return (uint16_t)((rx_w + 1u) % UART_RX_RING_SIZE) == rx_r; }
static inline uint16_t rb_count(void){ return (uint16_t)((rx_w - rx_r + UART_RX_RING_SIZE) % UART_RX_RING_SIZE); }
static inline void rb_put(uint8_t b){ if (!rb_full()){ rx_ring[rx_w] = b; rx_w = (uint16_t)((rx_w + 1u) % UART_RX_RING_SIZE);} }
static inline bool rb_pop(uint8_t *out){ if (rb_empty()) return false; *out = rx_ring[rx_r]; rx_r = (uint16_t)((rx_r + 1u) % UART_RX_RING_SIZE); return true; }

// RX DMA buffer + repost flag
static uint8_t rx_chunk[UART_RX_CHUNK];
static volatile bool rx_repost_needed = false;

// ======================= Byte order & CRC ======================
static inline uint16_t u16_le(const uint8_t lo, const uint8_t hi)
{ return (uint16_t)((uint16_t)lo | ((uint16_t)hi << 8)); }

static inline void put_u16_le(uint8_t *dst, uint16_t value)
{ dst[0]=(uint8_t)(value & 0xFFu); dst[1]=(uint8_t)(value >> 8); }

#define CRC16_POLY 0x1021u
#define CRC16_INIT 0xFFFFu
static uint16_t crc16_ccitt(const uint8_t *data, uint16_t len)
{
  uint16_t crc = CRC16_INIT;
  for (uint16_t i = 0; i < len; i++) {
    crc ^= (uint16_t)((uint16_t)data[i] << 8);
    for (uint8_t j = 0; j < 8; j++) {
      crc = (crc & 0x8000u) ? (uint16_t)((crc << 1) ^ CRC16_POLY) : (uint16_t)(crc << 1);
    }
  }
  return crc;
}

// =================== Packet builder / parser ===================
static uint16_t build_uart_packet(uint8_t *dst, uint16_t dst_cap,
                                  uint16_t type_le, const uint8_t *payload, uint16_t payload_len)
{
  if (!dst) return 0u;
  if (payload_len && !payload) return 0u;
  const uint16_t need = (uint16_t)(LEN_HEADER + payload_len + LEN_FOOTER);
  if (dst_cap < need) return 0u;

  uint16_t idx = 0u;
  dst[idx++] = (uint8_t)SOF;
  put_u16_le(&dst[idx], type_le);           idx += LEN_TYPE;
  put_u16_le(&dst[idx], payload_len);       idx += LEN_PLEN;
  if (payload_len) { memcpy(&dst[idx], payload, payload_len); idx += payload_len; }
  dst[idx++] = (uint8_t)ENDCODE;

  const uint16_t crc = crc16_ccitt(&dst[1], (uint16_t)(LEN_TYPE + LEN_PLEN + payload_len + LEN_ENDCODE));
  put_u16_le(&dst[idx], crc);               idx += LEN_CRC;
  dst[idx++] = (uint8_t)TAIL;
  return idx;
}

static bool parse_uart_frame(const uint8_t *frame, uint16_t len)
{
  if ((!frame) || (len < (LEN_HEADER + LEN_FOOTER))) return false;
  if (frame[0] != (uint8_t)SOF) return false;

  const uint16_t payload_len = u16_le(frame[3], frame[4]);
  const uint16_t expect_len  = (uint16_t)(LEN_HEADER + payload_len + LEN_FOOTER);
  if (len != expect_len) return false;

  const uint16_t endcode_pos = (uint16_t)(LEN_HEADER + payload_len);
  if (frame[endcode_pos] != (uint8_t)ENDCODE) return false;
  if (frame[len - 1u] != (uint8_t)TAIL) return false;

  const uint16_t rx_crc  = u16_le(frame[endcode_pos + 1u], frame[endcode_pos + 2u]);
  const uint16_t cal_crc = crc16_ccitt(&frame[1], (uint16_t)(LEN_TYPE + LEN_PLEN + payload_len + LEN_ENDCODE));
  return (rx_crc == cal_crc);
}

// =================== UART driver (HAL wrappers) =========
static inline bool uart_is_inited(void) { return (usart_handle != NULL); }

static inline sl_status_t uart_send_raw(const uint8_t *data, uint16_t len)
{
  if (!uart_is_inited()) return SL_STATUS_NOT_INITIALIZED;
  if (!data || !len)     return SL_STATUS_INVALID_PARAMETER;
  usart_send_complete = false;
  return sl_si91x_usart_send_data(usart_handle, data, len);
}

static inline void uart_repost_rx_if_needed(void)
{
  if (rx_repost_needed) {
    rx_repost_needed = false;
    (void)sl_si91x_usart_receive_data(usart_handle, rx_chunk, UART_RX_CHUNK);
  }
}

// =================== USART callback =====================
void usart_callback_event(uint32_t event)
{
  switch (event) {
    case SL_USART_EVENT_SEND_COMPLETE:
      usart_send_complete = true;
      break;
    case SL_USART_EVENT_RECEIVE_COMPLETE:
      for (uint16_t i = 0; i < UART_RX_CHUNK; ++i) rb_put(rx_chunk[i]);
      rx_repost_needed = true;
      break;
    default:
      break;
  }
}

// =================== Init ===============================
static void uart_init(void)
{
  sl_status_t status;
  sl_si91x_usart_control_config_t usart_config;
  memset(&usart_config, 0, sizeof(usart_config));
  sl_si91x_usart_control_config_t get_config;

  status = sl_si91x_usart_init(UART_INSTANCE, &usart_handle);
  if (status != SL_STATUS_OK) 
  { DEBUGOUT("sl_si91x_usart_init: %lu\n", status); return; }
  DEBUGOUT("USART initialization is successful \n");

#if NON_UC_DEFAULT_CONFIG
  usart_config.baudrate      = 115200;
  usart_config.mode          = SL_USART_MODE_ASYNCHRONOUS;
  usart_config.parity        = SL_USART_NO_PARITY;
  usart_config.stopbits      = SL_USART_STOP_BITS_1;
  usart_config.hwflowcontrol = SL_USART_FLOW_CONTROL_NONE;
  usart_config.databits      = SL_USART_DATA_BITS_8;
  usart_config.misc_control  = SL_USART_MISC_CONTROL_NONE;
  usart_config.usart_module  = UART_INSTANCE;
  usart_config.config_enable = ENABLE;
  usart_config.synch_mode    = DISABLE;

  status = sl_si91x_usart_set_configuration(usart_handle, &usart_config);
  if (status != SL_STATUS_OK) 
  { DEBUGOUT("set_configuration: %lu\n", status); return; }

  DEBUGOUT("USART ready, Baud=%ld\n", get_config.baudrate);
#endif

  status = sl_si91x_usart_multiple_instance_register_event_callback(UART_INSTANCE, usart_callback_event);
  if (status != SL_STATUS_OK) 
  { DEBUGOUT("register_event_callback: %lu\n", status); return; }
    DEBUGOUT("USART user event callback registered successfully \n");

  // Khởi động nhận chunk đầu tiên
  rx_repost_needed = false;
  (void)sl_si91x_usart_receive_data(usart_handle, rx_chunk, UART_RX_CHUNK);

  sl_si91x_usart_get_configurations(UART_INSTANCE, &get_config);
  DEBUGOUT("USART configuration is successful \n");
}

// =================== RX Assembler (only RX state) =======
typedef enum {
  RX_WAIT_SOF,
  RX_READ_HDR,
  RX_READ_BODY
} rx_state_t;

static rx_state_t rx_state = RX_WAIT_SOF;

static uint8_t  rx_frame[PROTO_MAX_FRAME];
static uint16_t rx_idx = 0;
static uint16_t rx_expect_len = 0;
static uint16_t rx_payload_len = 0;

static inline bool pop_byte(uint8_t *out) { return rb_pop(out); }

// forward declare message router
static void handle_frame_and_route(const uint8_t *frame, uint16_t len);

static void uart_process_rx(void)
{
  uint8_t b;

  uart_repost_rx_if_needed();

  while (rb_count() > 0) {
    switch (rx_state) {
      case RX_WAIT_SOF:
        if (!pop_byte(&b)) return;
        if (b == (uint8_t)SOF) {
          rx_idx = 0;
          rx_frame[rx_idx++] = b;
          rx_state = RX_READ_HDR;
        }
        break;

      case RX_READ_HDR:
        while ((rx_idx < LEN_HEADER) && pop_byte(&b))
        rx_frame[rx_idx++] = b;

        if (rx_idx < LEN_HEADER) return;

        rx_payload_len = u16_le(rx_frame[3], rx_frame[4]);
        rx_expect_len  = (uint16_t)(LEN_HEADER + rx_payload_len + LEN_FOOTER);

        if (rx_expect_len > PROTO_MAX_FRAME) {
            rx_state = RX_WAIT_SOF; rx_idx = 0; break;
        }

        rx_state = RX_READ_BODY;
        break;

      case RX_READ_BODY:
        while ((rx_idx < rx_expect_len) && pop_byte(&b)) rx_frame[rx_idx++] = b;
        if (rx_idx < rx_expect_len) return;

        if (parse_uart_frame(rx_frame, rx_expect_len)) {
            const uint16_t type = u16_le(rx_frame[1], rx_frame[2]);
            const uint16_t plen = u16_le(rx_frame[3], rx_frame[4]);
            DEBUGOUT("RX OK TYPE=0x%04X PLEN=%u\n", type, plen);
            log_frame("RX", rx_frame, rx_expect_len);      // <<< in khung RX đầy đủ
          handle_frame_and_route(rx_frame, rx_expect_len);
        } else {
          DEBUGOUT("Invalid frame\n");
        }
        rx_state = RX_WAIT_SOF; rx_idx = 0;
        break;
    }
  }
}

// =================== Public send API =====================
static sl_status_t uart_send_packet(uint16_t type_le, const uint8_t *payload, uint16_t payload_len)
{
  const uint16_t n = build_uart_packet(uart_tx_buffer, UART_TX_BUFFER_SIZE, type_le, payload, payload_len);
  if (n == 0u) return SL_STATUS_ALLOCATION_FAILED;
  log_frame("TX", uart_tx_buffer, n);    
  return uart_send_raw(uart_tx_buffer, n);
}

// =================== Message Handlers ====================
typedef void (*uart_msg_handler_t)
(uint16_t type, const uint8_t *payload, uint16_t len);

typedef struct { 
  uint16_t type; 
  uart_msg_handler_t handler; 
} uart_msg_route_t;

// Ping
static void on_ping(uint16_t type, const uint8_t *payload, uint16_t len)
{
  (void)type; (void)payload; (void)len;
  static const uint8_t pong[] = "PONG";
  (void)uart_send_packet(UART_TYPE_PING_ACK, pong, sizeof(pong));
}

// Version
static void on_get_version(uint16_t type, const uint8_t *pl, uint16_t len)
{
  (void)type; (void)pl; (void)len;
  static const uint8_t ver[] = "917-1.0.0|build-1234";
  (void)uart_send_packet(UART_TYPE_VERSION_RESP, ver, (uint16_t)sizeof(ver));
}

// Wi-Fi
static void on_wifi_scan(uint16_t type, const uint8_t *payload, uint16_t len)
{
  (void)type; (void)payload; (void)len;
  uint8_t resp[] = {
    0x02,
    0x06,'R','o','u','t','e','r', 0xB4, 0x02,
    0x04,'H','o','m','e',        0xC8, 0x01
  };
  (void)uart_send_packet(UART_TYPE_WIFI_SCAN_RESP, resp, sizeof(resp));
}

static void on_wifi_status(uint16_t type, const uint8_t *payload, uint16_t len)
{
  (void)type; (void)payload; (void)len;
  uint8_t resp[] = { (uint8_t)WIFI_CONNECTED, (uint8_t)(-50 & 0xFF), 192,168,1,50 };
  (void)uart_send_packet(UART_TYPE_WIFI_STATUS_RESP, resp, sizeof(resp));
}

static void on_wifi_connect(uint16_t type, const uint8_t *payload, uint16_t len)
{
  (void)type; (void)payload; (void)len;   // <<< thêm dòng này

  uint8_t status = (uint8_t)WIFI_CONNECTING;
  (void)uart_send_packet(UART_TYPE_WIFI_CONNECT_RES, &status, 1);
  // TODO: parse SSID/PSK từ payload và thực hiện connect thật sự
  status = (uint8_t)WIFI_CONNECTED;
  (void)uart_send_packet(UART_TYPE_WIFI_CONNECT_RES, &status, 1);
}
  

// MQTT
static void on_mqtt_pub(uint16_t type, const uint8_t *payload, uint16_t len)
{
  (void)type;
  if (len < 1) return;
  uint8_t topic_len = payload[0];
  if (len < (1 + topic_len)) return;
  const uint8_t ack[] = { 0x01, 0x00 }; // msg_id=1, success
  (void)uart_send_packet(UART_TYPE_MQTT_PUB_ACK, ack, sizeof(ack));
}

// Node
static void on_node_list(uint16_t type, const uint8_t *pl, uint16_t len)
{
  (void)type; (void)pl; (void)len;
  uint8_t resp[] = { 0x02, 0x34,0x12, 0x78,0x56 }; // 2 nodes: 0x1234, 0x5678
  (void)uart_send_packet(UART_TYPE_NODE_LIST_RESP, resp, sizeof(resp));
}

// Stats
static void on_stats_req(uint16_t type, const uint8_t *pl, uint16_t len)
{
  (void)type; (void)pl; (void)len;
  uint8_t resp[] = { 0x10,0x00,0x00,0x00, 25, 0x20,0x03 }; // uptime=16s, 25%, 800kB
  (void)uart_send_packet(UART_TYPE_STATS_RESP, resp, sizeof(resp));
}

// Router
static const uart_msg_route_t routes[] = {
  { UART_TYPE_PING,             on_ping },
  { UART_TYPE_GET_VERSION,      on_get_version },
  { UART_TYPE_WIFI_SCAN_REQ,    on_wifi_scan },
  { UART_TYPE_WIFI_STATUS_REQ,  on_wifi_status },
  { UART_TYPE_WIFI_CONNECT_REQ, on_wifi_connect },
  { UART_TYPE_MQTT_PUB,         on_mqtt_pub },
  { UART_TYPE_NODE_LIST_REQ,    on_node_list },
  { UART_TYPE_STATS_REQ,        on_stats_req },
};
#define ROUTES_CNT (sizeof(routes)/sizeof(routes[0]))

static void handle_frame_and_route(const uint8_t *frame, uint16_t len)
{

  (void)len;

  const uint16_t type        = u16_le(frame[1], frame[2]);
  const uint16_t payload_len = u16_le(frame[3], frame[4]);
  const uint8_t *payload     = &frame[LEN_HEADER];

  for (size_t i = 0; i < ROUTES_CNT; ++i) {
    if (routes[i].type == type) { routes[i].handler(type, payload, payload_len); return; }
  }

  const uint8_t err[] = {
    (uint8_t)(ERR_UNSUPPORTED_TYPE & 0xFF),
    (uint8_t)(ERR_UNSUPPORTED_TYPE >> 8),
    (uint8_t)(type & 0xFF),
    (uint8_t)(type >> 8)
  };
  (void)uart_send_packet(UART_TYPE_ERROR, err, sizeof(err));
  DEBUGOUT("No handler for type 0x%04X\n", type);
}

// =================== App ================================
void app_init(void)
{
  DEBUGINIT();
  uart_init();

  // Gửi thử 1 gói PING
  static const uint8_t demo_payload[] = { 'P','i','n','g' };
  (void)uart_send_packet(UART_TYPE_PING, demo_payload, sizeof(demo_payload));
}

void app_process_action(void)
{
  uart_process_rx();
}

// =================== main ===============================
int main(void)
{
  sl_main_init();

#if defined(SL_CATALOG_KERNEL_PRESENT)
  sl_main_kernel_start();
#else
  app_init();
  while (1) {
    sl_main_process_action();
    app_process_action();
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
    sl_power_manager_sleep();
#endif
  }
#endif
  return 0;
}
