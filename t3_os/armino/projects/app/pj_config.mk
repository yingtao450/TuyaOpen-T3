PRE_BUILD_TARGET :=
ifeq ($(SUPPORT_DUAL_CORE),true)
	PRE_BUILD_TARGET += $(ARMINO_SOC)_cp1
endif

ifeq ($(SUPPORT_TRIPLE_CORE),true)
	PRE_BUILD_TARGET += $(ARMINO_SOC)_cp1
	PRE_BUILD_TARGET += $(ARMINO_SOC)_cp2
endif

ifeq ($(SUPPORT_BOOTLOADER),true)
	PRE_BUILD_TARGET += bootloader
endif