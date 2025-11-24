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
    memset(decoder->payload_buffer, 0, sizeof(decoder->payload_buffer));
    decoder->payload_index = 0;

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

// ============================= DEBUG FUNCTIONS ==========================
//void uart_fsm_print_stats(const uart_fsm_decoder_t *decoder) {
//    printf("\n=== PACKET STATISTICS ===\n");
//    printf("Total packets parsed: %u\n", decoder->total_packets_parsed);
//    printf("Packets accepted: %u\n", decoder->packets_accepted);
//    printf("Packets rejected: %u\n", decoder->packets_rejected);
//    printf("  - Bad checksum: %u\n", decoder->bad_checksum_count);
//    printf("  - Bad length: %u\n", decoder->bad_length_count);
//    printf("========================\n");
//}
//
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
        case PKT_TYPE_TEST:
            DEBUGOUT("Packet TYPE: PKT_TYPE_TEST (0x%04X)\n", pkt->type);
            break;
        default:
            DEBUGOUT("Packet type is invalid: 0x%04X\n", pkt->type);
            return false;
    }
    return true;
}

bool packet_length_validate (uart_packet_t *pkt){
    if (pkt->length == 0 || pkt->length > UART_MAX_PACKET_LEN){
        DEBUGOUT ("Packet length is invalid: %u\n", pkt->length);
        return false;
    }
    DEBUGOUT("Packet LENGTH: %u\n", pkt->length);
    return true;
}

bool payload_validate (payload_t *payload){

    if (!payload) {
        DEBUGOUT("Payload is NULL\n");
        return false;
    }

    // Payload type validate
    switch (payload->type) {
        case TLV_TEST:
            DEBUGOUT("Payload TYPE: TLV_TEST (0x%02X)\n", payload->type);
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
                decoder->state = READ_TYPE;
                DEBUGOUT("State: WAIT_FOR_HEADER -> READ_TYPE\n");
            }
            break;

        case READ_TYPE:
            decoder->packet.type |= (byte << ((decoder->payload_index % 2) * 8));
            decoder->payload_index++;
            if (decoder->payload_index == 2) {
                decoder->state = READ_LENGTH;
                decoder->payload_index = 0;
                DEBUGOUT("State: READ_TYPE -> READ_LENGTH\n");
            }
            break;

        case READ_LENGTH:
            decoder->packet.length |= (byte << ((decoder->payload_index % 2) * 8));
            decoder->payload_index++;
            if (decoder->payload_index == 2) {
                if (!packet_length_validate(&decoder->packet)) {
                    decoder->state = WAIT_FOR_HEADER;
                    decoder->bad_length_count++;
                    DEBUGOUT("Invalid length. Resetting to WAIT_FOR_HEADER\n");
                } else {
                    decoder->state = READ_PAYLOAD;
                    decoder->payload_index = 0;
                    DEBUGOUT("State: READ_LENGTH -> READ_PAYLOAD\n");
                }
            }
            break;

        case READ_PAYLOAD:
            decoder->payload_buffer[decoder->payload_index++] = byte;
            if (decoder->payload_index >= decoder->packet.length) {
                memcpy(decoder->packet.payload,
                       decoder->payload_buffer,
                       decoder->packet.length);
                decoder->state = READ_ENDCODE;
                DEBUGOUT("State: READ_PAYLOAD -> READ_ENDCODE\n");
            }
            break;

        case READ_ENDCODE:
            if (byte == UART_ENDCODE) {
                decoder->packet.endcode = byte;
                decoder->state = READ_CRC;
                decoder->payload_index = 0;
                DEBUGOUT("State: READ_ENDCODE -> READ_CRC\n");
            } else {
                decoder->state = WAIT_FOR_HEADER;
                DEBUGOUT("Invalid endcode. Resetting to WAIT_FOR_HEADER\n");
            }
            break;

        case READ_CRC:
            decoder->received_crc |= (byte << ((decoder->payload_index % 2) * 8));
            decoder->payload_index++;
            if (decoder->payload_index == 2) {
                // Calculate CRC
                decoder->calculated_crc = crc16_ccitt((uint8_t *)&decoder->packet, sizeof(uart_packet_t) - sizeof(uint16_t) - sizeof(uint8_t));
                if (decoder->calculated_crc == decoder->received_crc) {
                    decoder->packet.crc = decoder->received_crc;
                    decoder->state = READ_TAIL;
                    decoder->payload_index = 0;
                    DEBUGOUT("State: READ_CRC -> READ_TAIL\n");
                } else {
                    decoder->state = WAIT_FOR_HEADER;
                    decoder->bad_checksum_count++;
                    DEBUGOUT("CRC mismatch. Resetting to WAIT_FOR_HEADER\n");
                }
}
            break;

        case READ_TAIL:
            if (byte == UART_TAIL) {
                decoder->packet.tail = byte;
                decoder->total_packets_parsed++;
                decoder->packets_accepted++;
                DEBUGOUT("Packet accepted!\n");
                uart_fsm_print_packet(&decoder->packet);
                decoder->state = WAIT_FOR_HEADER; // Reset for next packet
                decoder->payload_index = 0;
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


