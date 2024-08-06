 #
 # Makefile
 # As part of the Avery project
 # Created by Maxims Enterprise in 2024
 # --------------------------------------------------
 # Description: Makefile for the project
 # Copyright (c) 2024 Maxims Enterprise
 #

# Include the constants from constants.mk
include build/constants.mk

.PHONY: all clean run debug disk

# Define the default target
all: $(FINAL)

# Define the final target
$(FINAL): $(BIN) disk
	# For problems, I have to copy this to my HHD
	cp $(BIN) iso/boot/avery.bin
	mkdir -p ~/.temp
	cp -rf iso ~/.temp/iso
	$(GRUB) $(GRUB_FLAGS)
	cp ~/.temp/avery.iso "$(FULL_PATH)/avery.iso"
	rm -rf ~/.temp

# Define the binary target
$(BIN): $(BOOT_OBJS) $(C_OBJS) $(CXX_OBJS) $(AS_OBJS) 
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

# Creating the FAT16 disk
disk:
	dd if=/dev/zero of=$(DISK) bs=2880 count=10000
	mkfs.fat -F 16 $(DISK)
	mcopy -i $(DISK) test.txt ::

# Define the clean target
clean:
	rm -rf bin $(OBJ) $(FINAL) $(BIN)
	mkdir -p bin $(OBJ)
	rm -rf $(DISK)

# Define the debug target
debug: CFLAGS += -D DEBUG
debug: run

# Define the run target for QEMU testing
run: $(FINAL)
	qemu-system-x86_64 -hda $(DISK) -cdrom $(FINAL) -boot d
