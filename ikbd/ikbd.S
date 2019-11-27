|
|	IKBD 6301 interrupt routine
|	(c) 2010/11/14 by Simon Sunnyboy / Paradize <marndt@asmsoftware.de>
|	http://paradize.atari.org/
|
|	derived from a similar routine Copyright (C) 2002	Patrice Mandin
|
|
|	This library is free software| you can redistribute it and/or
|	modify it under the terms of the GNU Lesser General Public
|	License as published by the Free Software Foundation| either
|	version 2.1 of the License, or (at your option) any later version.
|
|	This library is distributed in the hope that it will be useful,
|	but WITHOUT ANY WARRANTY| without .even the implied warranty of
|	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|	Lesser General Public License for more details.
|
|	You should have received a copy of the GNU Lesser General Public
|	License along with this library| if not, write to the Free Software
|	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
|

.globl	_IKBD_Install
.globl	_IKBD_Uninstall
.globl	_IKBD_Keyboard
.globl	_IKBD_MouseX
.globl	_IKBD_MouseY
.globl	_IKBD_MouseB
.globl	_IKBD_Joystick0
.globl	_IKBD_Joystick1
.globl	_IKBD_MouseOff
.globl	_IKBD_MouseOn
.globl  _IKBD_Mousemode

	.text
| ** installs the IKBD handler
_IKBD_Install:
	movem.l	d0-d1/a0-a1,-(sp)

	| Disable all interrupts

	move.w	#0x2700,sr

	| Save MFP registers used for ACIA interrupt

	lea	0xfffffa00.w,a0
	btst	#6,0x9(a0)
	sne		ikbd_ierb
	btst	#6,0x15(a0)
	sne		ikbd_imrb

	| Set our MFP routine

	move.l	0x118.w,old_ikbd
	move.l	#ikbd,0x118.w
	bset	#6,0xfffffa09.w	| IERB
	bset	#6,0xfffffa15.w	| IMRB

	| Set relative mouse motion mode
	| needed because running a .tos or .ttp program
	| disable the mouse (entering VT52 textmode)

	bsr _IKBD_MouseOn

	| Re-enable interrupts

	move.w	#0x2300,sr

	movem.l	(sp)+,d0-d1/a0-a1
	rts

| ** reinstalls the old IKBD vector
_IKBD_Uninstall:
	move.l	a0,-(sp)

	| Disable interrupts

	move.w	#0x2700,sr

	| Restore previous MFP registers

	lea	0xfffffa00.w,a0

	bclr	#6,0x9(a0)
	tst.b	ikbd_ierb
	beq.s	ikbd_restoreierb
	bset	#6,0x9(a0)
ikbd_restoreierb:

	bclr	#6,0x15(a0)
	tst.b	ikbd_imrb
	beq.s	ikbd_restoreimrb
	bset	#6,0x15(a0)
ikbd_restoreimrb:

	move.l	old_ikbd,0x118.w

	| Clear keyboard buffer

	lea	0xfffffc00.w,a0
ikbd_clearbuffer:
	btst	#0,(a0)
	beq.s	ikbd_buffercleared
	tst.b	2(a0)
	bra.s	ikbd_clearbuffer
ikbd_buffercleared:

	| Re-enable interrupts

	move.w	#0x2300,sr

	move.l	(sp)+,a0
	rts
| *** mouse off routine -> set joystick reporting ***
_IKBD_MouseOff:
	move.b #0x12,0xfffffc02.w
wait_sticks:
	btst	#1,0xfffffc00.w
	beq.s wait_sticks
	move.b #0x14,0xfffffc02.w
	| clear mouse buttons
	clr.b _IKBD_MouseB
	clr.b _IKBD_Mousemode
	rts
| *** mouse on routine -> disable joystick and report mouse again ***
_IKBD_MouseOn:
	move.b #0x08,0xfffffc02.w
	| disable old joystick packets
	clr.b _IKBD_Joystick0
	clr.b _IKBD_Joystick1
	move.b #0xff,_IKBD_Mousemode
	rts
