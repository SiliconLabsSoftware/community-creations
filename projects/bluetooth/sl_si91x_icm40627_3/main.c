#include "sl_component_catalog.h"
#include "sl_main_init.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_main_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_main_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT

int main(void)
{
    sl_main_init();
#if defined(SL_CATALOG_KERNEL_PRESENT)
    sl_main_kernel_start();
#else
    app_init();
    while (1) {
        sl_main_process_action();
        app_process_action();
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
        sl_power_manager_sleep();
#endif
    }
#endif
}
