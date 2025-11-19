import serial
import struct
import time

# ------------------ Cấu hình UART ---------------------
PORT = 'COM4'           # ⚠️ sửa đúng cổng bạn dùng
BAUD = 115200
TIMEOUT = 1

ser = serial.Serial(PORT, BAUD, timeout=TIMEOUT)
print(f"Connected to {PORT} at {BAUD} baud")

# ------------------ Hàm CRC16-CCITT -------------------
def crc16_ccitt(data: bytes, poly=0x1021, init=0xFFFF):
    crc = init
    for byte in data:
        crc ^= byte << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = (crc << 1) ^ poly
            else:
                crc <<= 1
            crc &= 0xFFFF
    return crc

# ------------------ Build frame -----------------------
def build_uart_frame(type_val: int, payload: bytes) -> bytes:
    SOF = 0xF0
    ENDCODE = 0xFF
    TAIL = 0x0F

    payload_len = len(payload)
    header = struct.pack('<BHH', SOF, type_val, payload_len)
    footer_raw = struct.pack('<B', ENDCODE)

    crc_input = header[1:] + payload + footer_raw  # exclude SOF
    crc = crc16_ccitt(crc_input)
    footer_crc_tail = struct.pack('<H', crc) + struct.pack('<B', TAIL)

    return header + payload + footer_raw + footer_crc_tail

# ------------------ Send frame ------------------------
type_val = 0x0210     # PING
payload = b'Ping'

frame = build_uart_frame(type_val, payload)
print("Sending frame:", frame.hex())
ser.write(frame)

# ------------------ Read response ---------------------
time.sleep(0.5)
received = ser.read(256)  # đọc tối đa 256 bytes
print("Received:", received.hex())

# ------------------ Optional: Parse -------------------
def parse_uart_frame(frame: bytes):
    if len(frame) < 8:
        print("Too short")
        return

    if frame[0] != 0xF0:
        print("Invalid SOF")
        return

    type_val = int.from_bytes(frame[1:3], 'little')
    plen = int.from_bytes(frame[3:5], 'little')
    endcode = frame[5+plen]

    crc = int.from_bytes(frame[6+plen:8+plen], 'little')
    tail = frame[8+plen]

    payload = frame[5:5+plen]
    expected_crc = crc16_ccitt(frame[1:6+plen])  # exclude SOF

    print(f"TYPE=0x{type_val:04X}, LEN={plen}, PAYLOAD={payload}, CRC={crc:04X}, TAIL={tail:02X}")
    if crc == expected_crc:
        print("✅ CRC OK")
    else:
        print("❌ CRC MISMATCH")

if len(received) > 0:
    parse_uart_frame(received)
