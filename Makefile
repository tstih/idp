# We only allow compilation on linux!
ifneq ($(shell uname), Linux)
$(error OS must be Linux!)
endif

# Check if all required tools are on the system.
REQUIRED = sdcc sdar sdasz80 sdldz80 mkfs.cpm cpmcp gcc sed
K := $(foreach exec,$(REQUIRED),\
    $(if $(shell which $(exec)),,$(error "$(exec) not found. Please install or add to path.")))

# Global settings: folders.
export ROOT 		= 	$(realpath .)
export BUILD_DIR	=	$(ROOT)/build
export BIN_DIR		=	$(ROOT)/bin
export INC_DIR		=	$(ROOT)/include $(ROOT)/lib/libcpm3-z80/include $(ROOT)/lib/libgpx/include
export SCR_DIR		=	$(ROOT)/scripts

# Build reduced version of libraries?
export SLIM			=	

# Globa settings: tools.
export CC			=	sdcc
export CFLAGS		=	--std-c11 -mz80 -I. $(addprefix -I,$(INC_DIR)) --no-std-crt0 --nostdinc --nostdlib --debug -D SLIM=$(SLIM)
export AS			=	sdasz80
export ASFLAGS		=	-xlos -g
export AR			=	sdar
export ARFLAGS		=	-rc

# Subfolders for make.
SUBDIRS = lib src test
TOOLDIRS = tools/gpxtools

# Program data (to deploy): IHX->COM, TST
IHX					=	$(wildcard $(BUILD_DIR)/*.ihx)
COM					=	$(patsubst %.ihx,%.com,$(IHX))
APPS 				= 	$(filter-out %test.com %xp.com,$(COM))
TESTS				= 	$(filter %test.com %xp.com,$(COM))
TEST_DATA			=	$(wildcard $(BUILD_DIR)/*.tst) $(wildcard $(BUILD_DIR)/*.gph)

# CP/M User areas.
APP_USER			=	0
TEST_USER			=	15

# Rules.
.PHONY: all
all:	$(BUILD_DIR) $(SUBDIRS)

.PHONY: tools
tools:	$(BUILD_DIR) $(TOOLDIRS)

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
	$(MAKE) -C $@ PLATFORM=partner SLIM=$(SLIM)
	
.PHONY: clean
clean:
	rm -f -r $(BUILD_DIR)
	rm -f diskdefs

.PHONY: install
install: floppy $(APPS) $(TESTS) $(TEST_DATA) bin hfe after

.PHONY: ccp
ccp: floppy-ccp $(APPS) $(TESTS) $(TEST_DATA) bin hfe after

.PHONY: boot
boot: floppy-boot $(APPS) $(TESTS) $(TEST_DATA) bin hfe after

.PHONY: bootg
bootg: floppy-bootg $(APPS) $(TESTS) $(TEST_DATA) bin hfe after

.PHONY: floppy
floppy:
	cp $(ROOT)/scripts/diskdefs .
	mkfs.cpm -f idpfdd -t $(BUILD_DIR)/fddb.img

.PHONY: floppy-ccp
floppy-ccp:
	cp $(ROOT)/scripts/diskdefs .
	mkfs.cpm -f idpfdd -t $(BUILD_DIR)/fddb.img
	cpmcp -f idpfdd $(BUILD_DIR)/fddb.img $(SCR_DIR)/CCP.COM $(APP_USER):CCP.COM

.PHONY: floppy-boot
floppy-boot:
	cp $(ROOT)/scripts/diskdefs .
	cp $(SCR_DIR)/boot.img $(BUILD_DIR)/fddb.img

.PHONY: floppy-bootg
floppy-bootg:
	cp $(ROOT)/scripts/diskdefs .
	cp $(SCR_DIR)/bootg.img $(BUILD_DIR)/fddb.img

.PHONY: hfe
hfe:
	-hxcfe -uselayout:IDP -conv:HXC_HFE -finput:$(BUILD_DIR)/fddb.img -foutput:$(BUILD_DIR)/fddb.hfe

.PHONY: $(APPS)
$(APPS):
	sdobjcopy -I ihex -O binary $(basename $@).ihx $(basename $@).com
	cpmcp -f idpfdd $(BUILD_DIR)/fddb.img $@ $(APP_USER):$(@F)

.PHONY: $(TESTS)
$(TESTS):
	sdobjcopy -I ihex -O binary $(basename $@).ihx $(basename $@).com
	cpmcp -f idpfdd $(BUILD_DIR)/fddb.img $@ $(TEST_USER):$(@F)

.PHONY: $(TEST_DATA)
$(TEST_DATA):
	cpmcp -f idpfdd $(BUILD_DIR)/fddb.img $@ $(TEST_USER):$(@F)


.PHONY: bin
bin:
	cp $(BUILD_DIR)/crt0cpm3-z80.rel $(BIN_DIR)
	cp $(BUILD_DIR)/*.lib $(BIN_DIR)
	cp $(BUILD_DIR)/*.com $(BIN_DIR)
	cp $(BUILD_DIR)/fddb.img $(BIN_DIR)
	cp $(BUILD_DIR)/*.tst $(BIN_DIR)

.PHONY: after
after:
	rm -f diskdefs

# Special build shortcut. Don't use!
.PHONY: dex
dex:	install
	cp $(BUILD_DIR)/fddb.img ~/Dex/fddb.img

.PHONY: $(TOOLDIRS)
$(TOOLDIRS):
	$(MAKE) -C $@
	
