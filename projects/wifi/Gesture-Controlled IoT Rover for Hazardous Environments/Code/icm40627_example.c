/***************************************************************************/ /**
 * @file icm40627_example.c
 * @brief ICM40627 example APIs for FreeRTOS
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 ******************************************************************************/

#include "rsi_debug.h"
#include "sl_si91x_icm40627.h"
#include "icm40627_example.h"
#include "sl_si91x_driver_gpio.h"
#include "sl_si91x_ssi.h"
#include "rsi_rom_clks.h"
#include "cmsis_os2.h"

/*******************************************************************************
 ***************************  Defines / Macros  ********************************/
#define SSI_MASTER_DIVISION_FACTOR         0
#define SSI_MASTER_INTF_PLL_CLK            180000000
#define SSI_MASTER_INTF_PLL_REF_CLK        40000000
#define SSI_MASTER_SOC_PLL_CLK             20000000
#define SSI_MASTER_SOC_PLL_REF_CLK         40000000
#define SSI_MASTER_INTF_PLL_500_CTRL_VALUE 0xD900
#define SSI_MASTER_SOC_PLL_MM_COUNT_LIMIT  0xA4
#define SSI_MASTER_BIT_WIDTH               8
#define SSI_MASTER_BAUDRATE                10000000
#define SSI_MASTER_RECEIVE_SAMPLE_DELAY    0

/*******************************************************************************
 *************************** LOCAL VARIABLES **********************************/
static sl_ssi_handle_t ssi_driver_handle = NULL;
static uint32_t ssi_slave_number = SSI_SLAVE_0;

/*******************************************************************************
 **********************  Local Function prototypes ***************************/
static sl_status_t ssi_master_init_clock_configuration_structure(sl_ssi_clock_config_t *clock_config);
static void ssi_master_callback_event_handler(uint32_t event);

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   ******************************
 ******************************************************************************/

void icm40627_example_init(void)
{
    sl_status_t status;
    sl_ssi_clock_config_t ssi_clock_config;
    sl_ssi_version_t ssi_version;
    uint8_t dev_id;

    sl_ssi_control_config_t ssi_master_config = {
        .bit_width            = SSI_MASTER_BIT_WIDTH,
        .device_mode          = SL_SSI_ULP_MASTER_ACTIVE,
        .clock_mode           = SL_SSI_PERIPHERAL_CPOL0_CPHA0,
        .baud_rate            = SSI_MASTER_BAUDRATE,
        .receive_sample_delay = SSI_MASTER_RECEIVE_SAMPLE_DELAY
    };

#if defined(SENSOR_ENABLE_GPIO_MAPPED_TO_UULP)
    // Enable GPIO for sensor
    status = sl_si91x_gpio_driver_enable_clock((sl_si91x_gpio_select_clock_t)ULPCLK_GPIO);
    DEBUGOUT("GPIO driver clock enable %s\n", (status == SL_STATUS_OK) ? "successful" : "failed");
    sl_si91x_gpio_driver_set_uulp_npss_pin_mux(SENSOR_ENABLE_GPIO_PIN, NPSS_GPIO_PIN_MUX_MODE0);
    sl_si91x_gpio_driver_set_uulp_npss_direction(SENSOR_ENABLE_GPIO_PIN, GPIO_OUTPUT);
    sl_si91x_gpio_driver_set_uulp_npss_pin_value(SENSOR_ENABLE_GPIO_PIN, SET);
#endif

    // Initialize SSI
    ssi_version = sl_si91x_ssi_get_version();
    DEBUGOUT("Initializing SSI for ICM40627...\n");
    DEBUGOUT("SSI version %d.%d.%d\n", ssi_version.release, ssi_version.major, ssi_version.minor);

    status = ssi_master_init_clock_configuration_structure(&ssi_clock_config);
    if (status != SL_STATUS_OK) {
        DEBUGOUT("SSI clock config init failed: 0x%lx\n", status);
        return;
    }
    sl_si91x_ssi_configure_clock(&ssi_clock_config);

    status = sl_si91x_ssi_init(ssi_master_config.device_mode, &ssi_driver_handle);
    if (status != SL_STATUS_OK) {
        DEBUGOUT("SSI init failed: 0x%lx\n", status);
        return;
    }

    status = sl_si91x_ssi_set_configuration(ssi_driver_handle, &ssi_master_config, ssi_slave_number);
    if (status != SL_STATUS_OK) {
        DEBUGOUT("SSI set configuration failed: 0x%lx\n", status);
        return;
    }

    sl_si91x_ssi_register_event_callback(ssi_driver_handle, ssi_master_callback_event_handler);
    sl_si91x_ssi_set_slave_number((uint8_t)ssi_slave_number);

    // Reset sensor
    status = sl_si91x_icm40627_software_reset(ssi_driver_handle);
    if (status != SL_STATUS_OK) {
        DEBUGOUT("Sensor software reset failed: 0x%lx\n", status);
        return;
    }
    DEBUGOUT("Sensor software reset successful\n");

    // Verify Device ID
    status = sl_si91x_icm40627_get_device_id(ssi_driver_handle, &dev_id);
    if ((status != SL_STATUS_OK) || (dev_id != ICM40627_DEVICE_ID)) {
        DEBUGOUT("ICM40627 Get Device ID failed! (status=0x%lx, dev_id=0x%02x)\n", status, dev_id);
        return;
    }
    DEBUGOUT("Successfully verified ICM40627 Device ID\n");

    // Initialize sensor
    status = sl_si91x_icm40627_init(ssi_driver_handle);
    if (status != SL_STATUS_OK) {
        DEBUGOUT("Sensor init failed: 0x%lx\n", status);
        return;
    }
    DEBUGOUT("Sensor init successful\n");
}

