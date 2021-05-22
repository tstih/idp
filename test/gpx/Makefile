# Special tools.
LD		= sdcc
LDFLAGS	= -mz80 --no-std-crt0 --code-loc 0x100 -l$(BUILD_DIR)/libgpx.lib -Wl -y 

# Source files.
CRT0	=	crt0cpm
C_SRCS	=	$(wildcard *.c)
S_SRCS	=	$(wildcard *.s)
OBJS	=	$(BUILD_DIR)/$(CRT0).rel $(patsubst %.c,$(BUILD_DIR)/%.rel,$(C_SRCS)) $(patsubst %.s,$(BUILD_DIR)/%.rel,$(S_SRCS))
TARGET	=	gpx-test

# Rules.
.PHONY: all
all: $(BUILD_DIR)/$(TARGET).ihx

$(BUILD_DIR)/$(TARGET).ihx: $(OBJS)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET).ihx $(OBJS)

(BUILD_DIR)/$(CRT0).rel: $(SCR_DIR)/$(CRT0).s
	$(AS) $(ASFLAGS) -o $(BUILD_DIR)/$(CRT0).rel $(SCR_DIR)/$(CRT0).s

$(BUILD_DIR)/%.rel: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(BUILD_DIR)/%.rel:	%.s
	$(AS) $(ASFLAGS) $(BUILD_DIR)/$(basename $*).rel $*.s

$(BUILD_DIR)/%.rel: %.h
