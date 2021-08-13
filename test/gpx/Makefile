# Special tools.
LD		= sdcc
LDFLAGS	= -mz80 -Wl -y \
	--code-loc 0x100 --no-std-crt0 --nostdlib --nostdinc \
	-L$(BUILD_DIR) -llibsdcc-z80 -llibcpm3-z80 -llibpartner -llibgpx

# Data segment fix (link again)
L2		= sdldz80
L2FLAGS	= -nf
L2FIX	= sed '/-b _DATA = 0x8000/d'

# Source files.
CRT0	=	crt0cpm3-z80
SRC		=	$(wildcard *.c)
TARGETS	=	$(patsubst %.c,$(BUILD_DIR)/%.ihx,$(SRC))

# Rules.
.PHONY: all
all: $(TARGETS)
	cp *.tst $(BUILD_DIR) 2>/dev/null || :

$(BUILD_DIR)/%.ihx: $(BUILD_DIR)/%.rel
	$(LD) $(LDFLAGS) -o $@ $(BUILD_DIR)/$(CRT0).rel $<
	$(L2FIX) $(basename $<).lk > $(basename $<).link
	$(L2) $(L2FLAGS) $(basename $<).link


$(BUILD_DIR)/%.rel: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/%.rel: %.h