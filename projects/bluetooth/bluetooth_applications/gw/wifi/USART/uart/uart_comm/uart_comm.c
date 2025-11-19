#include <frame/proto_frame.h>
#include "uart_comm.h"
#include "sl_si91x_usart.h"
#include "rsi_debug.h"
#include "base_types.h"
#include "log.h"
#include "comm_types.h"
#include "uart_driver.h"

// ======================= Config ==============================
// UART instance & buffer
#define UART_INSTANCE                  USART_0
#define UART_RX_BUFFER_SIZE            256u
#define UART_TX_BUFFER_SIZE            1024u

//  Enable this macro to set the default configurations in non_uc case,
// This is useful when someone don't want to use UC configuration
#define NON_UC_DEFAULT_CONFIG          0

// Feature toggles
#define ENABLE_LOOPBACK_CHECK          1       // So sánh TX==RX nếu peer echo
#define USE_RECEIVE                    1       // Chờ phản hồi sau khi gửi
#define USE_SEND                       0

// ======================= Globals ========================
static sl_usart_handle_t usart_handle;
static sm_state_t current_state = SM_IDLE;

// =============== Local functions prototypes =============
void usart_callback_event(uint32_t event);
static void compare_loop_back_frame(const uint8_t *tx,
                                    uint16_t tx_len,
                                    const uint8_t *rx,
                                    uint16_t rx_len);

// ======================= Locals ========================
/// RX
// RX buffer (gom frame)
static uint8_t  uart_rx_buffer[UART_RX_BUFFER_SIZE];
// Con tro dem so byte da duoc luu vao buffer
static uint16_t uart_rx_index = 0;

/// TX
// TX buffer (frame đã build)
static uint8_t  uart_tx_buffer[UART_TX_BUFFER_SIZE];
// tx_len: Do dai thuc te cua goi tin da build
static uint16_t uart_tx_len = 0;

static volatile bool tx_pending = false;            // Có lệnh đang xếp chờ gửi

// Flags event (nếu sau này muốn dùng interrupt)
volatile boolean_t usart_send_complete = false;
volatile boolean_t usart_transfer_complete = false;
volatile boolean_t usart_receive_complete = false;
volatile boolean_t usart_begin_transmission = true;

// =================== USART callback (optional) ==========
// thêm cờ báo "có byte mới"
volatile bool usart_rx_byte_done = false;

void usart_callback_event(uint32_t event)
{

  switch (event) {
    case SL_USART_EVENT_RECEIVE_COMPLETE:
      usart_receive_complete = true;     // giữ lại nếu bạn đang dùng ở nơi khác
      usart_rx_byte_done     = true;     // dùng cho nhận từng byte
      break;
    case SL_USART_EVENT_SEND_COMPLETE:
      usart_send_complete = true;
      break;
    case SL_USART_EVENT_TRANSFER_COMPLETE:
      usart_transfer_complete = true;
      break;
  }
}

// =================== Init ===============================
void uart_init(void)
{
  sl_status_t status;
  sl_si91x_usart_control_config_t usart_config;
  sl_si91x_usart_control_config_t get_config;

#if NON_UC_DEFAULT_CONFIG
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
#endif

  do {
  status = sl_si91x_usart_init(UART_INSTANCE, &usart_handle);
  if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_usart_initialize: Error Code : %lu \n", status);
    return;
  }
  DEBUGOUT("USART initialization is successful \n");

  // Cấu hình từ UC/PinTool (baud, pins, parity…)
  status = sl_si91x_usart_set_configuration(usart_handle, &usart_config);
  if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_usart_set_configuration: Error Code : %lu \n",
               status);
    return;
  }
  printf ("Handle: %p\n", &usart_handle);
  DEBUGOUT("USART configuration is successful \n");


  status = sl_si91x_usart_multiple_instance_register_event_callback
          (USART_0, usart_callback_event);
      if (status != SL_STATUS_OK) {
        DEBUGOUT("sl_si91x_usart_register_event_callback: Error Code : %lu \n", status);
        break;
      }
      DEBUGOUT("USART user event callback registered successfully \n");

      // Log ra baud rate
      sl_si91x_usart_get_configurations(USART_0, &get_config);
      DEBUGOUT("Baud Rate = %ld \n", get_config.baudrate);


//  current_state = SM_SEND; // Cho phép demo TX sau app_init()
  } while (false);
}

