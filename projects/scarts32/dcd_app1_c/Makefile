# ******************************************************** */
#                                                          */
#                                                          */
#   ____-_-_-_-_-_-_-_-________________________            */
#                                                          */
#                                                          */
#   File:          Makefile                                */
#   Description:                                           */
#   Autor:         Johannes Natter                         */
#   Date:          05.10.2012                              */
#                                                          */
#   Notes:                                                 */
#                                                          */
#   Updates:                                               */
#       Date: dd.mm.yy   Autor:                            */
#       Changes:                                           */
#                                                          */
# ******************************************************** */

# The executable file name.
# If not specified, current directory name or `a.out' will be used.
SW_VER		= 12
SW_REV		= 30

PROGRAM		= dcd_app1

# The directories in which source files reside.
# If not specified, only the current directory will be serached.
SRCDIR		= SRC
INCDIR		= $(SRCDIR)/include

OBJDIR_SYS_DE0N = SYS_DE0N/objs
PROG		= $(OBJDIR_SYS_DE0N)/$(PROGRAM)

# Compiler and Linker options
CFLAGS		= -ggdb -Wa,-a,-ad -O0 -I$(SRCDIR) -I$(INCDIR)/scarts -I$(INCDIR)/microchip -Wall -DSW_VERSION=0x$(SW_VER)$(SW_REV)
LDFLAGS		= -Wl,--nmagic,-TSYS_DE0N/$(PROGRAM).ld,-Map=$(PROG).map

# Tools
CC			= scarts_32-none-eabi-gcc
OBJCOPY		= scarts_32-none-eabi-objcopy
OBJDUMP		= scarts_32-none-eabi-objdump
HEX2QUARTUS	= scarts-hex2quartus
SIZE		= scarts_32-none-eabi-size
NM			= scarts_32-none-eabi-nm
HEXCONV		= hex_conv
UN_PACK		= un_pack

# Misc
SRCS_C   = $(foreach d,$(SRCDIR),$(wildcard $(d)/*.c))
SRCS_ASM = $(foreach d,$(SRCDIR),$(wildcard $(d)/*.S))
SRCS_ALL = $(SRCS_ASM) $(SRCS_C)
HEADERS = $(foreach d,$(SRCDIR),$(wildcard $(d)/*.h))
SRCS_SYS_DE0N = $(filter-out %bootloader_main.c, $(SRCS_ALL))
OBJS_SYS_DE0N = $(addsuffix .o, $(basename $(addprefix $(OBJDIR_SYS_DE0N)/, $(notdir $(SRCS_SYS_DE0N)))))
DEPS = $(OBJS_SYS_DE0N:.o=.d)

DEPEND		= $(CC) -MM -MP $(CFLAGS)
DEPEND.d	= $(subst -g ,,$(DEPEND))
COMPILE.c	= $(CC) $(CFLAGS) -c
LINK.c		= $(CC) $(CFLAGS) $(LDFLAGS)

.PHONY: all SYS_DE0N clean show help

# Delete the default suffixes
.SUFFIXES:

all: SYS_DE0N

# Systems
#-------------------------------------
SYS_DE0N: $(OBJS_SYS_DE0N)
	$(LINK.c) $(OBJS_SYS_DE0N) -o $(PROG).elf
	$(OBJCOPY) -O srec $(PROG).elf $(PROG).srec
	$(OBJDUMP) -DSGlrswxz $(PROG).elf > $(PROG).txt

	$(OBJCOPY) --change-section-lma .text=0 --only-section=.text $(PROG).elf $(PROG).tmp
	$(OBJCOPY) -O ihex $(PROG).tmp $(PROG).ihex
	$(HEX2QUARTUS) < $(PROG).ihex > $(PROG).hex
	
	$(OBJCOPY) --remove-section=.text $(PROG).elf $(PROG)_data.tmp
	$(OBJCOPY) -O ihex $(PROG)_data.tmp $(PROG)_data.ihex
	
	$(HEXCONV) $(PROG)_data.ihex //p //h //s //e > $(PROG)_data.hex

	$(HEXCONV) $(PROG).hex //p //od //b0x10000 > $(PROG).dat

	$(HEXCONV) $(PROG).ihex //ob //a0x80000
	$(HEXCONV) $(PROG)_data.ihex //ob //a0x8000
	$(UN_PACK) $(PROG).bin $(PROG)_data.bin ../DCD_APP1_DESIGN/Quartus/TbdDCD_APP1_DESIGN.rbf
	mv packed.bin $(PROG).dca

	# Debug
# $(OBJCOPY) --change-section-lma .text=0 $(PROG).elf $(PROG)_gdb.elf
	# Memory use overview
	$(SIZE) -x $(PROG).elf > $(PROG).size
	# Memory use symbols
	$(NM) --line-numbers --numeric-sort --format=sysv $(PROG).elf > $(PROG).nm
	$(NM) --line-numbers --format=sysv $(PROG).elf >> $(PROG).nm
	@echo
	@echo SYS_DE0N ... OK

# Rules for creating certain files
#------------------------------------------
$(OBJDIR_SYS_DE0N)/%.d: $(SRCDIR)/%.c
	@echo -n $(dir $@) > $@
	@$(DEPEND.d) $< >> $@

$(OBJDIR_SYS_DE0N)/%.o: $(SRCDIR)/%.c
	$(COMPILE.c) $< -o $@ > $(basename $@).lst

$(OBJDIR_SYS_DE0N)/%.o: $(SRCDIR)/%.s
	$(COMPILE.c) $< -o $@ > $(basename $@).lst

# Additional
#------------------------------------------
clean:
	rm -f $(OBJDIR_SYS_DE0N)/*.*

help:
	@echo 'Usage: make [TARGET]'
	@echo 'TARGETS:'
	@echo '  all       (=make) compile and link.'
	@echo '  SYS_DE0N  make target for DE0 Nano Board.'
	@echo '  clean     clean objects and the executable file.'
	@echo '  show      show variables (for debug use only).'
	@echo '  help      print this message.'

show:
	@echo 'PROGRAM         :' $(PROGRAM)
	@echo 'SRCDIR          :' $(SRCDIR)
	@echo 'SRCS_C          :' $(SRCS_C)
	@echo 'SRCS_ASM        :' $(SRCS_ASM)
	@echo 'SRCS_ALL        :' $(SRCS_ALL)
	@echo
	@echo '- SYS_DE0N -'
	@echo 'SRCS_SYS_DE0N   :' $(SRCS_SYS_DE0N)
	@echo 'OBJS_SYS_DE0N   :' $(OBJS_SYS_DE0N)
	@echo 'COMPILE.c       :' $(COMPILE.c)
	@echo 'link.c          :' $(LINK.c)

ifneq ($(DEPS),)
  sinclude $(DEPS)
endif
