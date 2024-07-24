
#	constants.mk
#	As part of the Avery project
#	Created by Maxims Enterprise in 2024
#	--------------------------------------------------
#	Description: Constants for the Makefile
#	Copyright (c) 2024 Maxims Enterprise


# Define folder constants

FULL_PATH = $(shell pwd)

SRC = avery
INCLUDE = include
BIN = ./bin/avery
OBJ = obj
ISO = iso
FINAL = avery.iso
BOOT = boot

# Define compilers and their respective flags

# The C compiler
CC = x86_64-elf-gcc
CFLAGS = -m32 -I $(INCLUDE) -c 

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
GRUB_FLAGS = -o ~/.temp/avery.iso iso

# File constants
C_SRCS = $(shell find $(SRC) -name "*.c")
CXX_SRCS = $(shell find $(SRC) -name "*.cpp")
AS_SRCS = $(shell find $(SRC) -name "*.asm")
BOOT_SRCS = $(wildcard $(BOOT)/*.asm)
C_OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(C_SRCS))
CXX_OBJS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(CXX_SRCS))
AS_OBJS = $(patsubst $(SRC)/%.asm, $(OBJ)/%.o, $(AS_SRCS))
BOOT_OBJS = $(patsubst $(BOOT)/%.asm, $(OBJ)/%.o, $(BOOT_SRCS))

# Disk constants
DISK = disk.img

