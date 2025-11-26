#ifndef SRC_INC_DECODE_H_
#define SRC_INC_DECODE_H_

// Payload types
typedef enum {
    TLV_TEST        = 0x00,
    TLV_MAC_ADDRESS = 0x01,
    TLV_UUID        = 0x02,
    TLV_STATUS      = 0x03,
    TLV_UUID_VALUE  = 0x04,
    TLV_OTA_VALUE   = 0x05,
    TLV_WIFI_CONFIG = 0x06,
    TLV_HELLO       = 0x48,
} tlv_type_t;


// Packet types
typedef enum {
    PKT_TYPE_HELLO         = 0x0001,
    PKT_TYPE_TEST          = 0x0101,
    PKT_TYPE_OTA_REQUEST   = 0x0202,
    PKT_TYPE_OTA_RESPONSE  = 0x0203,
    PKT_TYPE_OTA_DATA      = 0x0204,
    PKT_TYPE_UUID_REQUEST  = 0x0210,
    PKT_TYPE_UUID_RESPONSE = 0x0211,
} uart_packet_type_t;


#define UART_HEADER     0xF0
#define UART_ENDCODE    0xFF
#define UART_TAIL       0x0F
#define UART_MAX_PACKET_LEN 512
#define UART_MAX_PAYLOAD_LEN 256


// UART Packet structure
typedef struct packet {
    uint8_t sof;
    uart_packet_type_t type;
    uint16_t length;
    uint8_t payload[UART_MAX_PAYLOAD_LEN];
    uint8_t endcode;
    uint16_t crc;
    uint8_t tail;
}  uart_packet_t;

// UART Payload structure
typedef struct {
    tlv_type_t type;
    uint16_t length;
    uint8_t  *value;
} payload_t;

// FSM states for UART decoding
typedef enum {
    WAIT_FOR_HEADER,
    READ_TYPE,
    READ_LENGTH,
    READ_PAYLOAD,
    READ_ENDCODE,
    READ_CRC,
    READ_TAIL,
} uart_decode_state_t;

// Decoder structure for Message Queue
typedef struct {
    uart_decode_state_t state;
    uart_packet_t packet;
    uint8_t uart_payload_temp_buffer[UART_MAX_PAYLOAD_LEN];
    uint16_t uart_payload_temp_buffer_index;

    // CRC
    uint16_t calculated_crc;
    uint16_t received_crc;

    // Statistics
    uint32_t total_packets_parsed;
    uint32_t packets_accepted;
    uint32_t packets_rejected;
    uint32_t bad_checksum_count;
    uint32_t bad_length_count;
} uart_fsm_decoder_t;

// Decode error codes
typedef enum {
    UART_DECODE_PASSED             = 0,
    UART_DECODE_ERR_INVALID_TYPE   = 1,
    UART_DECODE_ERR_INVALID_LENGTH = 2,
    UART_DECODE_ERR_INVALID_PAYLOAD= 3,
    UART_DECODE_ERR_INVALID_CRC    = 4,
    UART_DECODE_ERR_INVALID_ENDCODE= 5,
    UART_DECODE_ERR_INVALID_TAIL   = 6,
    UART_DECODE_FAILED             = 7,
} uart_decode_error_t;



// Decoder initialization
bool uart_fsm_decoder_init (uart_fsm_decoder_t *decoder);
// void uart_fsm_decoder_reset (uart_fsm_decoder_t *decoder);


// Validation functions
bool payload_validate(payload_t *payload);
bool packet_type_validate (uart_packet_t *pkt);
bool packet_length_validate (uart_packet_t *pkt);

// Calculate crc16
uint16_t crc16_ccitt(const uint8_t *data, uint16_t length);

// Debug utilities
void uart_print_stats(const uart_fsm_decoder_t *decoder);
void uart_fsm_print_packet(const uart_packet_t *packet);

// States machine for UART decoding
void uart_decode_fsm (uart_fsm_decoder_t *decoder, uint8_t byte);

#endif /* SRC_INC_DECODE_H_ */
