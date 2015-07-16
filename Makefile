TRG	= server
SRC	= main.c handle.c

CROSS_COMPILE	= mips-openwrt-linux-
LIBS		= -nodefaultlibs -lgcc -lc -luClibc++

include Makefile_generic.defs
