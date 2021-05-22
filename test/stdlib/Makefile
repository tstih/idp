# Special tools.
LD		= sdcc
LDFLAGS	= -mz80 -Wl -y \
	--code-loc 0x100 --no-std-crt0 --nostdlib --nostdinc \
	-L$(BUILD_DIR) -llibsdcc -llibcpm 

# Source files.
CRT0	=	crt0cpm
SRC		=	$(wildcard *.c)
OBJS	=	$(BUILD_DIR)/$(CRT0).rel $(patsubst %.c,$(BUILD_DIR)/%.rel,$(SRC))
TARGET	=	std-test

# Rules.
.PHONY: all
all: $(BUILD_DIR)/$(TARGET).ihx

$(BUILD_DIR)/$(TARGET).ihx: $(OBJS)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET).ihx $(OBJS)

$(BUILD_DIR)/$(CRT0).rel: $(SCR_DIR)/$(CRT0).s
	$(AS) $(ASFLAGS) -o $(BUILD_DIR)/$(CRT0).rel $(SCR_DIR)/$(CRT0).s

$(BUILD_DIR)/%.rel: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/%.rel: %.h
