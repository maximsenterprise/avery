# constants.mk
# As part of the Avery project
# Created by Maxims Enterprise in 2024

# Define folder constants
SRC = avery
INCLUDE = include
BIN = ./bin/avery
OBJ = obj
ISO = iso
FINAL = $(BIN)/avery.iso
BOOT = boot

# Define compilers and their respective flags

# The C compiler
CC = x86_64-elf-gcc
CFLAGS = -m32 -Wall -Wextra -Werror -std=c99 -pedantic -g -I $(INCLUDE) -c 

# The C++ compiler
CXX = x86_64-elf-g++
CXXFLAGS = -m32 -Wall -Wextra -Werror -std=c++20 -pedantic -g -I $(INCLUDE) -c

# GNU Linker
LD = x86_64-elf-ld
LDFLAGS = -m elf_i386 -T linker.ld

# The assembler
AS = nasm
ASFLAGS = -f elf32

# Grub for booting
GRUB = grub-mkrescue
GRUB_FLAGS = -o $(FINAL) $(ISO)

# File constants
C_SRCS = $(wildcard $(SRC)/*.c)
CXX_SRCS = $(wildcard $(SRC)/*.cpp)
AS_SRCS = $(wildcard $(SRC)/*.asm)
BOOT_SRCS = $(wildcard $(BOOT)/*.asm)
C_OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(C_SRCS))
CXX_OBJS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(CXX_SRCS))
AS_OBJS = $(patsubst $(SRC)/%.asm, $(OBJ)/%.o, $(AS_SRCS))
BOOT_OBJS = $(patsubst $(BOOT)/%.asm, $(OBJ)/%.o, $(BOOT_SRCS))
