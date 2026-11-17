/***************************************************************************//**
 * @file main.c
 * @brief Main entry point
 ******************************************************************************/
#include "sl_component_catalog.h"
#include "sl_system_init.h"
#include "app.h"

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif

#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else
#include "sl_system_process_action.h"
#endif

int main(void)
{
    // Initialize Silicon Labs device, system, and protocol stacks
    sl_system_init();

    // Initialize the application (creates threads/tasks)
    app_init();

#if defined(SL_CATALOG_KERNEL_PRESENT)
    // Start the RTOS kernel
    sl_system_kernel_start();
#else
    while (1) {
        // Call Silicon Labs component processing routine
        sl_system_process_action();

        // Application process (if needed)
        app_process_action();

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
        sl_power_manager_sleep();
#endif
    }
#endif

    return 0;
}
