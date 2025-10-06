#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
SiWx917/EFR32 UART Protocol Tester (v2)

Usage examples:
  python uart_tester_v2.py --port COM13 ping
  python uart_tester_v2.py --port COM13 get_version
  python uart_tester_v2.py --port COM13 wifi_scan
  python uart_tester_v2.py --port COM13 wifi_status
  python uart_tester_v2.py --port COM13 wifi_connect --ssid MyAP --psk 12345678
  python uart_tester_v2.py --port COM13 mqtt_pub --topic test/topic --data "hello"
  python uart_tester_v2.py --port COM13 listen
  python uart_tester_v2.py --port COM13 raw --type 0x0210 --payload-hex "50 69 6E 67"
"""
from __future__ import annotations
import argparse
import serial
import time
import sys

SOF      = 0xF0
ENDCODE  = 0xFF
TAIL     = 0x0F

LEN_TYPE   = 2
LEN_PLEN   = 2
LEN_HEADER = 1 + LEN_TYPE + LEN_PLEN
LEN_ENDCODE= 1
LEN_CRC    = 2
LEN_TAIL   = 1
LEN_FOOTER = LEN_ENDCODE + LEN_CRC + LEN_TAIL

CRC16_POLY = 0x1021
CRC16_INIT = 0xFFFF

# ---------------------- Helpers ----------------------
def u16_le(v: int) -> bytes:
    return bytes((v & 0xFF, (v >> 8) & 0xFF))

def crc16_ccitt(data: bytes) -> int:
    crc = CRC16_INIT
    for d in data:
        crc ^= (d << 8) & 0xFFFF
        for _ in range(8):
            crc = ((crc << 1) ^ CRC16_POLY) & 0xFFFF if (crc & 0x8000) else ((crc << 1) & 0xFFFF)
    return crc

def build_frame(type_le: int, payload: bytes | None) -> bytes:
    if payload is None:
        payload = b''
    plen = len(payload)
    buf = bytearray()
    buf.append(SOF)
    buf += u16_le(type_le)
    buf += u16_le(plen)
    if plen:
        buf += payload
    buf.append(ENDCODE)
    crc = crc16_ccitt(bytes(buf[1:]))  # TYPE..ENDCODE
    buf += u16_le(crc)
    buf.append(TAIL)
    return bytes(buf)

def hexd(b: bytes) -> str:
    return ' '.join(f"{x:02X}" for x in b)

# --------- Stream parser (rolling buffer, tolerant) ----------
def extract_frame_from_buffer(buf: bytearray) -> tuple[bytes | None, int]:
    """
    Returns (frame_bytes, consumed_bytes).
    If no complete frame yet: (None, 0).
    If malformed at current SOF: (None, consumed_bytes_to_drop).
    """
    i = 0
    while i < len(buf):
        if buf[i] != SOF:
            i += 1
            continue
        # need header
        if i + LEN_HEADER > len(buf):
            break
        plen_lo_idx = i + 1 + LEN_TYPE
        plen = buf[plen_lo_idx] | (buf[plen_lo_idx + 1] << 8)
        total = LEN_HEADER + plen + LEN_FOOTER
        if i + total > len(buf):
            break  # incomplete
        endcode_pos = i + LEN_HEADER + plen
        if buf[endcode_pos] != ENDCODE or buf[i + total - 1] != TAIL:
            # bad tail/endcode -> skip this SOF
            i += 1
            continue
        rx_crc = buf[endcode_pos + 1] | (buf[endcode_pos + 2] << 8)
        calc_crc = crc16_ccitt(bytes(buf[i+1:endcode_pos+1]))  # TYPE..ENDCODE
        if rx_crc != calc_crc:
            # bad CRC -> drop this SOF
            return (None, i + 1)
        frame = bytes(buf[i:i+total])
        return (frame, i + total)
    return (None, 0)

def read_frame(ser: serial.Serial, timeout: float = 1.0) -> bytes | None:
    deadline = time.time() + timeout
    buf = bytearray()
    while time.time() < deadline:
        data = ser.read(ser.in_waiting or 1)
        if data:
            buf += data
            frame, consumed = extract_frame_from_buffer(buf)
            if frame:
                return frame
            elif consumed:
                del buf[:consumed]
        else:
            time.sleep(0.005)
    return None

# ------------------- Commands -------------------
def listen_loop(ser: serial.Serial):
    # flush trước khi nghe để bỏ dữ liệu cũ
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    buf = bytearray()
    print("Listening... press Ctrl-C to stop")
    try:
        while True:
            data = ser.read(ser.in_waiting or 1)
            if data:
                buf += data
                while True:
                    frame, consumed = extract_frame_from_buffer(buf)
                    if frame:
                        print("RX:", hexd(frame))
                        del buf[:consumed]
                    elif consumed:
                        del buf[:consumed]
                    else:
                        break
            else:
                time.sleep(0.01)
    except KeyboardInterrupt:
        print("Stopped.")

def cmd_ping(ser: serial.Serial):
    # flush trước khi gửi để khỏi dính rác
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    frm = build_frame(0x0210, b"Ping")
    ser.write(frm)
    print("TX:", hexd(frm))
    rx = read_frame(ser, timeout=1.0)
    print("RX:", hexd(rx) if rx else "No response")

def cmd_get_version(ser: serial.Serial):
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    frm = build_frame(0x0212, b'')
    ser.write(frm)
    print("TX:", hexd(frm))
    rx = read_frame(ser, timeout=1.0)
    print("RX:", hexd(rx) if rx else "No response")

def cmd_wifi_scan(ser: serial.Serial):
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    frm = build_frame(0x0232, b'')
    ser.write(frm)
    print("TX:", hexd(frm))
    rx = read_frame(ser, timeout=2.0)
    if rx: print("RX:", hexd(rx))
    else:  print("No response")

def cmd_wifi_status(ser: serial.Serial):
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    frm = build_frame(0x0236, b'')
    ser.write(frm)
    print("TX:", hexd(frm))
    rx = read_frame(ser, timeout=1.0)
    if rx: print("RX:", hexd(rx))
    else:  print("No response")

def cmd_wifi_connect(ser: serial.Serial, ssid: str, psk: str):
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    ssid_b = ssid.encode('utf-8')
    psk_b  = psk.encode('utf-8')
    payload = bytes([len(ssid_b)]) + ssid_b + bytes([len(psk_b)]) + psk_b
    frm = build_frame(0x0234, payload)
    ser.write(frm)
    print("TX:", hexd(frm))
    rx = read_frame(ser, timeout=5.0)
    print("RX:", hexd(rx) if rx else "No response")

def cmd_mqtt_pub(ser: serial.Serial, topic: str, data: str):
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    t = topic.encode('utf-8')
    d = data.encode('utf-8')
    payload = bytes([len(t)]) + t + d
    frm = build_frame(0x0248, payload)
    ser.write(frm)
    print("TX:", hexd(frm))
    rx = read_frame(ser, timeout=2.0)
    if rx: print("RX:", hexd(rx))
    else:  print("No response")

def cmd_raw(ser: serial.Serial, type_val: int, payload_hex: str):
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    payload = bytes(int(x, 16) for x in payload_hex.split()) if payload_hex else b''
    frm = build_frame(type_val, payload)
    ser.write(frm)
    print("TX:", hexd(frm))
    rx = read_frame(ser, timeout=1.0)
    if rx: print("RX:", hexd(rx))
    else:  print("No response")

# ------------------- Serial open -------------------
def open_serial(port: str, baud: int = 115200, timeout: float = 0.1) -> serial.Serial:
    ser = serial.Serial(port, baud, timeout=timeout)
    # >>> xả buffer ngay sau khi mở cổng
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    return ser

# ------------------- CLI -------------------
def main():
    p = argparse.ArgumentParser(description="SiWx917/EFR32 UART protocol tester")
    p.add_argument("--port", "-p", required=True)
    p.add_argument("--baud", "-b", type=int, default=115200)
    sub = p.add_subparsers(dest="cmd", required=True)

    sub.add_parser("ping")
    sub.add_parser("get_version")
    sub.add_parser("wifi_scan")
    sub.add_parser("wifi_status")
    sc = sub.add_parser("wifi_connect")
    sc.add_argument("--ssid", required=True)
    sc.add_argument("--psk", required=True)
    mp = sub.add_parser("mqtt_pub")
    mp.add_argument("--topic", required=True)
    mp.add_argument("--data", required=True)
    sub.add_parser("listen")
    raw = sub.add_parser("raw")
    raw.add_argument("--type", required=True)
    raw.add_argument("--payload-hex", default="")

    args = p.parse_args()

    try:
        ser = open_serial(args.port, baud=args.baud)
    except Exception as e:
        print("Can't open serial port:", e, file=sys.stderr)
        sys.exit(2)

    try:
        if args.cmd == "ping":
            cmd_ping(ser)
        elif args.cmd == "get_version":
            cmd_get_version(ser)
        elif args.cmd == "wifi_scan":
            cmd_wifi_scan(ser)
        elif args.cmd == "wifi_status":
            cmd_wifi_status(ser)
        elif args.cmd == "wifi_connect":
            cmd_wifi_connect(ser, args.ssid, args.psk)
        elif args.cmd == "mqtt_pub":
            cmd_mqtt_pub(ser, args.topic, args.data)
        elif args.cmd == "listen":
            listen_loop(ser)
        elif args.cmd == "raw":
            tval = int(args.type, 0)
            cmd_raw(ser, tval, args.payload_hex)
    finally:
        ser.close()

if __name__ == "__main__":
    main()
