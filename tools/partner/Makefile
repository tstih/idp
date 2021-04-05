# Folders
IDIR	= include
LDIR	= lib
ODIR	= build
BDIR	= bin

# Special tools.
CC		= g++
CFLAGS	= -g -I$(IDIR) -I$(LDIR)

# Source files.
INC		= $(wildcard *.h) $(wildcard *.hpp) $(wildcard */*.h) $(wildcard */*.hpp)
SRC		= $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
TARGET	= partner

# Objects.
OBJS	= $(patsubst %.o,$(ODIR)/%.o, $(shell basename $(patsubst %.cpp,%.o,$(SRC))))

# Rules.
.PHONY: all
all: subdirs $(ODIR)/$(TARGET)
	cp $(ODIR)/$(TARGET) $(BDIR)/$(TARGET)
	cp ./partner.rom $(BDIR)/partner.rom
	
#.PHONY: subdirs
subdirs:
	mkdir -p $(ODIR)
	mkdir -p $(BDIR)
	
$(ODIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	
$(ODIR)/%.o: $(SRC)
	$(CC) -c -o $@ $< $(CFLAGS)

$(SRC): $(INC)

.PHONY: clean
clean:
	rm -r $(ODIR)
	rm -r $(BDIR)
