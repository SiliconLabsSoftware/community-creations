/***************************************************************************/ /**
 * @file icm40627_example.h
 * @brief ICM40627 example
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#ifndef ICM40627_EXAMPLE_H_
#define ICM40627_EXAMPLE_H_

#include "sl_status.h"

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Prototypes
/***************************************************************************/ /**
 * ICM40627 initialization function.
 * @param none
 * @return none
 ******************************************************************************/
void icm40627_example_init(void);

/***************************************************************************/ /**
 * Function will run continuously in while loop and reads sensor values
 * @param none
 * @return none
 ******************************************************************************/
void icm40627_example_process_action(void);

/***************************************************************************/ /**
 * Get latest acceleration values (X, Y, Z).
 * @param accel_data[3]  array to hold accel data
 * @return sl_status_t   SL_STATUS_OK if successful
 ******************************************************************************/
sl_status_t icm40627_example_get_accel(float accel_data[3]);

/***************************************************************************/ /**
 * Get latest gyroscope values (X, Y, Z).
 * @param gyro_data[3]   array to hold gyro data
 * @return sl_status_t   SL_STATUS_OK if successful
 ******************************************************************************/
sl_status_t icm40627_example_get_gyro(float gyro_data[3]);

/***************************************************************************/ /**
 * Get latest temperature value.
 * @param temperature    pointer to float to hold temperature
 * @return sl_status_t   SL_STATUS_OK if successful
 ******************************************************************************/
sl_status_t icm40627_example_get_temperature(float *temperature);

#endif /* ICM40627_EXAMPLE_H_ */
