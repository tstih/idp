# Special tools.
LD		= sdcc
LDFLAGS	= -mz80 --no-std-crt0 --code-loc 0x100

# Source files.
SRC	=	$(wildcard *.c)
OBJS	=	$(BUILD_DIR)/cpm0.rel $(patsubst %.c,$(BUILD_DIR)/%.rel,$(SRC))

# Rules.
.PHONY: all
all: $(BUILD_DIR)/hello.ihx

$(BUILD_DIR)/hello.ihx: $(OBJS)
	$(LD)  $(LDFLAGS) -o $(BUILD_DIR)/hello.ihx $(OBJS)

$(BUILD_DIR)/cpm0.rel: $(SCR_DIR)/cpm0.s
	$(AS) $(ASFLAGS) -o $(BUILD_DIR)/cpm0.rel $(SCR_DIR)/cpm0.s

$(BUILD_DIR)/%.rel: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/%.rel: %.h
