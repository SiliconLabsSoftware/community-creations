#include "app.h"
#include "icm40627_example.h"
#include "sl_si91x_icm40627.h"
#include "sl_status.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "sl_sleeptimer.h"
#define TEMP_THRESHOLD 75.0f    // °C
#define VIB_THRESHOLD  2.0f     // g RMS
float latest_temperature = 0.0f;
float latest_accel[3] = {0};
float latest_gyro[3]  = {0};
static float get_vibration_level(void)
{
    return sqrtf(latest_accel[0]*latest_accel[0] +
                 latest_accel[1]*latest_accel[1] +
                 latest_accel[2]*latest_accel[2]);
}
void app_init(void)
{
    icm40627_example_init();
    printf("Engine Health Monitoring Started...\n");
}

void app_process_action(void)
{
    sl_status_t status;
    char status_str[16] = "NORMAL";
    float vibration = 0.0f;

    icm40627_example_process_action();

    status = sl_si91x_icm40627_get_temperature_data(ssi_driver_handle, &latest_temperature);
    if (status != SL_STATUS_OK) {
        printf("Temperature read failed\n");
    }

    status = sl_si91x_icm40627_get_accel_data(ssi_driver_handle, latest_accel);
    if (status != SL_STATUS_OK) {
        printf("Accel read failed\n");
    }

    status = sl_si91x_icm40627_get_gyro_data(ssi_driver_handle, latest_gyro);
    if (status != SL_STATUS_OK) {
        printf("Gyro read failed\n");
    }

    vibration = get_vibration_level();

    if (latest_temperature > TEMP_THRESHOLD || vibration > VIB_THRESHOLD) {
        strcpy(status_str, "UNHEALTHY");
    } else if (latest_temperature > (TEMP_THRESHOLD - 5) || vibration > (VIB_THRESHOLD - 0.5)) {
        strcpy(status_str, "WARNING");
    }

    printf("Temp: %.2f °C | Vib: %.2f g | Status: %s\n",
           latest_temperature, vibration, status_str);
    sl_sleeptimer_delay_millisecond(3000);
}
