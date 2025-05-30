export PROPERTIES_PROJECT_DIR := projects/properties_libs
ARMINO_DIR := $(shell pwd)
ARMINO_BOOTLOADER := $(ARMINO_DIR)/properties/modules/bootloader/aboot/monitor_bootloader/
ARMINO_TOOL := @$(ARMINO_DIR)/tools/build_tools/armino
ARMINO_TOOL_WRAPPER := @$(ARMINO_DIR)/tools/build_tools/build.sh
# 1. soc_targets contains all supported SoCs
# 2. cmake_supported_targets contains all targets that can directly
#    passed to armino cmake build system
# 3. cmake_not_supported_targets contains all targets:
#    3.1> armino cmake doesn't support it, only implemented in this
#         Makefile
#    3.2> armino cmake supports it, but has different target name
soc_targets := $(shell find  middleware/soc/ -name "*.defconfig" -exec basename {} \; | cut -f1 -d ".")
properties_lib_targets := $(subst bk, libbk, $(soc_targets))
rel_targets := $(subst bk, relbk, $(soc_targets))
clean_targets := $(subst bk, cleanbk, $(soc_targets))
doc_targets := $(subst bk, docbk, $(soc_targets))
cmake_supported_targets := menuconfig doc
cmake_not_supported_targets = help clean
all_targets = cmake_not_supported_targets soc_targets cmake_supported_targets
export SOC_SUPPORTED_TARGETS := ${soc_targets}

export ARMINO_SOC := $(findstring $(MAKECMDGOALS), $(soc_targets))
export ARMINO_SOC_LIB := $(findstring $(MAKECMDGOALS), $(properties_lib_targets))
export CMD_TARGET := $(MAKECMDGOALS)

ifeq ("$(APP_VERSION)", "")
	export APP_VERSION := unknow
else
	export APP_VERSION := $(APP_VERSION)
endif

ifeq ("$(PROJECT)", "")
	ifneq ("$(INTERNAL)", "")
		export PROJECT := $(INTERNAL)
		export PROJECT_DIR := properties/projects/$(PROJECT)
		export PROPERTIES_PROJECT_DIR := properties/projects/properties_libs
	else
		export PROJECT := app
	endif
else
	export PROJECT := $(PROJECT)
endif

ifeq ("$(PROJECT_LIBS)", "")
	export PROJECT_LIBS := $(PROJECT)
else
	export PROJECT_LIBS := $(PROJECT_LIBS)
endif

ifeq ("$(PROJECT_DIR)", "")
	export PROJECT_DIR := projects/$(PROJECT)
else
	export PROJECT_DIR := $(PROJECT_DIR)
endif

ifeq ("$(ARMINO_SOC)", "")
ifeq ("$(ARMINO_SOC_LIB)", "")
	ARMINO_SOC := bk7236
	ARMINO_TARGET := $(MAKECMDGOALS)
endif
else
	ARMINO_TARGET := build
endif

PROJECT_NAME := $(notdir $(PROJECT_DIR))
ifdef BUILD_DIR
	PROJECT_BUILD_DIR := $(BUILD_DIR)
else
	PROJECT_BUILD_DIR := build/$(PROJECT_NAME)
endif

# overwrite project config
-include $(ARMINO_DIR)/middleware/soc/$(ARMINO_SOC)/soc_config.mk
-include $(ARMINO_DIR)/components/part_table/part_table.mk
-include $(ARMINO_DIR)/$(PROJECT_DIR)/pj_config.mk

.PHONY: all_targets

help:
	@echo ""
	@echo " make bkxxx - build soc bkxxx"
	@echo " make build - fast build last soc"
	@echo " make all - build all soc"
	@echo " make libbkxxx - build properties libs for soc bkxxx"
	@echo " make liball - build all properties libs for all soc"
	@echo " make menuconfig - confiure armino"
	@echo " make clean - clean build"
	@echo " make help - display this help info"
	@echo " make doc - generate all doc"
	@echo " make docbk72xx - generate doc for bk72xx"
	@echo ""

