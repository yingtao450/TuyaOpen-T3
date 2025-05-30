armino_build_get_property(target ARMINO_SOC)

if(NOT "${target}" STREQUAL "bk7286")
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
)

if (CONFIG_SOC_BK7286XX)
	list(APPEND srcs
		fpga_driver/drv_dma.c
		fpga_driver/drv_gpio.c
		fpga_driver/drv_ring_buffer.c
		fpga_driver/drv_system.c
		fpga_driver/drv_uart.c
	)
	list(APPEND incs
		fpga_driver/
		fpga_inc/
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" REQUIRES partitions PRIV_REQUIRES tfm)

armino_build_get_property(sdkconfig_header SDKCONFIG_HEADER)
armino_build_get_property(config_dir CONFIG_DIR)

#TODO add support to generate ELF with/without bootloader

set(LD_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if($ENV{PROJECT} STREQUAL "cpu1")
    set(LD_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${target}_cpu1.ld)
else()
	set(LD_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${target}_bsp.ld)
endif()

if(CONFIG_SOC_BK7286_SMP)
add_custom_command(
	OUTPUT ${target}_out.ld
	COMMAND "${CMAKE_C_COMPILER}" -P -x c -E - < ${LD_FILE} -o ${target}_out.ld -I ${armino_path}/components/cmsis/CMSIS_5/Device/Beken/bk7286xx/Include/ -I ${armino_path}/include/ -I ${config_dir} -I ${armino_path}/middleware/boards/bk7286/partitions -I ${CMAKE_CURRENT_BINARY_DIR}/../partitions/_build
	MAIN_DEPENDENCY ${LD_DIR}/${target}.ld ${sdkconfig_header}
	DEPENDS generated_files
	COMMENT "Generating linker script..."
	VERBATIM)
else()
add_custom_command(
	OUTPUT ${target}_out.ld
	COMMAND "${CMAKE_C_COMPILER}" -P -x c -E - < ${LD_FILE} -o ${target}_out.ld -I ${armino_path}/include/ -I ${config_dir} -I ${armino_path}/middleware/boards/bk7286/partitions -I ${CMAKE_CURRENT_BINARY_DIR}/../partitions/_build
	MAIN_DEPENDENCY ${LD_DIR}/${target}.ld ${sdkconfig_header}
	DEPENDS generated_files
	COMMENT "Generating linker script..."
	VERBATIM)
endif()

add_custom_target(${target}_linker_script DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${target}_out.ld)
add_dependencies(${COMPONENT_LIB} ${target}_linker_script)

target_linker_script(${COMPONENT_LIB} INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/${target}_out.ld")
