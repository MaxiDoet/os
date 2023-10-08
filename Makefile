obj_files := boot/boot.o \
			 kernel/main.o kernel/debug.o \
			 kernel/cpu/gdt.o kernel/cpu/gdt_install.o kernel/cpu/idt.o kernel/cpu/int_handler.o kernel/cpu/irq.o \
			 kernel/mem/heap.o \
			 kernel/io/pci.o kernel/io/pci_device.o \
			 kernel/fs/fs.o kernel/fs/ext2.o \
			 kernel/dev/dev.o kernel/dev/sd.o \
			 kernel/drivers/serial.o kernel/drivers/screen.o kernel/drivers/pic.o kernel/drivers/pit.o kernel/drivers/input/mouse.o kernel/drivers/ata.o kernel/drivers/rtc.o kernel/drivers/sound/ac97.o kernel/drivers/video/vga.o kernel/drivers/video/cirrus.o

CXXFLAGS := -m32 -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -Wno-write-strings -Ofast -Ikernel
LDFLAGS := -melf_i386
ASFLAGS = -f elf32

%.o: %.c
	g++ -c $< -o $@ $(CXXFLAGS)

%.o: %.asm
	nasm $(ASFLAGS) $< -o $@

os.bin: $(obj_files)
	ld $(LDFLAGS) -T linker.ld $^ -o os.bin

os.iso: os.bin
	mkdir -p build/boot/grub
	cp os.bin build/boot/os.bin
	cp boot/grub.cfg build/boot/grub/grub.cfg
	grub-mkrescue -o os.iso build

clean:
	rm -f $(obj_files)
	rm -f *.iso*
	rm -fr build
	rm -f os.bin

run:
	qemu-system-i386 -vga cirrus -boot d -cdrom os.iso -hda hdd.img -device AC97 -d trace:vga_cirrus_bitblt_start
	#../qemu/build/qemu-system-i386 -vga cirrus -boot d -cdrom os.iso -hda hdd.img -device AC97 -d trace:vga_cirrus_bitblt_start
