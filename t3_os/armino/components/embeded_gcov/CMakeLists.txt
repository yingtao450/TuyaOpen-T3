set(incs
	code
	)

if (CONFIG_GCOV)
set(srcs
	code/gcov_public.c
	code/gcov_gcc.c
	code/gcov_printf.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
