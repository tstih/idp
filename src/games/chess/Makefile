# Special tools.
LD		= sdcc
LDFLAGS	= -mz80 -Wl -y \
	--code-loc 0x100 --no-std-crt0 --nostdlib --nostdinc \
	-L$(BUILD_DIR) -llibsdcc -llibccpm 

# Data segment fix (link again)
L2		= sdldz80
L2FLAGS	= -nf
L2FIX	= sed '/-b _DATA = 0x8000/d'

# Source files.
CRT0	=	crt0cpm
SRC		=	$(wildcard *.c)
OBJS	=	$(BUILD_DIR)/$(CRT0).rel $(patsubst %.c,$(BUILD_DIR)/%.rel,$(SRC))
TARGET	=	chess

# Rules.
.PHONY: all
all: $(BUILD_DIR)/$(TARGET).ihx

$(BUILD_DIR)/$(TARGET).ihx: $(OBJS)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET).ihx $(OBJS)
	$(L2FIX) $(BUILD_DIR)/$(TARGET).lk > $(BUILD_DIR)/$(TARGET).link
	$(L2) $(L2FLAGS) $(BUILD_DIR)/$(TARGET).link

$(BUILD_DIR)/$(CRT0).rel: $(SCR_DIR)/$(CRT0).s
	$(AS) $(ASFLAGS) -o $(BUILD_DIR)/$(CRT0).rel $(SCR_DIR)/$(CRT0).s

$(BUILD_DIR)/%.rel: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/%.rel: %.h
