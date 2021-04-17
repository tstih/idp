# We only allow compilation on linux!
ifneq ($(shell uname), Linux)
$(error OS must be Linux!)
endif

# Check if all required tools are on the system.
REQUIRED = sdcc sdar sdasz80 mkfs.cpm cpmcp gcc
K := $(foreach exec,$(REQUIRED),\
    $(if $(shell which $(exec)),,$(error "$(exec) not found. Please install or add to path.")))

# Global settings: folders.
ROOT = $(realpath .)
export BUILD_DIR=	$(ROOT)/build
export BIN_DIR	=	$(ROOT)/bin
export INC_DIR	=	$(ROOT)/include
export SCR_DIR	=	$(ROOT)/scripts

# Globa settings: tools.
export CC		=	sdcc
export CFLAGS	=	--std-sdcc99 -mz80 -I. -I$(INC_DIR) --no-std-crt0 -D__ID_PARTNER__ --debug
export AS		=	sdasz80
export ASFLAGS	=	-xlos -g
export AR		=	sdar
export ARFLAGS	=	-rc

# Subfolders for make.
SUBDIRS = tools lib src

# Rules.
.PHONY: all
all:	$(BUILD_DIR) $(SUBDIRS)

.PHONY: $(BUILD_DIR)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@
	
.PHONY: clean
clean:
	rm -f -r $(BUILD_DIR)
	rm -f -r $(BIN_DIR)
	rm -f diskdefs

.PHONY: install
install: all
	# Make .COM files (for CP/M).
	$(BUILD_DIR)/load $(BUILD_DIR)/hello
	$(BUILD_DIR)/load $(BUILD_DIR)/zwin
	# Make CP/M floppy.
	cp $(ROOT)/scripts/diskdefs .
	mkfs.cpm -f idpfdd -t $(BUILD_DIR)/fddb.img
	cpmcp -f idpfdd $(BUILD_DIR)/fddb.img $(BUILD_DIR)/hello.com 0:hello.com
	cpmcp -f idpfdd $(BUILD_DIR)/fddb.img $(BUILD_DIR)/zwin.com 0:zwin.com
	rm -f diskdefs
	# And copy binaries to bin dir.
	cp $(BUILD_DIR)/*.com $(BIN_DIR)
	cp $(BUILD_DIR)/fddb.img $(BIN_DIR)
	
# Special build shortcut. Don't use!
.PHONY: dex
dex:	install
	cp $(BUILD_DIR)/fddb.img ~/Dex/fddb.img