| custom IKBD vector
	.even
	.ascii	"XBRA"
	.ascii	"IKBD"
old_ikbd:	.skip 4

ikbd:
	| test if byte coming from IKBD or MIDI

	btst	#0,0xfffffc00.w
	beq.s	ikbd_endit

	movem.l	d0-d1/a0,-(sp)

	move.b	0xfffffc02.w,d0

	| Joystick packet ?

	cmp.b	#0xff,d0
	beq.s	ikbd_yes_joystick1

	cmp.b	#0xfe,d0
	beq.s	ikbd_yes_joystick0

	| Mouse packet ?

	cmp.b	#0xf8,d0
	bmi.s	ikbd_no_mouse
	cmp.b	#0xfc,d0
	bpl.s	ikbd_no_mouse

ikbd_yes_mouse:
	and.b	#3,d0
	move.b	d0,_IKBD_MouseB

	move.l	#ikbd_mousex,0x118.w    | queue different irq vector for mouse packet
	bra.s	ikbd_endit_stack

ikbd_yes_joystick0:
	move.l	#ikbd_poll_joystick0,0x118.w  | queue joystick irq for next packet
	bra.s	ikbd_endit_stack

ikbd_yes_joystick1:
	move.l	#ikbd_poll_joystick1,0x118.w  | queue joystick irq for next packet
	bra.s	ikbd_endit_stack

	| Keyboard press/release

ikbd_no_mouse:
	move.b	d0,d1
	lea	_IKBD_Keyboard,a0
	and.l	#0x7f,d1			| mask scancode
	tas	d0
	spl	0(a0,d1.w)		| store -1 in scancode table

ikbd_endit_stack:
	movem.l	(sp)+,d0-d1/a0
ikbd_endit:
	bclr	#6,0xfffffa11.w		| acknowledge IKBD interrupt
	rte				| ... and terminate the ISR

ikbd_mousex:
	| mouse packet?

	btst	#0,0xfffffc00.w
	beq.s	ikbd_endit

	move.w	d0,-(sp)

	move.b	0xfffffc02.w,d0
	ext.w	d0
	add.w	d0,_IKBD_MouseX

	move.w	(sp)+,d0

	move.l	#ikbd_mousey,0x118.w
	bra.s	ikbd_endit

ikbd_mousey:
	| mouse packet?

	btst	#0,0xfffffc00.w
	beq.s	ikbd_endit

	move.w	d0,-(sp)

	move.b	0xfffffc02.w,d0
	ext.w	d0
	add.w	d0,_IKBD_MouseY

	move.w	(sp)+,d0

	move.l	#ikbd,0x118.w
	bra.s	ikbd_endit

ikbd_poll_joystick1:
	| test if byte coming from joystick

	btst	#0,0xfffffc00.w
	beq.s	ikbd_endit

	move.b	0xfffffc02.w,_IKBD_Joystick1

	move.l	#ikbd,0x118.w
	bra.s	ikbd_endit

ikbd_poll_joystick0:
	| test if byte coming from joystick 0 (mouse port)

	btst	#0,0xfffffc00.w
	beq.s	ikbd_endit

	move.b	0xfffffc02.w,_IKBD_Joystick0

	move.l	#ikbd,0x118.w
	bra.s	ikbd_endit

	.bss
	.even

_IKBD_Keyboard:
    .skip 128
	.even
_IKBD_MouseX:	.skip 2
_IKBD_MouseY:	.skip 2
	.even
_IKBD_MouseB:	.skip 1
_IKBD_Mousemode: .skip 1
_IKBD_Joystick0:	.skip 1
_IKBD_Joystick1: .skip 1
	.even
ikbd_ierb:	.skip 1
ikbd_imrb:	.skip 1
	.end
