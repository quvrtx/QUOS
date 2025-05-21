ARCH ?= x86_32

CC = clang
AS = nasm
LD = ld
OBJCOPY = objcopy
MKDIR = mkdir -p
RM = rm -f

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

ASFLAGS = -f elf32
CC_FLAGS = -target i386-pc-none-elf -m32 -ffreestanding -nostdlib \
           -fno-builtin -fno-stack-protector -fno-pic \
           -I$(INCLUDE_DIR)/kernel/arch/$(ARCH) -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/kernel \
           -nostdinc -DARCH_$(ARCH)
LD_FLAGS = -m elf_i386 -nostdlib -T linker.ld --oformat=elf32-i386

ASM_SOURCES := $(shell find $(SRC_DIR) -name '*.asm')
C_SOURCES := $(shell find $(SRC_DIR) -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$(ASM_SOURCES)) \
           $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
DIRS := $(sort $(dir $(OBJECTS)))

all: $(BIN_DIR)/kernel-$(ARCH).bin

$(BIN_DIR)/kernel-$(ARCH).bin: $(BIN_DIR)/kernel-$(ARCH).elf
	$(OBJCOPY) -O binary $< $@

$(BIN_DIR)/kernel-$(ARCH).elf: $(OBJECTS) | $(BIN_DIR)
	$(LD) $(LD_FLAGS) -o $@ $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm | $(DIRS)
	@$(MKDIR) $(@D)
	$(AS) $(ASFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(DIRS)
	@$(MKDIR) $(@D)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(DIRS) $(BIN_DIR):
	@$(MKDIR) $@

# ===== Очистка =====
clean:
	@$(RM) -r $(OBJ_DIR) $(BIN_DIR)

run: $(BIN_DIR)/kernel-$(ARCH).elf
	qemu-system-i386 -kernel $<

.PHONY: all clean run