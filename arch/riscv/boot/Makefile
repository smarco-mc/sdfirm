#
# arch/arm64/boot/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies.
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 2009 Lv Zheng
#

sdfirm := sdfirm.bin sdfirm.srec sdfirm.ram sdfirm.rom

$(obj)/sdfirm.bin: sdfirm.strip FORCE
	$(call if_changed,objcopy)
	@echo '  Small Device Firmware: $@ is ready'
$(obj)/sdfirm.srec: sdfirm.strip FORCE
	$(call if_changed,objcopy)
	@echo '  Small Device Firmware: $@ is ready'
$(obj)/sdfirm.rom: $(obj)/sdfirm.bin FORCE
ifdef CONFIG_XIP
ifdef CONFIG_LOAD_DATA
	$(call if_changed,bin2vhx)
else
	touch $@
endif
else
	touch $@
endif
	@echo '  Small Device Firmware: $@ is ready'
$(obj)/sdfirm.ram: $(obj)/sdfirm.srec FORCE
	$(call if_changed,srec2vhx)
	@echo '  Small Device Firmware: $@ is ready'
OBJCOPYFLAGS_sdfirm.bin		+=-O binary
OBJCOPYFLAGS_sdfirm.srec	+=-O srec
BIN2VHXFLAGS_sdfirm.rom		+=--width=32
SREC2VHXFLAGS_sdfirm.ram	+=-w 32
