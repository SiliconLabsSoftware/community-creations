# pip install pyserial
import serial

SOF     = 0xF0
ENDCODE = 0xFF
TAIL    = 0x0F

def crc16_ccitt(data: bytes, init=0xFFFF) -> int:
    """Calculates CRC-16-CCITT."""
    crc = init
    for d in data:
        crc ^= (d << 8) & 0xFFFF
        for _ in range(8):
            if crc & 0x8000:
                crc = ((crc << 1) ^ 0x1021) & 0xFFFF
            else:
                crc = (crc << 1) & 0xFFFF
    return crc

def build_frame(type_le: int, payload: bytes) -> bytes:
    """Builds a full frame from type and payload."""
    typ = type_le.to_bytes(2, 'little')
    ln  = len(payload).to_bytes(2, 'little')
    endcode = bytes([ENDCODE])
    crc = crc16_ccitt(typ + ln + payload + endcode).to_bytes(2, 'little')
    return bytes([SOF]) + typ + ln + payload + endcode + crc + bytes([TAIL])

def hexdump(tag, data: bytes):
    """Prints a hex dump of the data."""
    print(f"{tag} ({len(data)}):", data.hex(" ").upper())

def parse_frame(buf: bytes):
    """Parses a frame and returns its type and payload."""
    if len(buf) < 9 or buf[0] != SOF or buf[-1] != TAIL:
        print("Invalid frame")
        return None
    type_le = int.from_bytes(buf[1:3], "little")
    ln      = int.from_bytes(buf[3:5], "little")
    expect  = 1+2+2+ln+1+2+1
    if len(buf) != expect:
        print("Length mismatch")
        return None
    payload = buf[5:5+ln]
    rx_crc  = int.from_bytes(buf[6+ln:8+ln], "little")
    cal_crc = crc16_ccitt(buf[1: 1+2+2+ln+1])
    if rx_crc != cal_crc:
        print(f"CRC mismatch: rx={rx_crc:04X} calc={cal_crc:04X}")
        return None
    print(f"RX OK: TYPE=0x{type_le:04X}, PAYLOAD({ln}): {payload} | ASCII={payload.decode(errors='ignore')}")
    return type_le, payload

def main():
    """Main function to handle communication."""
    port = "COM13"      # đổi thành cổng UART kết nối với 917
    baud = 115200
    ser = serial.Serial(port, 115200, bytesize=8, parity='N', stopbits=1,
                        timeout=1, rtscts=False, dsrdtr=False, xonxoff=False)


    buf = bytearray()
    print("Echo host ready, waiting frame from 917...")
    while True:
        b = ser.read(1)
        if not b:
            continue
        buf.extend(b)
        if b[0] == TAIL:
            hexdump("RX frame", buf)
            parsed = parse_frame(bytes(buf))
            if parsed:
                _type, payload = parsed
                # Reply ACK voi TYPE=0x0211
                ack = build_frame(0x0211, payload)
                # In ra frame trước khi gửi
                hexdump("TX ACK", ack)
                ser.write(ack)
            buf.clear()

if __name__ == "__main__":
    main()
