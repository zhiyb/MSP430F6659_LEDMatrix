TRG	= server
SRC	= server.c

CROSS_COMPILE	= mips-openwrt-linux-
LIBS		= -nodefaultlibs -lgcc -lc -luClibc++

include Makefile_generic.defs
