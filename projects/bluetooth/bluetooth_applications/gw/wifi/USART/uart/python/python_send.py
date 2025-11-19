# pip install pyserial
import argparse
import serial
import time

SOF, ENDCODE, TAIL = 0xF0, 0xFF, 0x0F

def crc16_ccitt(data: bytes, init=0xFFFF) -> int:
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
    typ = type_le.to_bytes(2, "little")
    ln  = len(payload).to_bytes(2, "little")
    end = bytes([ENDCODE])
    crc = crc16_ccitt(typ + ln + payload + end).to_bytes(2, "little")
    return bytes([SOF]) + typ + ln + payload + end + crc + bytes([TAIL])

def hexdump(tag: str, b: bytes):
    print(f"{tag} ({len(b)}): {b.hex(' ').upper()}")

def parse_frame(buf: bytes):
    """Đủ dùng để in ra phản hồi từ 917 (nếu có)."""
    if len(buf) < 9 or buf[0] != SOF or buf[-1] != TAIL:
        return None, "invalid header/tail"
    typ = int.from_bytes(buf[1:3], "little")
    ln  = int.from_bytes(buf[3:5], "little")
    expect = 1+2+2+ln+1+2+1
    if len(buf) != expect:
        return None, f"length mismatch (len={len(buf)} expect={expect})"
    payload = buf[5:5+ln]
    rx_crc  = int.from_bytes(buf[6+ln:8+ln], "little")
    cal_crc = crc16_ccitt(buf[1:1+2+2+ln+1])
    if rx_crc != cal_crc:
        return None, f"crc mismatch rx={rx_crc:04X} calc={cal_crc:04X}"
    return (typ, payload), None

def parse_hex_bytes(s: str) -> bytes:
    s = s.replace(",", " ").strip()
    return bytes(int(x, 16) for x in s.split()) if s else b""

def main():
    ap = argparse.ArgumentParser(description="Send one or more frames to SiWx917 via UART.")
    ap.add_argument("--port", required=True, help="Serial port (e.g., COM4 or /dev/ttyUSB0)")
    ap.add_argument("--baud", type=int, default=115200)
    ap.add_argument("--type", type=lambda x: int(x, 0), default=0x0210, help="Frame TYPE (e.g., 0x0210)")
    grp = ap.add_mutually_exclusive_group()
    grp.add_argument("--text", help="ASCII payload, e.g., 'Ping'")
    grp.add_argument("--hex",  help="Hex payload, e.g., '01 02 FF'")
    ap.add_argument("--rx", action="store_true", help="Read back response once")
    ap.add_argument("--timeout", type=float, default=1.0, help="Read timeout (s)")
    ap.add_argument("--repeat", type=int, default=1, help="Send N times")
    ap.add_argument("--interval", type=float, default=0.1, help="Delay between sends (s)")
    ap.add_argument("--rtscts", action="store_true", help="Enable RTS/CTS (default off)")
    ap.add_argument("--xonxoff", action="store_true", help="Enable XON/XOFF (default off)")
    args = ap.parse_args()

    payload = (
        args.text.encode("utf-8", errors="ignore") if args.text is not None
        else parse_hex_bytes(args.hex) if args.hex is not None
        else b"Ping"
    )

    frame = build_frame(args.type, payload)
    hexdump("TX frame", frame)

    ser = serial.Serial(
        port=args.port,
        baudrate=args.baud,
        bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        timeout=args.timeout,
        rtscts=args.rtscts,
        dsrdtr=False,
        xonxoff=args.xonxoff,
    )

    try:
        for i in range(args.repeat):
            ser.write(frame)
            if args.rx:
                # đọc tối đa 1 frame trả về
                buf = bytearray()
                t0 = time.monotonic()
                while time.monotonic() - t0 < args.timeout:
                    chunk = ser.read(1)
                    if not chunk:
                        continue
                    buf.extend(chunk)
                    if chunk[0] == TAIL:
                        break
                if buf:
                    hexdump("RX bytes", bytes(buf))
                    parsed, err = parse_frame(bytes(buf))
                    if parsed:
                        typ, pl = parsed
                        print(f"RX OK: TYPE=0x{typ:04X}, PAYLOAD({len(pl)}): {pl.hex(' ').upper()} | ASCII={pl.decode(errors='ignore')}")
                    else:
                        print("[PARSE ERR]", err)
                else:
                    print("RX: <timeout/no data>")
            if i+1 < args.repeat:
                time.sleep(args.interval)
    finally:
        ser.close()

if __name__ == "__main__":
    main()