void icm40627_example_process_action(void)
{
    sl_status_t status;
    float temperature;
    float sensor_data[3];

    // Temperature
    status = icm40627_example_get_temperature(&temperature);
    if (status == SL_STATUS_OK) {
        DEBUGOUT("Temperature: %.2f C\n", temperature);
    } else {
        DEBUGOUT("Temperature read failed: 0x%lx\n", status);
    }

    // Accelerometer
    status = icm40627_example_get_accel(sensor_data);
    if (status == SL_STATUS_OK) {
        DEBUGOUT("Acceleration: { %.2f, %.2f, %.2f }\n", sensor_data[0], sensor_data[1], sensor_data[2]);
    } else {
        DEBUGOUT("Accel read failed: 0x%lx\n", status);
    }

    // Gyroscope
    status = icm40627_example_get_gyro(sensor_data);
    if (status == SL_STATUS_OK) {
        DEBUGOUT("Gyro: { %.2f, %.2f, %.2f }\n\n", sensor_data[0], sensor_data[1], sensor_data[2]);
    } else {
        DEBUGOUT("Gyro read failed: 0x%lx\n", status);
    }
}

/*******************************************************************************
 * @brief  SSI Master callback handler
 *******************************************************************************/
static void ssi_master_callback_event_handler(uint32_t event)
{
    if (event == SSI_EVENT_TRANSFER_COMPLETE) {
        // Handle transfer complete if needed
    }
}

/*******************************************************************************
 * @brief  SSI clock configuration
 *******************************************************************************/
static sl_status_t ssi_master_init_clock_configuration_structure(sl_ssi_clock_config_t *clock_config)
{
    if (!clock_config) return SL_STATUS_NULL_POINTER;
    clock_config->soc_pll_mm_count_value     = SSI_MASTER_SOC_PLL_MM_COUNT_LIMIT;
    clock_config->intf_pll_500_control_value = SSI_MASTER_INTF_PLL_500_CTRL_VALUE;
    clock_config->intf_pll_clock             = SSI_MASTER_INTF_PLL_CLK;
    clock_config->intf_pll_reference_clock   = SSI_MASTER_INTF_PLL_REF_CLK;
    clock_config->soc_pll_clock              = SSI_MASTER_SOC_PLL_CLK;
    clock_config->soc_pll_reference_clock    = SSI_MASTER_SOC_PLL_REF_CLK;
    clock_config->division_factor            = SSI_MASTER_DIVISION_FACTOR;
    return SL_STATUS_OK;
}

/*******************************************************************************
 * @brief  Getter functions for use in Wi-Fi thread
 *******************************************************************************/
sl_status_t icm40627_example_get_accel(float accel_data[3])
{
    return sl_si91x_icm40627_get_accel_data(ssi_driver_handle, accel_data);
}

sl_status_t icm40627_example_get_gyro(float gyro_data[3])
{
    return sl_si91x_icm40627_get_gyro_data(ssi_driver_handle, gyro_data);
}

sl_status_t icm40627_example_get_temperature(float *temperature)
{
    return sl_si91x_icm40627_get_temperature_data(ssi_driver_handle, temperature);
}
