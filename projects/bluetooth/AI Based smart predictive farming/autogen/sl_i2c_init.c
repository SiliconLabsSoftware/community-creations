/***************************************************************************//**
 * @file sl_i2c_init.c.jinja
 * @brief I2C Driver Instance Initialization
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "sl_si91x_i2c.h"

#include "sl_si91x_i2c_init_i2c2_config.h"


#include "sl_i2c_instances.h"


sl_i2c_config_t sl_i2c_i2c2_config = {
  .mode = SL_I2C_I2C2_MODE,
  .operating_mode = SL_I2C_I2C2_OPERATING_MODE,
  .transfer_type = SL_I2C_I2C2_TRANSFER_TYPE,
  .i2c_callback = NULL,
};



sl_i2c_pin_init_t sl_i2c_i2c2_pin_init = {
  .sda_port = SL_I2C_I2C2_SDA_PORT,
  .sda_pin = SL_I2C_I2C2_SDA_PIN,
  .sda_mux = SL_I2C_I2C2_SDA_MUX,
  .sda_pad = SL_I2C_I2C2_SDA_PAD,
  .scl_port = SL_I2C_I2C2_SCL_PORT,
  .scl_pin = SL_I2C_I2C2_SCL_PIN,
  .scl_mux = SL_I2C_I2C2_SCL_MUX,
  .scl_pad = SL_I2C_I2C2_SCL_PAD,
  .instance = SL_I2C2,
};



void sl_i2c_init_instances(void)
{


  sl_si91x_i2c_pin_init(&sl_i2c_i2c2_pin_init);
  
  
}