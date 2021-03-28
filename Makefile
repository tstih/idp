# We only allow compilation on linux!
ifneq ($(shell uname), Linux)
$(error OS must be Linux!)
endif

# Check if all required tools are on the system.
REQUIRED = sdcc sdar sdasz80 mkfs.cpm cpmcp
K := $(foreach exec,$(REQUIRED),\
    $(if $(shell which $(exec)),,$(error "$(exec) not found. Please install or add to path.")))

# Global settings: folders.
ROOT = $(realpath .)
export BUILD_DIR=	$(ROOT)/build
export INC_DIR	=	$(ROOT)/include	

# Globa settings: tools.
export CC	=	sdcc
export CFLAGS	=	--std-sdcc99 -mz80 -I. -I$(INC_DIR) --no-std-crt0
export AS	=	sdasz80
export ASFLAGS	=	-xlos -g
export AR	=	sdar
export ARFLAGS	=	-rc

# Subfolders for make.
SUBDIRS = libs
HELLO   = hello.com

# Rules.
.PHONY: all
all:	$(BUILD_DIR) $(SUBDIRS)

.PHONY: $(BUILD_DIR)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@
	
.PHONY: clean
clean:
	rm -f -r $(BUILD_DIR)
	rm -f diskdefs

.PHONY: install
install:
	cp $(ROOT)/scripts/diskdefs .
	mkfs.cpm -f idpfdd -t $(BUILD_DIR)/fddb.img
	cpmcp -f idpfdd $(BUILD_DIR)/fddb.img $(BUILD_DIR)/$(HELLO) 0:$(HELLO)
	rm -f diskdefs
