# Folders
SUBDIRS = libsdcc-z80 libcpm3-z80 libpartner libgpx

# Rules.
.PHONY: all
all:	$(SUBDIRS) 

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ BUILD_DIR=$(BUILD_DIR) BIN_DIR=$(BIN_DIR) PLATFORM=$(PLATFORM)
