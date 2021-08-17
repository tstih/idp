# Special tools.
LD		= sdcc
LDFLAGS	= -mz80 -Wl -y \
	--code-loc 0x100 --no-std-crt0 --nostdlib --nostdinc \
	-L$(BUILD_DIR) -llibsdcc-z80 -llibcpm3-z80 -llibpartner -llibgpx -p

# Data segment fix (link again)
L2		= sdldz80
L2FLAGS	= -nf
L2FIX	= sed '/-b _DATA = 0x8000/d'

# Source files.
CRT0	=	crt0cpm3-z80
TARGET	=	gshow

# Virtual paths are all subfolders!
DIRS	=	$(wildcard */)
vpath %.c $(DIRS)
vpath %.s $(DIRS)
vpath %.h $(DIRS)

# Source files.
C_SRCS	=	$(wildcard *.c) $(wildcard */*.c)
S_SRCS	=	$(wildcard *.s) $(wildcard */*.s)
OBJS	=	$(addprefix $(BUILD_DIR)/, \
				$(notdir \
					$(patsubst %.c,%.rel,$(C_SRCS)) \
					$(patsubst %.s,%.rel,$(S_SRCS)) \
				) \
			)

# Rules.
.PHONY: all
all: $(BUILD_DIR)/$(TARGET).ihx
	cp *.g $(BUILD_DIR) 2>/dev/null || :

$(BUILD_DIR)/$(TARGET).ihx: $(OBJS)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET).ihx $(BUILD_DIR)/$(CRT0).rel $(OBJS)
	$(L2FIX) $(BUILD_DIR)/$(TARGET).lk > $(BUILD_DIR)/$(TARGET).link
	$(L2) $(L2FLAGS) $(BUILD_DIR)/$(TARGET).link

$(BUILD_DIR)/%.rel:	%.s
	$(AS) $(ASFLAGS) $(BUILD_DIR)/$(@F) $<

$(BUILD_DIR)/%.rel: %.c
	$(CC) -c -o $(BUILD_DIR)/$(@F) $< $(CFLAGS)

$(BUILD_DIR)/%.rel: %.h