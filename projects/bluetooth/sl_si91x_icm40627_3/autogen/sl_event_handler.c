#include "sl_event_handler.h"

#include "sl_si91x_clock_manager.h"
#include "rsi_board.h"
#include "rsi_debug.h"

void sli_driver_permanent_allocation(void)
{
}

void sli_service_permanent_allocation(void)
{
}

void sli_stack_permanent_allocation(void)
{
}

void sli_internal_permanent_allocation(void)
{
}

void sl_platform_init(void)
{
  sl_si91x_clock_manager_init();
  RSI_Board_Init();
  DEBUGINIT();
}

void sli_internal_init_early(void)
{
}

void sl_driver_init(void)
{
}

void sl_service_init(void)
{
}

void sl_stack_init(void)
{
}

void sl_internal_app_init(void)
{
}

void sli_platform_process_action(void)
{
}

void sli_service_process_action(void)
{
}

void sli_stack_process_action(void)
{
}

void sli_internal_app_process_action(void)
{
}

