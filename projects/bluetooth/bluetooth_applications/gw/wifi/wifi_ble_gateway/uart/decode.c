#include "uart_comm.h"
#include "rsi_debug.h"
#include "decode.h"
#include "crc16.h"
#include "string.h"


// ==================== Initialization of the decoder =======================
bool uart_fsm_decoder_init(uart_fsm_decoder_t *decoder) {

    if (!decoder) {
        DEBUGOUT ("Decoder pointer is NULL\n");
        return false; // check NULL
    }

    decoder->state = WAIT_FOR_HEADER;

    memset(&decoder->packet, 0, sizeof(uart_packet_t));
    memset(decoder->uart_payload_temp_buffer, 0, sizeof(decoder->uart_payload_temp_buffer));
    decoder->uart_payload_temp_buffer_index = 0;

    decoder->calculated_crc = 0;
    decoder->received_crc = 0;

    decoder->total_packets_parsed = 0;
    decoder->packets_accepted = 0;
    decoder->packets_rejected = 0;
    decoder->bad_checksum_count = 0;
    decoder->bad_length_count = 0;

    DEBUGOUT ("UART FSM Decoder initialized successfully\n");
    return true;
}

// ============================= DEBUG FUNCTIONS =============================
void uart_fsm_print_stats(const uart_fsm_decoder_t *decoder) {
   printf("\n=== PACKET STATISTICS ===\n");
   printf("Total packets parsed: %lu\n", decoder->total_packets_parsed);
   printf("Packets accepted: %lu\n", decoder->packets_accepted);
   printf("Packets rejected: %lu\n", decoder->packets_rejected);
   printf("  - Bad checksum: %lu\n", decoder->bad_checksum_count);
   printf("  - Bad length: %lu\n", decoder->bad_length_count);
   printf("========================\n");
}

void uart_fsm_print_packet(const uart_packet_t *packet){
    printf("\n--- PACKET DETAILS ---\n");
    printf("SOF: 0x%02X\n", packet->sof);
    printf("Type: 0x%04X\n", packet->type);
    printf("Length: %u\n", packet->length);
    printf("Payload: ");
    for (uint16_t i = 0; i < packet->length; i++) {
        printf("%02X ", packet->payload[i]);
    }
    printf("\nEndcode: 0x%02X\n", packet->endcode);
    printf("CRC: 0x%04X\n", packet->crc);
    printf("Tail: 0x%02X\n", packet->tail);
    printf("----------------------\n");
}

// ============================= Validation functions =======================
bool packet_type_validate(uart_packet_t *pkt) {
    switch (pkt->type) {
        case PKT_TYPE_HELLO:
            break;

        default:
            DEBUGOUT("Packet type is invalid: 0x%04X\n", pkt->type);
            return false;
    }
    return true;
}

bool packet_length_validate (uart_packet_t *pkt){
    if (pkt->length == 0 || pkt->length > UART_MAX_PACKET_LEN){
        DEBUGOUT ("Packet length is invalid: %u \n", pkt->length);
        return false;
    }
    // DEBUGOUT("Packet LENGTH: %u bytes\n", pkt->length);
    return true;
}

bool payload_validate (payload_t *payload){

    if (!payload) {
        DEBUGOUT("Payload is NULL\n");
        return false;
    }

    // Payload type validate
    switch (payload->type) {
        case TLV_HELLO:
            DEBUGOUT("Payload TYPE: TLV_HELLO (0x%02X)\n", payload->type);
            break;
        default:
            DEBUGOUT("Unknown TLV TYPE: 0x%02X\n", payload->type);
            return false;
    }

    // Payload length validate
    if (payload->length == 0 || payload->length > UART_MAX_PAYLOAD_LEN){
        DEBUGOUT ("Payload length is invalid: %u\n", payload->length);
        return false;
    }

    // Payload value validate
    if (!payload->value){
        DEBUGOUT ("Payload value is NULL\n");
        return false;
    }

    DEBUGOUT("Payload VALUE: ");
    for (uint16_t i = 0; i < payload->length; i++) {
        DEBUGOUT("%02X ", payload->value[i]);
    }
    DEBUGOUT("\n");

    return true;
}

