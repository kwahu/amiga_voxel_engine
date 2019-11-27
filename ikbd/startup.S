			.equ 	BASEPAGE_SIZE,0x100
			.equ	STACK_SIZE,0x1000

			.text
			.globl ___main

start:
			move.l	4(sp),a5				| address to basepage
			move.l	0x0c(a5),d0				| length of text segment
			add.l	0x14(a5),d0				| length of data segment
			add.l	0x1c(a5),d0				| length of bss segment
			add.l	#STACK_SIZE+BASEPAGE_SIZE,d0		| length of stackpointer+basepage
			move.l	a5,d1					| address to basepage
			add.l	d0,d1					| end of program
			and.l	#0xfffffff0,d1			| align stack
			move.l	d1,sp					| new stackspace

			move.l	d0,-(sp)				| mshrink()
			move.l	a5,-(sp)
			clr.w	-(sp)
			move.w	#0x4a,-(sp)
			trap	#1
			lea.l	12(sp),sp


			pea       _main | Offset 2
			move.w    #38,-(sp)    | Offset 0
			trap      #14          | Call XBIOS Supexec()
			addq.l    #6,sp        | Correct stack

exit:
			clr.w -(sp)
			trap #1

___main:
            rts

			.bss

_basepage:	ds.l	1
_program_length:
			ds.l	1

			.end