// =================== Process (state machine) ============
void usart_process_action(void)
{
  sl_status_t status;
  switch (current_state) {
    case SM_SEND:
      if (tx_pending && usart_begin_transmission) {

          //  In ra frame chuan bi gui
          hexdump("TX frame", uart_tx_buffer, uart_tx_len);

          // Ham nay chi send frame co san
          // Khong tu build frame
          status = uart_send_raw(uart_tx_buffer, uart_tx_len);
          if (status != SL_STATUS_OK) {
            // If it fails to execute the API, it will not execute rest of the things
            DEBUGOUT("sl_si91x_usart_send_data: Error Code : %lu \n", status);
            current_state = SM_IDLE;
            break;
          }

          else if (status == SL_STATUS_OK) {
              // Dat co de tranh goi API nhieu lan
              usart_begin_transmission = false;
          }
      }
        // In ra so byte da truyen
        DEBUGOUT("TX frame pushed: %u bytes\n", uart_tx_len);
        tx_pending = false;
        usart_send_complete = false;


        // Chuyen sang state send neu enable
        if (USE_RECEIVE) {
            // Quay lai san sang nhan
            current_state = SM_RECEIVE;
            usart_begin_transmission = true;
            // Chuẩn bị gom phản hồi
            // Reset con tro index = 0, tuc la ko co byte nao trong mang
            uart_rx_index = 0u;
            // Set tat ca cac gia tri trong buffer = 0
            memset(uart_rx_buffer, 0, sizeof(uart_rx_buffer));
            break;
        }

            DEBUGOUT("USART send completed successfully \n");
            // Current mode is set to complete
            current_state = SM_IDLE;
            break;


    case SM_RECEIVE: {
      // 1. Khởi phát nhận một lần khi vừa chuyển sang SM_RECEIVE
      if (usart_begin_transmission) {
        uart_rx_index = 0u;
        memset(uart_rx_buffer, 0, sizeof(uart_rx_buffer));

        status = sl_si91x_usart_receive_data(usart_handle,
                                             uart_rx_buffer,
                                             sizeof(uart_rx_buffer));
        if (status != SL_STATUS_OK) {
          DEBUGOUT("sl_si91x_usart_receive_data failed: %lu\n", status);
          current_state = SM_IDLE;
          break;
        }

        else DEBUGOUT("USART receive started\n");

        // In ra data
        DEBUGOUT("Received data: ");
        for (size_t i = 0; i < sizeof(uart_rx_buffer); i++) {
            DEBUGOUT("%02X ", uart_rx_buffer[i]);
        }
        DEBUGOUT("\n");

        usart_begin_transmission = false;   // tránh gọi lại nhiều lần
      }

//      printf ("%d\n", usart_receive_complete);
      // 2. Khi callback báo xong, xử lý frame
      if (usart_rx_byte_done) {
          usart_rx_byte_done = false;


        // Tìm chiều dài frame (đến TAIL = 0x0F)
        uint16_t frame_len = 0u;
        for (uint16_t i = 0; i < UART_RX_BUFFER_SIZE; i++) {
          if (uart_rx_buffer[i] == (uint8_t)TAIL) {
            frame_len = (uint16_t)(i + 1u);
            break;
          }
        }

        if (frame_len > 0u) {
          hexdump("RX frame", uart_rx_buffer, frame_len);

          uint16_t type;
          const uint8_t *payload;
          uint16_t plen;

          if (proto_parse (uart_rx_buffer, frame_len, &type, &payload, &plen)) {
            // So sánh với frame TX trước đó (nếu có)
            compare_loop_back_frame(uart_tx_buffer, uart_tx_len,
                                    uart_rx_buffer, frame_len);
            DEBUGOUT("Frame parsed successfully\n");
          } else {
            DEBUGOUT("Frame parsing failed\n");
          }


        } else {
          DEBUGOUT("TAIL not found in received buffer\n");
          hexdump("RX raw", uart_rx_buffer, UART_RX_BUFFER_SIZE);
        }

    #if USE_SEND
        current_state = SM_SEND;
        usart_begin_transmission = true;
        DEBUGOUT("Receive completed -> switch to SEND\n");
    #else
        // Giữ nguyên ở RECEIVE để tiếp tục lắng nghe frame mới
        current_state = SM_RECEIVE;
        usart_begin_transmission = true; // cho phép vòng nhận kế tiếp
        DEBUGOUT("Receive completed -> stay in RECEIVE\n");
    #endif
      }
      break;
    }

    case SM_IDLE:
    default:
      // Không làm gì; khi app gọi uart_send_command(...) sẽ tự chuyển sang SEND
      break;

  }
}

// ============== Optional: compare loopback ==============
static void compare_loop_back_frame(const uint8_t *tx, uint16_t tx_len,
                                    const uint8_t *rx, uint16_t rx_len)
{
#if ENABLE_LOOPBACK_CHECK
  uint16_t data_index = 0;

  // Kiểm tra độ dài trước khi so sánh từng byte
  if (tx_len != rx_len) {
    DEBUGOUT("Loop Back Test Failed: Length mismatch (TX=%u, RX=%u) \n", tx_len, rx_len);
    return;
  }

  // So sánh từng byte giữa buffer gửi và buffer nhận
  for (data_index = 0; data_index < tx_len; data_index++) {
    if (tx[data_index] != rx[data_index]) {
      DEBUGOUT("Loop Back Test Failed at index %u: TX=%02X, RX=%02X \n",
               data_index, tx[data_index], rx[data_index]);
      return;
    }
  }

  // Nếu toàn bộ dữ liệu giống nhau
  DEBUGOUT("Data comparison successful, Loop Back Test Passed \n");

#else
  // Nếu kiểm tra loopback bị disable, tránh cảnh báo compiler
  (void)tx;
  (void)tx_len;
  (void)rx;
  (void)rx_len;
#endif
}

