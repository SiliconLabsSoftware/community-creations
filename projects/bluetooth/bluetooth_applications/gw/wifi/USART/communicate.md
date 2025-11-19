# Giao thức UART Si917 ↔️ EFR32 (v1.1)

**Cập nhật theo yêu cầu:** *OTA_REQUEST/OTA_RESPONSE chỉ dùng để **xin phép OTA** (permission). Thông tin **version** của node phải được lấy qua **UUID_REQUEST/UUID_RESPONSE** (ví dụ đọc Firmware Revision trong Device Information Service).*

## 1. Khung gói (frame)

```
| Header |   Type   | Length |           Payload            | Endcode | Checksum | Tail |
|  1 B   |  2 B LE  | 2 B LE |        L byte (TLV)         |  1 B    |  2 B LE  | 1 B  |
```

- **Header** = `0xF0` (cố định)  
- **Type** = 2 byte, **little-endian**  
- **Length** = tổng byte của **Payload**  
- **Endcode** = `0xFF` (cố định)  
- **Checksum** = **CRC-16/CCITT** (poly `0x1021`, init `0xFFFF`) tính trên **Type‖Length‖Payload‖Endcode** (không gồm Header & Tail), ghi **LE**  
- **Tail** = `0x0F` (cố định)

---

## 2. Bảng Type (2 byte, LE)

| Type (hex) | Tên lệnh        | Mô tả |
|---:|---|---|
| —    | ~~OTA_NOTIFY~~ | **Bỏ**: version không nằm ở đây |
| 0x0202 | OTA_REQUEST   | Xin phép OTA cho node (không kèm version) |
| 0x0203 | OTA_RESPONSE  | Phản hồi phép OTA (`STATUS`: OK/ERROR) |
| 0x0204 | OTA_DATA      | Truyền **firmware** theo chunk |
| 0x0210 | UUID_REQUEST  | Yêu cầu dữ liệu theo **UUID** (ví dụ Firmware Revision) |
| 0x0211 | UUID_RESPONSE | Trả dữ liệu theo **UUID** |
| 0x0220 | NODE_NOTIFY   | Trạng thái kết nối node |
| 0x0230 | WIFI_REQUEST  | EFR32 → Si917: yêu cầu kết nối Wi-Fi |
| 0x0231 | WIFI_RESPONSE | Si917 → EFR32: kết quả kết nối |
| 0x0232 | WIFI_NOTIFY   | Trạng thái Wi-Fi hiện tại |

---

## 3. Payload: TLV (Type–Length–Value)

```
|  T  |   L    |   V ... |
| 1 B | 2 B LE | L bytes |
(repeat)
```

### 3.1. Danh mục TLV.T (1 byte)

| T (hex) | Tên TLV           | Giá trị V |
|---:|---|---|
| 0x01 | MAC_ADDRESS     | 6 byte (BD_ADDR) |
| 0x02 | UUID            | 16 byte (128-bit UUID) |
| 0x03 | STATUS          | 1 byte mã trạng thái |
| 0x04 | UUID_VALUE      | Dữ liệu từ UUID |
| 0x05 | OTA_VALUE       | Dữ liệu OTA (tuỳ loại gói) |
| 0x06 | WIFI_CONFIG     | JSON UTF-8 |

#### 3.2. STATUS (0x03) — mã đề xuất

| Mã | Ý nghĩa |
|---:|---|
| 0x00 | OK / Cho phép |
| 0x01 | BUSY |
| 0x02 | ERROR |
| 0x03 | INVALID_ARG / FORMAT |
| 0x04 | NOT_FOUND |
| 0x05 | TIMEOUT |
| 0x06 | DENIED / POLICY |

#### 3.3. OTA_VALUE (0x05)

- **Trong OTA_REQUEST**:  
  `V = [REQ_FLAGS:1]` (bit0=force, 1=force; 0=normal). *Không chứa version.*  
- **Trong OTA_RESPONSE**:  
  Không bắt buộc có `OTA_VALUE`; chỉ cần `STATUS` (0=OK cho phép). Có thể thêm thông điệp lỗi bằng TLV khác nếu muốn.  
- **Trong OTA_DATA (chunk)**:  
  ```
  V = [CHUNK_IDX:2B LE][TOTAL_CHUNKS:2B LE][DATA...]
  ```

> **Version** của node: thực hiện bằng **UUID_REQUEST** tới UUID tương ứng (ví dụ **Firmware Revision: 00002A26-0000-1000-8000-00805F9B34FB**). Giá trị sẽ nằm trong `UUID_RESPONSE` dưới TLV `UUID_VALUE` (0x04).

---

## 4. Ví dụ gói (đã tính CRC)

### 4.1. UUID_REQUEST để lấy phiên bản (Firmware Revision – 0x2A26 dưới dạng 128-bit)

- **Type** = `0x0210` (LE: `10 02`), **Payload** = TLV MAC (T=01) + TLV UUID (T=02).  
- **Hex frame:**

```
F0 10 02 1C 00 01 06 00 DE AD BE EF 00 01 02 10 00 00 00 2A 26 00 00 10 00 80 00 00 80 5F 9B 34 FB FF 44 D9 0F
```

### 4.2. OTA_REQUEST (xin phép OTA, không kèm version)

- **Type** = `0x0202` (LE: `02 02`), **Payload** = TLV MAC (T=01) + TLV OTA_VALUE (T=05, REQ_FLAGS=0x00).  
- **Hex frame:**

```
F0 02 02 0D 00 01 06 00 DE AD BE EF 00 01 05 01 00 00 FF E0 58 0F
```

### 4.3. OTA_RESPONSE (được phép OTA)

- **Type** = `0x0203` (LE: `03 02`), **Payload** = TLV MAC (T=01) + TLV STATUS (T=03 = 0x00 OK).  
- **Hex frame:**

```
F0 03 02 0D 00 01 06 00 DE AD BE EF 00 01 03 01 00 00 FF 54 65 0F
```

### 4.4. OTA_DATA (chunk 1/3, DATA=01..08)

- **Type** = `0x0204` (LE: `04 02`), **Payload** = TLV OTA_VALUE (T=05).  
- **Hex frame:**

```
F0 04 02 0F 00 05 0C 00 01 00 03 00 01 02 03 04 05 06 07 08 FF E8 88 0F
```

---

## 5. CRC-16/CCITT (C tham khảo)

```c
uint16_t crc16_ccitt(const uint8_t *d, uint16_t n){
  uint16_t crc = 0xFFFF;
  for (uint16_t i=0;i<n;i++){
    crc ^= (uint16_t)d[i] << 8;
    for (int k=0;k<8;k++)
      crc = (crc & 0x8000) ? (crc<<1) ^ 0x1021 : (crc<<1);
  }
  return crc;
}
```

Tính CRC trên: **Type (2B) + Length (2B) + Payload + Endcode (0xFF)**, ghi **LE** vào trường *Checksum*.

---

## 6. Ghi chú

- **Version** luôn đi qua **UUID_REQUEST/RESPONSE**; cơ chế OTA chỉ dùng để **xin phép** và **truyền dữ liệu**.  
- Nên giới hạn độ dài `Length` theo RAM; OTA chia chunk và có thể bổ sung cơ chế **ACK/NAK** ở lớp ứng dụng.  
- Có thể thêm TLV thông điệp lỗi (ví dụ `0x07=ERROR_TEXT`) trong `OTA_RESPONSE` nếu muốn lý do chi tiết.

— *Hết.*