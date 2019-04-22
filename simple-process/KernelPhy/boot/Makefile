ASM = nasm
ASMFLAGS = -I boot/include/
TARGETS = boot/boot.bin boot/loader.bin

.PHONY: everything clean all

everything: $(TARGETS)

clean: 
	rm -f $(TARGETS)

all: clean everything

boot/boot.bin: boot/boot.asm boot/include/fat12hdr.inc boot/include/load.inc
	$(ASM) $(ASMFLAGS) -o $@ $<

boot/loader.bin: boot/loader.asm boot/include/fat12hdr.inc \
	boot/include/load.inc boot/include/pm.inc boot/include/lib.inc
	$(ASM) $(ASMFLAGS) -o $@ $<


