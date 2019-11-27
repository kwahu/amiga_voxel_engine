# 
# Makefile for libikbd.a and associated test program
#

TOOLCHAIN_PREFIX=m68k-atari-mint-
CC=$(TOOLCHAIN_PREFIX)gcc
AR=$(TOOLCHAIN_PREFIX)ar

UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
PREFIX=m68k-atari-mint
HATARI=hatari
else
PREFIX=/opt/cross-mint/m68k-atari-mint
HATARI=/usr/local/bin/hatari
endif

CFLAGS=-mcpu=68000\
	-O3\
	-fomit-frame-pointer \
	-Wall\
	-mshort\
	-nostdlib
	
ASFLAGS=$(CFLAGS)	
	
TST=testikbd
APP=$(TST).tos
LIB=libikbd.a

all: $(LIB) $(APP) ikbd_joy.tos

CSOURCES=keys.c mouse.c
		
OBJECTS=keys.o mouse.o ikbd.o

$(APP): $(OBJECTS) startup.S main.c depend
	$(CC) $(CFLAGS) startup.S main.c $(LIB) -o $(APP)
	m68k-atari-mint-strip $(APP)

ikbd_joy.tos: $(OBJECTS) startup.S main_joy.c depend
	$(CC) $(CFLAGS) startup.S main_joy.c $(LIB) -o ikbd_joy.tos
	m68k-atari-mint-strip ikbd_joy.tos

$(LIB): $(OBJECTS)
	$(AR) rcs $(LIB) $(OBJECTS) 
		
.PHONY clean:
	- rm -rf *.o $(LIB) depend $(APP) ikbd_joy.tos

depend: $(CSOURCES) main.c main_joy.c
		$(CC) $(CFLAGS) $(INCLUDE) -M $(CSOURCES) main.c main_joy.c > depend

	
ifneq (clean,$(MAKECMDGOALS))
-include depend
endif
