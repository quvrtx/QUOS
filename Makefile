ARCH = x86_64

ASM = nasm
CC = gcc
LD = ld

ASM_FLAGS = -f elf32
CC_FLAGS = -ffreestanding -nostdlib -nostartfiles -nodefaultlibs \
           -fno-builtin -fno-stack-protector -m32 -Isrc
LD_FLAGS = -m elf_i386 -nostdlib -T linker.ld

SRC_DIRS := $(shell find src -type d)
ASM_SOURCES := $(shell find src -name '*.asm')
C_SOURCES := $(shell find src -name '*.c')

OBJ_DIR = obj
ASM_OBJECTS := $(ASM_SOURCES:src/%.asm=$(OBJ_DIR)/%.o)
C_OBJECTS := $(C_SOURCES:src/%.c=$(OBJ_DIR)/%.o)
OBJECTS := $(ASM_OBJECTS) $(C_OBJECTS)

KERNEL = kernel.bin

OBJ_SUBDIRS := $(patsubst src%,$(OBJ_DIR)%,$(SRC_DIRS))

all: $(KERNEL)

$(KERNEL): $(OBJECTS) | $(OBJ_SUBDIRS)
	$(LD) $(LD_FLAGS) -o $@ $(OBJECTS)

$(OBJ_DIR)/%.o: src/%.asm | $(OBJ_SUBDIRS)
	$(ASM) $(ASM_FLAGS) $< -o $@

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_SUBDIRS)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(OBJ_SUBDIRS):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(KERNEL)

run:
	qemu-system-$(ARCH) -kernel $(KERNEL)

.PHONY: all clean run