common:
	@echo "ARMINO_SOC is set to $(ARMINO_SOC)"
	@echo "ARMINO_TARGET is set to $(ARMINO_TARGET)"
	@echo "armino project path=$(PROJECT_DIR)"
	@echo "armino path=$(ARMINO_DIR)"
	@echo "armino build path=$(ARMINO_DIR)/$(PROJECT_BUILD_DIR)"
	@export ARMINO_PATH=$(ARMINO_DIR)
	@export APP_NAME=$(APP_NAME)

has_lib_src := $(shell $(ARMINO_DIR)/tools/build_tools/detect_internal_lib_src.py)

bootloader:
ifeq ($(has_lib_src), 1)
ifneq ($(findstring $(ARMINO_SOC), $(PART_TABLE_SUPPORTED_TARGETS)),)
ifneq ($(findstring $(PROJECT), $(PART_TABLE_SUPPORTED_PROJECTS)),)
	@$(ARMINO_TOOL_PART_TABLE) $(PARTITIONS_CSV_FILE) --to-json=$(BOOTLOADER_JSON) $(PARTITIONS_ARGS) $(BOOTLOADER_JSON_INSEQ)
	@cp $(BOOTLOADER_JSON) $(BOOTLOADER_JSON_OLD)
endif
endif
	make clean -C ${ARMINO_BOOTLOADER}
	make -C ${ARMINO_BOOTLOADER}
endif

ifeq ($(has_lib_src), 1)
ifneq ("$(APP_VERSION)", "verify")
$(properties_lib_targets): common
ifneq ("$(ARMINO_TARGET)", "doc")
	@$(ARMINO_TOOL_WRAPPER) $(ARMINO_DIR) $(PROJECT_DIR) $(PROJECT_BUILD_DIR) $@
else
	@echo "skip make doc"
endif
endif
endif

liball: $(properties_lib_targets)


$(soc_targets): ${PRE_BUILD_TARGET}
ifneq ($(findstring $(ARMINO_SOC), $(PART_TABLE_SUPPORTED_TARGETS)),)
ifneq ($(findstring $(PROJECT), $(PART_TABLE_SUPPORTED_PROJECTS)),)
	@$(ARMINO_TOOL_PART_TABLE) $(PARTITIONS_CSV_FILE) --to-json=$(BOOTLOADER_JSON) $(PARTITIONS_ARGS) $(BOOTLOADER_JSON_INSEQ)
	@cp $(BOOTLOADER_JSON) $(BOOTLOADER_JSON_PACK)
else
ifneq ($(wildcard $(BOOTLOADER_JSON_PACK)),)
	$(warning "$(BOOTLOADER_JSON_PACK) is exist")
	@rm -rf $(BOOTLOADER_JSON_PACK)
else
	$(warning "$(BOOTLOADER_JSON_PACK) is not exsit")
endif
endif
endif
	@$(ARMINO_TOOL_WRAPPER) $(ARMINO_DIR) $(PROJECT_DIR) $(PROJECT_BUILD_DIR) $@

all: $(soc_targets)

$(rel_targets):
	@$(ARMINO_TOOL_WRAPPER) $(ARMINO_DIR) $(PROJECT_DIR) $(PROJECT_NAME) $@

relall: $(rel_targets)

$(cmake_supported_targets): common
ifneq ("$(ARMINO_TARGET)", "doc")
	@$(ARMINO_TOOL) -B ./$(PROJECT_BUILD_DIR) -P ./$(PROJECT_DIR) $@
else
	@echo "skip make doc"
endif

$(clean_targets):
	@$(ARMINO_TOOL_WRAPPER) $(ARMINO_DIR) $(PROJECT_DIR) $(PROJECT_NAME) $@

$(doc_targets):
	@$(ARMINO_TOOL_WRAPPER) $(ARMINO_DIR) $(PROJECT_DIR) $(PROJECT_NAME) $@

clean:
	@echo "rm all files generated by part_table_tool"
	-@$(ARMINO_TOOL_PART_TABLE) $(DEFAULT_CSV_FILE) $(PARTITIONS_ARGS) $(CLEAN_ALLFILE_INSEQ)
	@echo "rm -rf ./build"
	@python3 tools/build_tools/armino_doc.py clean
	@rm -rf ./build

