#include "bk_private/bk_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>

extern void user_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);
extern int bk_cli_init(void);
extern void bk_set_jtag_mode(uint32_t cpu_id, uint32_t group_id);

void user_app_main(void)
{
	extern void wdt_init(void);
	extern bool ate_is_enabled(void);
	if (ate_is_enabled() == 0)
	{
		wdt_init();
		bk_printf("wdt debug enabled\n");
	}

#if CONFIG_AT
	extern int atsvr_app_init(void);
	bk_set_printf_sync(true);
	if(0 != atsvr_app_init())
		return;
#endif
}

int main(void)
{
#if (CONFIG_SYS_CPU0)
	rtos_set_user_app_entry((beken_thread_function_t)user_app_main);
	// bk_set_printf_sync(true);
	// shell_set_log_level(BK_LOG_WARN);
#endif
	bk_init();

	return 0;
}