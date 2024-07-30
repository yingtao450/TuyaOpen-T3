set(incs)
set(srcs)

macro(FIND_INCLUDE_DIR result curdir)
    file(GLOB_RECURSE children "${curdir}/*.h")
    set(dirlist "")
    foreach(child ${children})
        string(REGEX REPLACE "(.*)/.*" "\\1" DIR_NAME ${child})
        if((IS_DIRECTORY ${DIR_NAME}) AND (NOT (${DIR_NAME} IN_LIST dirlist)))
            list(APPEND dirlist ${DIR_NAME})
        endif()
    endforeach()
    set(${result} ${dirlist})
    #message(STATUS "result=${dirlist}")
endmacro()

#add tuya include dir
set(INCLUDE_DIR_LIST_TUYA $ENV{TUYA_HEADER_DIR})
string(REPLACE " " ";" INCLUDE_DIR_LIST_TUYA "${INCLUDE_DIR_LIST_TUYA}")
set(incs ${INCLUDE_DIR_LIST_TUYA})

list(APPEND srcs
	"src/driver/tkl_adc.c"
	"src/driver/tkl_flash.c"
	"src/driver/tkl_gpio.c"
	"src/driver/tkl_hci.c"
	"src/driver/tkl_pwm.c"
	"src/driver/tkl_rtc.c"
	"src/driver/tkl_spi.c"
	"src/driver/tkl_i2c.c"
	"src/driver/tkl_timer.c"
	"src/driver/tkl_uart.c"
	"src/driver/tkl_watchdog.c"
	"src/driver/tkl_wifi.c"
	"src/system/tkl_memory.c"
	"src/system/tkl_mutex.c"
	"src/system/tkl_network.c"
	"src/system/tkl_ota.c"
	"src/system/tkl_output.c"
	"src/system/tkl_queue.c"
	"src/system/tkl_semaphore.c"
	"src/system/tkl_sleep.c"
	"src/system/tkl_system.c"
	"src/system/tkl_thread.c"
	"src/driver/tkl_pinmux.c"
	"src/driver/tkl_wakeup.c"
)

file(GLOB_RECURSE utilities_srcs "$ENV{ARMINO_PATH}/../../tuyaos/tuyaos_adapter/include/utilities/src/*.c")
list(APPEND srcs ${utilities_srcs})

#add incs srcs
armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" REQUIRES bk_common bk_log lwip_intf_v2_1)

#add tuya lib
set(tuya_libs "$ENV{TUYA_LIBS}")
string(REPLACE " " ";" tuya_libs "${tuya_libs}")
foreach(lib ${tuya_libs})	
	set(lib_name "lib${lib}.a")
    add_prebuilt_library(${lib_name} "$ENV{TUYA_LIBS_DIR}/${lib_name}")
    target_link_libraries(${COMPONENT_LIB} INTERFACE ${lib_name})
endforeach()