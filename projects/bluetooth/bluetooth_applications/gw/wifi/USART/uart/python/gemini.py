import serial
import time
import crcmod

# --- Cấu hình ---
# Thay đổi COMx thành cổng COM của thiết bị SiLabs của bạn
SERIAL_PORT = 'COM13'
BAUD_RATE = 115200

# --- Cấu trúc gói tin ---
SOF = b'\xF0'
ENDCODE = b'\xFF'
TAIL = b'\x0F'
CRC16_INIT = 0xFFFF
CRC16_POLY = 0x1021

# --- Hàm CRC16 ---
def crc16_ccitt(data):
    crc16 = crcmod.Crc(poly=0x11021, initCrc=0xFFFF, xorOut=0x0000, rev=False)
    crc16.update(data)
    return crc16.crcValue

# --- Xây dựng gói tin ---
def build_packet(packet_type, payload):
    # Payload
    payload_len = len(payload)
    
    # Header: SOF + TYPE + LEN
    header = SOF + packet_type.to_bytes(2, 'little') + payload_len.to_bytes(2, 'little')
    
    # Data to be CRC'd
    crc_data = packet_type.to_bytes(2, 'little') + payload_len.to_bytes(2, 'little') + payload + ENDCODE
    
    # Calculate CRC
    calculated_crc = crc16_ccitt(crc_data)
    
    # Full packet
    packet = header + payload + ENDCODE + calculated_crc.to_bytes(2, 'little') + TAIL
    
    return packet

# --- Phân tích gói tin ---
def parse_packet(data):
    if len(data) < 9:
        print("Lỗi: Gói tin quá ngắn.")
        return None
    if data[0:1] != SOF or data[-1:] != TAIL:
        print("Lỗi: SOF hoặc TAIL không khớp.")
        return None

    # Tách các trường
    packet_type = int.from_bytes(data[1:3], 'little')
    payload_len = int.from_bytes(data[3:5], 'little')
    payload = data[5 : 5 + payload_len]
    endcode = data[5 + payload_len : 5 + payload_len + 1]
    crc_rx = int.from_bytes(data[5 + payload_len + 1 : 5 + payload_len + 3], 'little')
    
    # Kiểm tra ENDCODE
    if endcode != ENDCODE:
        print("Lỗi: ENDCODE không khớp.")
        return None
        
    # Tính CRC và so sánh
    crc_data = data[1:5] + payload + endcode
    crc_calc = crc16_ccitt(crc_data)
    
    if crc_rx != crc_calc:
        print(f"Lỗi: CRC không khớp. Nhận được: {hex(crc_rx)}, Tính toán: {hex(crc_calc)}")
        return None
        
    return {
        "type": packet_type,
        "payload": payload,
        "payload_len": payload_len
    }

# --- Chạy chính ---
def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=5)
        print(f"Đã mở cổng {SERIAL_PORT} thành công.")
        
        # Tạo gói tin để gửi
        packet_type = 0x0210  # Ví dụ: PING
        payload = b'Ping'
        
        tx_packet = build_packet(packet_type, payload)
        
        print(f"Gửi gói tin ({len(tx_packet)} bytes): {tx_packet.hex().upper()}")
        ser.write(tx_packet)
        
        # Chờ phản hồi
        print("Đang chờ phản hồi...")
        
        # ser.read_until() sẽ đọc cho đến khi tìm thấy byte TAIL
        rx_packet = ser.read_until(TAIL)
        
        if rx_packet:
            print(f"Nhận được ({len(rx_packet)} bytes): {rx_packet.hex().upper()}")
            
            # Phân tích và in kết quả
            parsed = parse_packet(rx_packet)
            if parsed:
                print(f"Phân tích thành công:")
                print(f"- Loại: 0x{parsed['type']:04X}")
                print(f"- Payload: {parsed['payload'].decode('utf-8', errors='ignore')}")
                print(f"- Độ dài Payload: {parsed['payload_len']} bytes")

        else:
            print("Không nhận được phản hồi nào trong thời gian chờ.")
            
    except serial.SerialException as e:
        print(f"Lỗi: Không thể mở cổng serial {SERIAL_PORT}. Vui lòng kiểm tra lại.")
        print(e)
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Đã đóng cổng serial.")

if __name__ == "__main__":
    main()
