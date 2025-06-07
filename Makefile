ARCH ?= x86

CC = clang
ASM = nasm
RUST = rustc
LD = ld
OBJCOPY = objcopy
MKDIR = mkdir -p
RM = rm -f

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

ASMFLAGS = -f elf32
CCFLAGS = -target i386-pc-none-elf -m32 -ffreestanding -nostdlib \
           -fno-builtin -fno-stack-protector -fno-pic \
           -I$(INCLUDE_DIR)/kernel/arch/$(ARCH) -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/kernel \
           -nostdinc -DARCH_$(ARCH)
RUSTFLAGS = --emit=obj --target=i686-unknown-none
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

# ASM compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm | $(DIRS)
	@$(MKDIR) $(@D)
	$(ASM) $(ASMFLAGS) $< -o $@

# C compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(DIRS)
	@$(MKDIR) $(@D)
	$(CC) $(CCFLAGS) -c $< -o $@

#RUST compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.rs | $(DIRS)
	@$(MKDIR) $(@D)
	$(RUST) $(RUSTFLAGS) -C opt-level=2 -C debuginfo=2 $< -o $@

$(DIRS) $(BIN_DIR):
	@$(MKDIR) $@

clean:
	@$(RM) -r $(OBJ_DIR) $(BIN_DIR)

run: $(BIN_DIR)/kernel-$(ARCH).elf
	qemu-system-i386 -kernel $<

.PHONY: all clean run