# We only allow compilation on linux!
ifneq ($(shell uname), Linux)
$(error OS must be Linux!)
endif

# Check if all required tools are on the system.
REQUIRED = sdcc sdar sdasz80 mkfs.cpm cpmcp gcc sed
K := $(foreach exec,$(REQUIRED),\
    $(if $(shell which $(exec)),,$(error "$(exec) not found. Please install or add to path.")))

# Global settings: folders.
ROOT = $(realpath .)
export BUILD_DIR	=	$(ROOT)/build
export BIN_DIR		=	$(ROOT)/bin
export INC_DIR		=	$(ROOT)/include 
export STD_LIB_INC	=	$(ROOT)/include/clib
export SCR_DIR		=	$(ROOT)/scripts

# Globa settings: tools.
export CC			=	sdcc
export CFLAGS		=	--std-c11 -mz80 --max-allocs-per-node 25000 -I$(STD_LIB_INC) -I. -I$(INC_DIR) --no-std-crt0 --nostdinc --nostdlib -D__ID_PARTNER__ --debug
export AS			=	sdasz80
export ASFLAGS		=	-xlos -g
export AR			=	sdar
export ARFLAGS		=	-rc

# Subfolders for make.
SUBDIRS = tools lib src test

# .COM programs from IHX.
IHX		=	$(wildcard $(BUILD_DIR)/*.ihx)
COM		=	$(patsubst %.ihx,%.com,$(IHX))

# Rules.
.PHONY: all
all:	$(BUILD_DIR) $(SUBDIRS)

.PHONY: $(BUILD_DIR)
$(BUILD_DIR):
	# Create build dir.
	mkdir -p $(BUILD_DIR)
	# Remove bin dir (we are going to write again).
	rm -f -r $(BIN_DIR)
	# And re-create!
	mkdir -p $(BIN_DIR)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@
	
.PHONY: clean
clean:
	rm -f -r $(BUILD_DIR)
	rm -f diskdefs

.PHONY: install
install: floppy $(COM) bin after

.PHONY: floppy
floppy:
	cp $(ROOT)/scripts/diskdefs .
	mkfs.cpm -f idpfdd -t $(BUILD_DIR)/fddb.img

# Make .COM files (for CP/M).
.PHONY: $(COM)
$(COM):
	$(BUILD_DIR)/load $(basename $@)
	cpmcp -f idpfdd $(BUILD_DIR)/fddb.img $@ 0:$(@F)

.PHONY: bin
bin:
	cp $(BUILD_DIR)/crt0cpm.rel $(BIN_DIR)
	cp $(BUILD_DIR)/*.lib $(BIN_DIR)
	cp $(BUILD_DIR)/*.com $(BIN_DIR)
	cp $(BUILD_DIR)/fddb.img $(BIN_DIR)

.PHONY: after
after:
	rm -f diskdefs

# Special build shortcut. Don't use!
.PHONY: dex
dex:	install
	cp $(BUILD_DIR)/fddb.img ~/Dex/fddb.img
