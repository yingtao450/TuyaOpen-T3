armino_build_get_property(target ARMINO_SOC)

if(NOT "${target}" STREQUAL ${COMPONENT_NAME})
    return()
endif()

set(incs
	soc
	hal
	./
	)
set(srcs "stub.c")
list(APPEND srcs
	"mpu_cfg.c"
	"interrupts.c"
	"hal/psram_ll.c"
	"hal/pwm_hal_debug.c"
	"hal/pwm_ll.c"
	"hal/timer_hal_debug.c"
	"hal/timer_ll.c"
	"hal/timer_hal.c"
	"hal/gpio_hal_debug.c"
	"hal/gpio_ll.c"
	"hal/dma_hal_debug.c"
	"hal/dma_ll.c"
	"hal/uart_hal_debug.c"
	"hal/uart_ll.c"
	"hal/icu_hal_debug.c"
	"hal/icu_ll.c"
	"hal/wdt_hal_debug.c"
	"hal/wdt_ll.c"
	"hal/trng_hal_debug.c"
	"hal/trng_ll.c"
	"hal/efuse_hal_debug.c"
	"hal/efuse_ll.c"
	"hal/adc_hal_debug.c"
	"hal/adc_ll.c"
	"hal/spi_hal_debug.c"
	"hal/spi_ll.c"
	"hal/sys_hal.c"
	"hal/sys_hal_debug.c"
	"hal/sys_pm_hal.c"
	"hal/sys_pm_hal_ctrl.c"
	"hal/sys_pm_hal_debug.c"
	"hal/i2c_hal_debug.c"
	"hal/i2c_ll.c"
	"hal/jpeg_hal_debug.c"
	"hal/jpeg_ll.c"
	"hal/calendar_hal_debug.c"
	"hal/calendar_ll.c"
	"hal/flash_hal_debug.c"
	"hal/flash_ll.c"
	"hal/aon_rtc_hal_debug.c"
	"hal/aon_pmu_hal.c"
	"hal/aon_pmu_hal_debug.c"
	"hal/sdio_hal_debug.c"
	"hal/dma2d_hal_debug.c"
	"hal/yuv_buf_hal_debug.c"
	"hal/sys_clock_hal_debug.c"
	"hal/qspi_hal_debug.c"
	"hal/i2s_hal.c"
	"hal/sdmadc_hal_debug.c"
	"hal/ckmn_ll.c"
	"hal/ckmn_hal_debug.c"
	"hal/lin_ll.c"
	"hal/lin_hal_debug.c"
)
# Modified by TUYA Start
armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" REQUIRES partitions tuya_os_adapter)
# Modified by TUYA End
armino_build_get_property(sdkconfig_header SDKCONFIG_HEADER)
armino_build_get_property(config_dir CONFIG_DIR)

#TODO add support to generate ELF with/without bootloader

set(LD_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LD_FILE ${LD_DIR}/${target}_bsp.ld)

if(EXISTS "${armino_path}/$ENV{PROJECT_DIR}/${target}_bsp.ld")
	set(LD_FILE ${armino_path}/$ENV{PROJECT_DIR}/${target}_bsp.ld)
endif()

LOGI("[Select ld file]: ${LD_FILE}.")

add_custom_command(
	OUTPUT ${target}_out.ld
	COMMAND "${CMAKE_C_COMPILER}" -P -x c -E - < ${LD_FILE} -o ${target}_out.ld -I ${armino_path}/include/ -I ${config_dir} -I ${armino_path}/middleware/boards/bk7258/partitions -I ${CMAKE_CURRENT_BINARY_DIR}/../partitions/_build
	MAIN_DEPENDENCY ${LD_DIR}/${target}.ld ${sdkconfig_header}
	DEPENDS generated_files
	COMMENT "Generating linker script..."
	VERBATIM)

add_custom_target(${target}_linker_script DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${target}_out.ld)
add_dependencies(${COMPONENT_LIB} ${target}_linker_script)

target_linker_script(${COMPONENT_LIB} INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/${target}_out.ld")
