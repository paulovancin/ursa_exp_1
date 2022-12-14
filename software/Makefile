#platform-specific configuration
#include ../Configuration.mk

#arch confs.
ARCH = riscv/hf-riscv
IMAGE_NAME = image

#dir config.
HFOS_DIR = hellfireos

CPU_ARCH = \"$(ARCH)\"
MAX_TASKS = 30
MUTEX_TYPE = 0
MEM_ALLOC = 3
HEAP_SIZE = 500000
FLOATING_POINT = 0
#KERNEL_LOG = 2
KERNEL_LOG = $(KERNEL_LOG_LEVEL)

SRC_DIR = $(HFOS_DIR)

#includes for kernel parts
include $(HFOS_DIR)/arch/$(ARCH)/arch.mak
include $(HFOS_DIR)/lib/lib.mak
include $(HFOS_DIR)/drivers/noc.mak
include $(HFOS_DIR)/sys/kernel.mak

#include tasks
include $(HFOS_DIR)/../applications/*/app.mak
include $(HFOS_DIR)/../extensions/*/ext.mak

#phonies
.PHONY: clean

INC_DIRS += -I $(HFOS_DIR)/lib/include \
			-I $(HFOS_DIR)/sys/include \
			-I $(HFOS_DIR)/drivers/noc/include \
			-I $(HFOS_DIR)/../extensions/orca-core/include \
			-I $(HFOS_DIR)/../extensions/orca-monitoring/include \
			-I $(HFOS_DIR)/../extensions/orca-pubsub/include

NOC_FLAGS = -DNOC_INTERCONNECT -DNOC_PACKET_SIZE=64 -DNOC_PACKET_SLOTS=64 \
	    -DNOC_WIDTH=$(ORCA_NOC_WIDTH) -DNOC_HEIGHT=$(ORCA_NOC_HEIGHT)

CFLAGS += -DCPU_ARCH=$(CPU_ARCH) \
	-DMAX_TASKS=$(MAX_TASKS) -DMEM_ALLOC=$(MEM_ALLOC) \
	-DHEAP_SIZE=$(HEAP_SIZE) -DMUTEX_TYPE=$(MUTEX_TYPE) \
	-DFLOATING_POINT=$(FLOATING_POINT) \
	-DKERNEL_LOG=$(KERNEL_LOG) \
	$(COMPLINE) \
	$(NOC_FLAGS)

$(IMAGE_NAME).bin:
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Making Kernel ...               \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	make hal
	make libc
	make noc
	make kernel
	ar rcs hellfire-os.a *.o
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Making Extensions ...           \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	@for ext in $(ORCA_EXTENSIONS) ; do \
		make ext-$$ext.a ; \
	done
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Making Apps ...                 \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	for app in $(ORCA_APPLICATIONS) ; do \
		make app-$$app.a ; \
	done
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Linking Software ...            \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	$(LD) $(LDFLAGS) -T$(LINKER_SCRIPT) -o $(IMAGE_NAME).elf *.o
	$(DUMP) --disassemble --reloc $(IMAGE_NAME).elf > $(IMAGE_NAME).lst
	$(DUMP) -h $(IMAGE_NAME).elf > $(IMAGE_NAME).sec
	$(DUMP) -s $(IMAGE_NAME).elf > $(IMAGE_NAME).cnt
	$(OBJ) -O binary $(IMAGE_NAME).elf $(IMAGE_NAME).bin
	$(SIZE) $(IMAGE_NAME).elf
	hexdump -v -e '4/1 "%02x" "\n"' $(IMAGE_NAME).bin > $(IMAGE_NAME).txt
	mv *.o *.elf *.bin *.cnt *.lst *.sec *.txt *.a ../bin

clean:
	rm -rf *.o *~ *.elf *.bin *.cnt *.lst *.sec *.txt *.a
