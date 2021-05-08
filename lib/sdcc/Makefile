# Source code.
SRCS	=	$(wildcard *.s)
OBJS	=	$(patsubst %.s,$(BUILD_DIR)/%.rel,$(SRCS))

# Target is libcpm.
TARGET = libsdcc

# Rules.
.PHONY: all
all:	$(BUILD_DIR)/$(TARGET).lib 

$(BUILD_DIR)/$(TARGET).lib: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/%.rel:	%.s
	$(AS) $(ASFLAGS) $(BUILD_DIR)/$(basename $*).rel $*.s

$(BUILD_DIR)/%.rel: %.h
