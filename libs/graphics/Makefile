# Source files.
C_SRCS	=	$(wildcard *.c)
S_SRCS	=	$(wildcard *.s)
OBJS	=	$(patsubst %.c,$(BUILD_DIR)/%.rel,$(C_SRCS)) $(patsubst %.s,$(BUILD_DIR)/%.rel,$(S_SRCS))

# Subdirectories must contain makes.
SUBDIRS	=	$(wildcard */.)

# Rules.
.PHONY: all
all:	$(SUBDIRS) $(BUILD_DIR)/graphics.lib 

$(BUILD_DIR)/graphics.lib: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/%.rel:	%.s
	$(AS) $(ASFLAGS) $(BUILD_DIR)/$(basename $*).rel $*.s

$(BUILD_DIR)/%.rel: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/%.rel: %.h

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@
