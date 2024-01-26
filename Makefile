K=kernel

OBJS = 						\
	$K/entry.o				\
	$K/sbi.o				\
	$K/printf.o				\
	$K/main.o

# 设置交叉编译工具链
TOOLPREFIX := riscv64-unknown-elf-
CC = $(TOOLPREFIX)gcc
AS = $(TOOLPREFIX)gas
LD = $(TOOLPREFIX)ld
OBJCOPY = $(TOOLPREFIX)objcopy
OBJDUMP = $(TOOLPREFIX)objdump

# QEMU 虚拟机
QEMU = qemu-system-riscv64

# gcc 编译选项
CFLAGS = -Wall -Werror -O -fno-omit-frame-pointer -ggdb
CFLAGS += -MD
CFLAGS += -mcmodel=medany
CFLAGS += -ffreestanding -fno-common -nostdlib -mno-relax
CFLAGS += -I.
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)

# ld 链接选项
LDFLAGS = -z max-page-size=4096

# QEMU 启动选项
QEMUOPTS = -machine virt -bios default -kernel Image -nographic

all: Image

Image: Kernel
	$(OBJCOPY) $K/Kernel -O binary Image

Kernel: $(OBJS)
	$(LD) $(LDFLAGS) -T $K/kernel.ld -o $K/Kernel $(OBJS)

# compile all .c and .S file to .o file
$K/%.o: $K/%.c $K/%.S
	$(CC) $(CFLAGS) -c $< -o $@

$U/%.o: $U/%.c $U/%.S
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f */*.d */*.o $K/Kernel Image Image.asm
	
asm: Kernel
	$(OBJDUMP) -S $K/Kernel > Image.asm

qemu: Image
	$(QEMU) $(QEMUOPTS)

gdbserver: Image
	$(QEMU) $(QEMUOPTS) -s -S

gdbclient:
	gdb-multiarch -ex 'file $K/Kernel' -ex 'set arch riscv:rv64' -ex 'target remote localhost:1234'