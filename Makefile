# Makefile
# As part of the Avery project
# Created by Maxims Enterprise in 2024

# Include the constants from constants.mk
include build/constants.mk

.PHONY: all clean run test

# Define the default target
all: $(FINAL)

# Define the final target
$(FINAL): $(BIN)
	cp $(BIN) iso/boot/avery.bin
	$(GRUB) $(GRUB_FLAGS)

# Define the binary target
$(BIN): $(C_OBJS) $(CXX_OBJS) $(AS_OBJS) $(BOOT_OBJS)	
	$(LD) $(LDFLAGS) -o $@ $^

# Define the C object files
$(OBJ)/%.o: $(SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $<

# Define the C++ object files
$(OBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Define the assembly object files
$(OBJ)/%.o: $(SRC)/%.asm
	mkdir -p $(@D)	
	$(AS) $(ASFLAGS) -o $@ $<

# Define the boot object files
$(OBJ)/%.o: $(BOOT)/%.asm
	mkdir -p $(@D)
	$(AS) $(ASFLAGS) -o $@ $<

# Define the clean target
clean:
	rm -rf $(BIN) $(OBJ) $(FINAL)
	mkdir -p $(BIN) $(OBJ)

# Define the debug target
debug:
	override CFLAGS += -D DEBUG
	override CXXFLAGS += -D DEBUG
	$(MAKE) run

# Define the run target for QEMU testing
run: $(FINAL)
	qemu-system-x86_64 -cdrom $(FINAL)
