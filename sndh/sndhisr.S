|
|	SNDH player lib for GCC
|	(c) 2010/2014/2019 by Simon Sunnyboy / Paradize <marndt@asmsoftware.de>
|	http://paradize.atari.org/
|
|	lowlevel SNDH player library routines for hooking into Timer C
|	original code by gwEm - adapted to GCC by Simon Sunnyboy
|
|	This library is free software| you can redistribute it and/or
|	modify it under the terms of the GNU Lesser General Public
|	License as published by the Free Software Foundation| either
|	version 2.1 of the License, or (at your option) any later version.
|
|	This library is distributed in the hope that it will be useful,
|	but WITHOUT ANY WARRANTY| without even the implied warranty of
|	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|	Lesser General Public License for more details.
|
|	You should have received a copy of the GNU Lesser General Public
|	License along with this library| if not, write to the Free Software
|	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
|
|
|   ATTENTION: These functions use the classic stack conventions. The file
|              must be assembled and linked with -mshort compiler options.

	.global _SNDH_PlayTuneISR
	.global _SNDH_StopTuneISR

	.text

| *** initialize Timer C to replay the given SNDH ***
| parameters:  a0  ptr to SNDH  d0.w replay frequency      d1.w subtune to play
_SNDH_PlayTuneISR:
    link a6,#0
	lea	sndhpoi(pc),a0
	move.l	8(a6),(a0)+	  	    | start of sndh
	move.w	12(a6),(a0)+ 		| replay freq
	move.w	14(a6),d0			| subtune
	move.b	0x484.w,(a0)+
	clr.b	0x484.w			    |keyclick off
	move.l	#0x08000000,0xffff8800.w	|die YM2149
	move.l	#0x09000000,0xffff8800.w	|die
	move.l	#0x0A000000,0xffff8800.w	|die ;)
	move.l	sndhpoi(pc),a0
	jsr	(a0)			        |sndh init (with subtune in d0)
	lea	oldtc(pc),a0
	move.l	0x114.w,(a0)		|store old timer c
	lea	newtc(pc),a0
	move.l	a0,0x114.w 		    |new timer c vector
	unlk a6
    rts

| *** stop current active SNDH replay ISR ***
_SNDH_StopTuneISR:
	move.l	oldtc(pc),0x114.w	|restore timer c
	move.l	sndhpoi(pc),a0
	jsr	4(a0) 			        |sndh deinit
	move.b	oldcon(pc),0x484.w	|restore key click
	rts

| *** new Timer C ISR for handling replay ***
newtc:
	movem.l	a0-a6/d0-d7,-(sp)
	lea	tccount(pc),a0		|synthesize any replay rate from timer c
	move.w	sndhfrq(pc),d0	|
	sub.w	d0,(a0) 		|
	bgt.s	nosndh			|
	add.w	#200,(a0)		|

dosndh:
	move.w	sr,-(sp)		|store status reg
	move.w	#0x2300,sr		|enable interrupts
	move.l	sndhpoi(pc),a0
	jsr	8(a0) 			    |call sndh interrupt routine
	move.w	(sp)+,sr		|restore status register


nosndh:
	movem.l	(sp)+,a0-a6/d0-d7
	move.l	oldtc(pc),-(sp) 	|system friendly  jump to old Timer C
	rts
|............................................................................

tccount:	dc.w	200
oldtc:		ds.l	1
sndhpoi:	ds.l	1
sndhfrq:	ds.w	1
oldcon:		ds.b	1
	.end