// ==================================== Decode FSM =====================================
void uart_decode_fsm (uart_fsm_decoder_t *decoder, uint8_t byte){
    switch (decoder->state) {
        case WAIT_FOR_HEADER:
            if (byte == UART_HEADER) {
                decoder->packet.sof = byte;
                // Print header
                DEBUGOUT ("Received HEADER: 0x%02X\n", byte);
                decoder->state = READ_TYPE;
                DEBUGOUT("State: WAIT_FOR_HEADER -> READ_TYPE\n");
            }
            break;

        case READ_TYPE: {
            if (decoder->uart_payload_temp_buffer_index == 0) {
                decoder->packet.type  = (uint16_t)byte;
            } else if (decoder->uart_payload_temp_buffer_index == 1) {
                decoder->packet.type |= (uint16_t)byte << 8;
            }
            decoder->uart_payload_temp_buffer_index++;

            // Print when both bytes of TYPE are read
            if (decoder->uart_payload_temp_buffer_index == 2) {
                DEBUGOUT("Received PACKET TYPE: 0x%04X\n", decoder->packet.type);

                // Validate type
                if (!packet_type_validate(&decoder->packet)) {
                    DEBUGOUT("Invalid type. Resetting to WAIT_FOR_HEADER\n");
                    decoder->state         = WAIT_FOR_HEADER;
                    decoder->uart_payload_temp_buffer_index = 0;
                    break;
                }

                // Print individual bytes of TYPE
                uint8_t type_lsb =  (uint8_t)(decoder->packet.type & 0xFF);
                uint8_t type_msb =  (uint8_t)((decoder->packet.type >> 8) & 0xFF);
                DEBUGOUT("Received TYPE bytes: LSB=0x%02X, MSB=0x%02X\n", type_lsb, type_msb);

                // State transition
                decoder->state         = READ_LENGTH;
                decoder->uart_payload_temp_buffer_index = 0;
                DEBUGOUT("State: READ_TYPE -> READ_LENGTH\n");
            }
            break;
        }

        case READ_LENGTH: {
            if (decoder->uart_payload_temp_buffer_index == 0) {
                decoder->packet.length = (uint16_t)byte;
            } else if (decoder->uart_payload_temp_buffer_index == 1) {
                decoder->packet.length |= ((uint16_t)byte << 8);
            }
            decoder->uart_payload_temp_buffer_index++;

            // 2bytes read → validate length
            if (decoder->uart_payload_temp_buffer_index == 2) {

                uint8_t len_LSB = (uint8_t)(decoder->packet.length & 0xFF);
                uint8_t len_MSB = (uint8_t)((decoder->packet.length >> 8) & 0xFF);

                DEBUGOUT("Packet LENGTH raw = %u (0x%04X)\n",
                        decoder->packet.length,
                        decoder->packet.length);

                DEBUGOUT("Received LENGTH bytes: LSB=0x%02X, MSB=0x%02X\n",
                        len_LSB, len_MSB);

                // Validate length
                if (!packet_length_validate(&decoder->packet)) {
                    decoder->state = WAIT_FOR_HEADER;
                    decoder->bad_length_count++;
                    DEBUGOUT("Invalid length. Resetting to WAIT_FOR_HEADER\n");
                    decoder->uart_payload_temp_buffer_index = 0;
                    break;
                }

                // Transit to READ_PAYLOAD
                decoder->state         = READ_PAYLOAD;
                decoder->uart_payload_temp_buffer_index = 0;
                DEBUGOUT("State: READ_LENGTH -> READ_PAYLOAD\n");
            }

            break;
        }

        case READ_PAYLOAD: {
            // Write byte to payload buffer
            if (decoder->uart_payload_temp_buffer_index < UART_MAX_PACKET_LEN) {
                decoder->uart_payload_temp_buffer[decoder->uart_payload_temp_buffer_index++] = byte;
            } else {
                DEBUGOUT("Payload buffer overflow. Resetting to WAIT_FOR_HEADER\n");
                decoder->state         = WAIT_FOR_HEADER;
                decoder->uart_payload_temp_buffer_index = 0;
                decoder->packets_rejected++;
                break;
            }

            // If received full payload
            if (decoder->uart_payload_temp_buffer_index >= decoder->packet.length) {

                // Temp payload struct to validate
                payload_t pl;
                pl.type   = decoder->uart_payload_temp_buffer[0];
                pl.length = decoder->packet.length;
                pl.value  = decoder->uart_payload_temp_buffer;

                // Validate payload
                if (!payload_validate(&pl)) {
                    DEBUGOUT("Payload invalid. Resetting to WAIT_FOR_HEADER\n");
                    decoder->state         = WAIT_FOR_HEADER;
                    decoder->uart_payload_temp_buffer_index = 0;
                    decoder->packets_rejected++;
                    break;
                }

                // Copy from temp buffer to packet.payload
                memcpy(decoder->packet.payload,
                    decoder->uart_payload_temp_buffer,
                    decoder->packet.length);


                // State transition to READ_ENDCODE
                decoder->state         = READ_ENDCODE;
                decoder->uart_payload_temp_buffer_index = 0;
                DEBUGOUT("State: READ_PAYLOAD -> READ_ENDCODE\n");
            }

            break;
}

        case READ_ENDCODE:
            if (byte == UART_ENDCODE) {
                decoder->packet.endcode = byte;

                // Print endcode
                DEBUGOUT ("Received ENDCODE: 0x%02X\n", byte);

                decoder->state = READ_CRC;
                decoder->uart_payload_temp_buffer_index = 0;
                DEBUGOUT("State: READ_ENDCODE -> READ_CRC\n");
            } else {
                decoder->state = WAIT_FOR_HEADER;
                DEBUGOUT("Invalid endcode. Resetting to WAIT_FOR_HEADER\n");
            }
            break;

        case READ_CRC: {
            if (decoder->uart_payload_temp_buffer_index == 0) {
                decoder->received_crc = (uint16_t)byte;
            } else if (decoder->uart_payload_temp_buffer_index == 1) {
                decoder->received_crc |= (uint16_t)byte << 8;
            }
            decoder->uart_payload_temp_buffer_index++;

            // Received full CRC (2 bytes)
            if (decoder->uart_payload_temp_buffer_index == 2) {

                // ====== Build buffer: TYPE(2) | LENGTH(2) | PAYLOAD(L) | ENDCODE(1) ======
                uint16_t crc_len = 0;
                uint8_t  crc_buf[4 + UART_MAX_PACKET_LEN + 1]; // 2 type + 2 len + payload + 1 endcode

                // Type (LE)
                crc_buf[crc_len++] = (uint8_t)(decoder->packet.type & 0xFF);        // LSB
                crc_buf[crc_len++] = (uint8_t)((decoder->packet.type >> 8) & 0xFF); // MSB

                // Length (LE)
                crc_buf[crc_len++] = (uint8_t)(decoder->packet.length & 0xFF);        // LSB
                crc_buf[crc_len++] = (uint8_t)((decoder->packet.length >> 8) & 0xFF); // MSB

                // Payload
                if (decoder->packet.length > 0) {
                    memcpy(&crc_buf[crc_len],
                        decoder->packet.payload,
                        decoder->packet.length);
                    crc_len += decoder->packet.length;
                }

                // Endcode (1 byte)
                crc_buf[crc_len++] = decoder->packet.endcode;

                // ====== Calculate CRC with the built buffer ======
                decoder->calculated_crc = crc16_ccitt(crc_buf, crc_len);

                DEBUGOUT("Received CRC:    0x%04X\n", decoder->received_crc);
                DEBUGOUT("Calculated CRC:  0x%04X\n", decoder->calculated_crc);

                if (decoder->calculated_crc == decoder->received_crc) {
                    decoder->packet.crc   = decoder->received_crc;
                    decoder->state        = READ_TAIL;
                    decoder->uart_payload_temp_buffer_index  = 0;
                    DEBUGOUT("CRC OK. State: READ_CRC -> READ_TAIL\n");
                } else {
                    decoder->state             = WAIT_FOR_HEADER;
                    decoder->bad_checksum_count++;
                    decoder->packets_rejected++;
                    DEBUGOUT("CRC mismatch. Resetting to WAIT_FOR_HEADER\n");
                    decoder->uart_payload_temp_buffer_index = 0;
                }
            }
            break;
}


        case READ_TAIL:
            if (byte == UART_TAIL) {
                decoder->packet.tail = byte;
                decoder->total_packets_parsed++;
                decoder->packets_accepted++;
                DEBUGOUT("Packet accepted!\n");
                uart_fsm_print_packet(&decoder->packet);
                decoder->state = WAIT_FOR_HEADER; // Reset for next packet
                decoder->uart_payload_temp_buffer_index = 0;
                DEBUGOUT("State: READ_TAIL -> WAIT_FOR_HEADER\n");
            } else {
                decoder->state = WAIT_FOR_HEADER;
                DEBUGOUT("Invalid tail. Resetting to WAIT_FOR_HEADER\n");
            }
            break;

        default:
            decoder->state = WAIT_FOR_HEADER;
            DEBUGOUT("Unknown state. Resetting to WAIT_FOR_HEADER\n");
            break;
    }
}


