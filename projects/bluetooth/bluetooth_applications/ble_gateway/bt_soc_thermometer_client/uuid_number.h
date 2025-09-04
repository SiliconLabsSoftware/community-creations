#ifndef UUID_NUMBER_H
#define UUID_NUMBER_H
#include<stdio.h>
#include "stdint.h"
#include "string.h"
// ====== Extern OTA UUID (định nghĩa trong ota_client.c hoặc file riêng) ======
extern const uint8_t uuid_ota_service[16] ;
// OTA Control characteristic UUID
extern const uint8_t uuid_ota_control[16] ;
extern const uint8_t uuid_ota_data[16] ;
extern const uint8_t uuid_bootloader_version[16] ;
extern const uint8_t uuid_apploader_version[16];
extern const uint8_t uuid_ota_version[16] ;
extern const uint8_t uuid_application_version[16];

//extern thermo_service UUID
extern const uint8_t uuid_thermo_service[2] ;
// Temperature Measurement characteristic UUID defined by Bluetooth SIG
extern const uint8_t uuid_temp[2] ;
extern const uint8_t uuid_temp_type[2] ;
extern const uint8_t uuid_measurement_interval[2] ;
extern const uint8_t uuid_intermediate_temp[2] ;


//extern device_info_service UUID
extern const uint8_t uuid_device_info_service[2] ;
//device info characteristic UUID
extern const uint8_t uuid_manufacturer_name[2];
extern const uint8_t uuid_model_number[2] ;
extern const uint8_t uuid_hardware_revision[2];
extern const uint8_t uuid_firmware_revision[2];
extern const uint8_t uuid_system_id[2];

#endif