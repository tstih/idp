# Special tools.
LD		= sdcc
LDFLAGS	= -mz80 -Wl -y \
	--code-loc 0x100 --no-std-crt0 --nostdlib --nostdinc \
	-L$(BUILD_DIR) -llibsdcc -llibccpm 

# Source files.
CRT0	=	crt0cpm
SRC		=	$(wildcard *.c)
TARGETS	=	$(patsubst %.c,$(BUILD_DIR)/%.ihx,$(SRC))

# Rules.
.PHONY: all
all: $(TARGETS)

$(BUILD_DIR)/%.ihx: $(BUILD_DIR)/%.rel
	$(LD) $(LDFLAGS) -o $@ $(BUILD_DIR)/$(CRT0).rel $<

$(BUILD_DIR)/$(CRT0).rel: $(SCR_DIR)/$(CRT0).s
	$(AS) $(ASFLAGS) -o $(BUILD_DIR)/$(CRT0).rel $(SCR_DIR)/$(CRT0).s

$(BUILD_DIR)/%.rel: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/%.rel: %.h