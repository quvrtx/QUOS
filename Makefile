# ===== Конфигурация =====
ARCH ?= x86_64  # По умолчанию x86_64, можно переопределить: make ARCH=riscv

# ===== Инструменты =====
# Для x86
ifeq ($(ARCH),x86_32)
    ASM = nasm
    CC = clang
    LD = ld
    ASM_FLAGS = -f elf32
    CC_FLAGS = -ffreestanding -nostdlib -nostartfiles -nodefaultlibs \
               -fno-builtin -fno-stack-protector -m32 -Isrc -nostdinc
    LD_FLAGS = -m elf_i386 -nostdlib -T linker_x86.ld
else ifeq ($(ARCH),x86_64)
    ASM = nasm
    CC = clang
    LD = ld
    ASM_FLAGS = -f elf64
    CC_FLAGS = -ffreestanding -nostdlib -nostartfiles -nodefaultlibs \
               -fno-builtin -fno-stack-protector -m64 -Isrc -nostdinc
    LD_FLAGS = -m elf_x86_64 -nostdlib -T linker_x86.ld
# Для RISC-V
else ifeq ($(ARCH),riscv)
    ASM = riscv64-unknown-elf-as
    CC = riscv64-unknown-elf-gcc
    LD = riscv64-unknown-elf-ld
    ASM_FLAGS = -march=rv64imac -mabi=lp64
    CC_FLAGS = -ffreestanding -nostdlib -nostartfiles -nodefaultlibs \
               -fno-builtin -march=rv64imac -mabi=lp64 -Isrc -nostdinc
    LD_FLAGS = -nostdlib -T linker_riscv.ld
else
    $(error Unknown ARCH: $(ARCH). Supported: x86_32, x86_64, riscv)
endif

# ===== Пути и файлы =====
SRC_DIRS := $(shell find src -type d)
ASM_SOURCES := $(shell find src -name '*.asm')
C_SOURCES := $(shell find src -name '*.c')

OBJ_DIR = obj/$(ARCH)
ASM_OBJECTS := $(ASM_SOURCES:src/%.asm=$(OBJ_DIR)/%.o)
C_OBJECTS := $(C_SOURCES:src/%.c=$(OBJ_DIR)/%.o)
OBJECTS := $(ASM_OBJECTS) $(C_OBJECTS)

KERNEL = kernel-$(ARCH).bin

OBJ_SUBDIRS := $(patsubst src%,$(OBJ_DIR)%,$(SRC_DIRS))

# ===== Цели =====
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
ifeq ($(ARCH),x86_32)
	qemu-system-i386 -kernel $(KERNEL)
else ifeq ($(ARCH),x86_64)
	qemu-system-x86_64 -kernel $(KERNEL)
else ifeq ($(ARCH),riscv)
	qemu-system-riscv64 -machine virt -kernel $(KERNEL) -nographic
else
	$(error Unknown ARCH: $(ARCH))
endif

.PHONY: all clean run