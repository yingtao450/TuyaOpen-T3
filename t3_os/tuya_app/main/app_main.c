#include "bk_private/bk_init.h"
#include <components/system.h>
#include <components/ate.h>
#include <os/os.h>
#include <components/shell_task.h>
#include "tuya_cloud_types.h"

extern void tuya_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);

#include "flash.h"
static void __get_flash_id(void)
{
    uint32_t flash_size;
    uint32_t flash_id = bk_flash_get_id();

    flash_size = 2 << ((flash_id & 0xff) - 1);

    bk_printf("flash id: 0x%08x, flash size: %x / %dM\r\n", flash_id, flash_size, flash_size / 1048576);
}

int main(void)
{
    __get_flash_id();

#if (CONFIG_SYS_CPU0)

    if (!ate_is_enabled()) {
        bk_printf("go to tuya\r\n");
        rtos_set_user_app_entry((beken_thread_function_t)tuya_app_main);
    }

    // bk_set_printf_sync(true);
    // shell_set_log_level(BK_LOG_WARN);
#endif

    bk_init();

    return 0;
}
