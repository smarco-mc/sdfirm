# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies. Remember to do have actions
# for "archclean" and "archdep" for cleaning up and making dependencies for
# this architecture
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#

OBJCOPYFLAGS    := -O binary
LDFLAGS_sdfirm	:=

export BITS
ifeq ($(CONFIG_RISCV_64I),y)
	BITS := 64
	UTS_MACHINE := riscv64

	KBUILD_CFLAGS += -mabi=lp64
	KBUILD_AFLAGS += -mabi=lp64

	KBUILD_LDFLAGS += -melf64lriscv
else
	BITS := 32
	UTS_MACHINE := riscv32

	CFLAGS += -mabi=ilp32
	AFLAGS += -mabi=ilp32
	LDFLAGS += -melf32lriscv
endif

# ISA string setting
riscv-march-$(CONFIG_RISCV_32I)	:= rv32ima
riscv-march-$(CONFIG_RISCV_64I)	:= rv64ima
riscv-march-$(CONFIG_RISCV_F)	:= $(riscv-march-y)f
riscv-march-$(CONFIG_RISCV_D)	:= $(riscv-march-y)d
riscv-march-$(CONFIG_RISCV_C)	:= $(riscv-march-y)c
CFLAGS += -march=$(subst fd,,$(riscv-march-y))
AFLAGS += -march=$(riscv-march-y)

CFLAGS += -mno-save-restore
CFLAGS += -DCONFIG_PAGE_OFFSET=$(CONFIG_PAGE_OFFSET)

ifeq ($(CONFIG_CMODEL_MEDLOW),y)
	CFLAGS += -mcmodel=medlow
endif
ifeq ($(CONFIG_CMODEL_MEDANY),y)
	CFLAGS += -mcmodel=medany
endif

# GCC versions that support the "-mstrict-align" option default to allowing
# unaligned accesses.  While unaligned accesses are explicitly allowed in the
# RISC-V ISA, they're emulated by machine mode traps on all extant
# architectures.  It's faster to have GCC emit only aligned accesses.
CFLAGS += $(call cc-option,-mstrict-align)

# Default target when executing plain make
boot		:= arch/riscv/boot
KBUILD_IMAGE	:= sdfirm.bin sdfirm.srec sdfirm.vhx

# arch specific predefines for sparse
CHECKFLAGS += -D__riscv -D__riscv_xlen=$(BITS)

head-y := arch/riscv/common/head.rel

ifeq ($(CONFIG_ARCH_VEGA),y)
 machine-$(CONFIG_ARCH_VEGA)       := vega
 incdir-$(CONFIG_ARCH_VEGA)	   := vega
endif

ifeq ($(incdir-y),)
incdir-y := $(machine-y)
endif
INCDIR   := mach-$(incdir-y)

ifneq ($(machine-y),)
MACHINE  := arch/riscv/mach-$(machine-y)/
else
MACHINE  :=
endif

machdirs := $(patsubst %,arch/riscv/mach-%/,$(machine-y))
platdirs := $(patsubst %,arch/riscv/plat-%/,$(plat-y))

core-y				+= arch/riscv/common/
drivers-y			+= $(MACHINE)

all: $(KBUILD_IMAGE)

#	Update machine arch and proc symlinks if something which affects
#	them changed.  We use .arch to indicate when they were updated
#	last, otherwise make uses the target directory mtime.

include/asm/mach: $(wildcard include/config/arch/*.h) \
		include/config/auto.conf include/asm
	@echo '  SYMLINK include/asm/mach -> include/asm/$(INCDIR)'
	$(Q)$(SYMLINK) $(TOPDIR)/include/asm/$(INCDIR) $@

archprepare: maketools

sdfirm.bin: sdfirm.strip
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@
sdfirm.srec: sdfirm.strip
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@
sdfirm.vhx: sdfirm.bin
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

PHONY += maketools FORCE
maketools: include/asm/mach FORCE

CLEAN_FILES += arch/$(ARCH)/include/asm/mach-types.h

# We use MRPROPER_FILES and CLEAN_FILES now
archclean:
	$(Q)$(MAKE) $(clean)=$(boot)