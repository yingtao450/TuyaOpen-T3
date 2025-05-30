set(incs
	include
	)


if(CONFIG_FREERTOS)
	list(APPEND incs freertos)
	if(CONFIG_FREERTOS_V9)
		set(srcs
			freertos/v9/rtos_pub.c
			freertos/v9/rtos_debug.c
			freertos/mem_arch.c
			platform_stub.c
			freertos/str_arch.c
			)
	elseif(CONFIG_FREERTOS_V10)
		set(srcs
			freertos/v10/rtos_pub.c
			freertos/v10/rtos_debug.c
			freertos/mem_arch.c
			platform_stub.c
			freertos/str_arch.c
		)
		if(CONFIG_ARCH_RISCV)
			list(APPEND srcs
				freertos/FreeRTOS_tick_config.c
			)
		endif()
	elseif(CONFIG_FREERTOS_SMP)
		set(srcs
			freertos/v10/rtos_pub_smp.c
			freertos/v10/rtos_debug.c
			freertos/mem_arch.c
			platform_stub.c
			freertos/str_arch.c
		)
		if(CONFIG_ARCH_RISCV)
			list(APPEND srcs
				freertos/FreeRTOS_tick_config.c
			)
		endif()
	endif()

	if(CONFIG_FULLY_HOSTED OR CONFIG_MEM_MGMT)
		list(APPEND srcs freertos/mmgmt.c)
	endif()

	if(CONFIG_FREERTOS_POSIX)
		list(APPEND incs
					freertos/posix/include
					freertos/posix/include/private
					freertos/posix/freertos_impl/include
					freertos/posix/freertos_impl/include/portable
		)
		list(APPEND srcs
					freertos/posix/freertos_impl/source/FreeRTOS_POSIX_timer.c
					freertos/posix/freertos_impl/source/FreeRTOS_POSIX_pthread_mutex.c
					freertos/posix/freertos_impl/source/FreeRTOS_POSIX_sched.c
					freertos/posix/freertos_impl/source/FreeRTOS_POSIX_pthread_barrier.c
					freertos/posix/freertos_impl/source/FreeRTOS_POSIX_unistd.c
					freertos/posix/freertos_impl/source/FreeRTOS_POSIX_pthread.c
					freertos/posix/freertos_impl/source/FreeRTOS_POSIX_utils.c
					freertos/posix/freertos_impl/source/FreeRTOS_POSIX_pthread_cond.c
					freertos/posix/freertos_impl/source/FreeRTOS_POSIX_semaphore.c
					freertos/posix/freertos_impl/source/FreeRTOS_POSIX_mqueue.c
					freertos/posix/freertos_impl/source/FreeRTOS_POSIX_clock.c
		)
	endif()

	if(CONFIG_FREERTOS_TRACE)
		list(APPEND incs
					freertos/trace/include
					freertos/trace/config
		)
		list(APPEND srcs
					freertos/trace/trcKernelPort.c
					freertos/trace/trcSnapshotRecorder.c
		)
	endif()

elseif(CONFIG_LITEOS_M)
	list(APPEND incs liteos_m)
	set(srcs
		liteos_m/os_clock.c
		platform_stub.c
		liteos_m/port.c
		liteos_m/rtos_pub.c
		liteos_m/mem_arch.c
		liteos_m/str_arch.c
		# liteos_m/wifi_ui_extra.c
	)
	set_source_files_properties("${CMAKE_CURRENT_LIST_DIR}/liteos_m/port.c" PROPERTIES COMPILE_FLAGS "-marm")
elseif(CONFIG_LITEOS_M_V3)
	list(APPEND incs liteos_m_v3)
	set(srcs
		liteos_m_v3/os_clock.c
		platform_stub.c
		liteos_m_v3/port.c
		liteos_m_v3/rtos_pub.c
		liteos_m_v3/mem_arch.c
		liteos_m_v3/str_arch.c
	)
elseif(CONFIG_NON_OS)
	list(APPEND incs non_os)
	set(srcs
		non_os/rtos_pub.c
		non_os/mem_arch.c
		non_os/heap_4.c
		non_os/port.c
		non_os/str_arch.c
	)
	set_source_files_properties("${CMAKE_CURRENT_LIST_DIR}/non_os/port.c" PROPERTIES COMPILE_FLAGS "-marm")
endif()

	list(APPEND srcs rtos_ext.c)

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" REQUIRES cmsis PRIV_REQUIRES bk_common riscv)

