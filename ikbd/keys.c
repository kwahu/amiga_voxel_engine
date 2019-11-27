/*
 *
 *	IKBD 6301 interrupt routine
 *	(c) 2010/11/14 by Simon Sunnyboy / Paradize <marndt@asmsoftware.de>
 *	http://paradize.atari.org/
 *
 *	handling of key presses
 *
 *	derived from a similar routine Copyright (C) 2002	Patrice Mandin
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation; either
 *	version 2.1 of the License, or (at your option) any later version.
 *
 *	This library is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *	Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public
 *	License along with this library; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include <stdint.h>
#include "ikbd.h"
#include <osbind.h>

#define TIMER (*(volatile uint32_t const *)0x000004baUL)

extern volatile uint8_t IKBD_Mousemode;

/* flushes all pending keyboard and joystick events */
void IKBD_Flush()
{
	uint16_t i;
	uint16_t flush_ok;
	uint32_t tick;

	do
	{
		tick = TIMER;
		if(IKBD_Mousemode != 0)
		{
		    IKBD_ReadMouse();
		}
		flush_ok = (  (IKBD_MOUSEK == 0)
		            &&(IKBD_STICK0 == 0)
		            &&(IKBD_STICK1 == 0));

		for(i=0; i<128; i++)
		{
			if(IKBD_Keyboard[i] == IKBD_KEY_PRESSED)
			{
			    (void)Cconws(".\r\n");
				flush_ok = 0;
				break;			/* allow some time to pass */
			}
		}
		/* await Timer C to count */
		while((tick + 10) < TIMER )
		{
			/* wait for time */
		}


	} while(!flush_ok);
	return;
}

/* returns and debounces a key by scancode */
uint8_t IKBD_IsKeyPressed(uint8_t scancode)
{
	uint8_t keystatus;

	if(scancode & 0x80)
	{
		/* upper bit of scancode set -> scancode is invalid */
		return(IKBD_KEY_RELEASED);
	}
	else
	{
		keystatus = IKBD_Keyboard[scancode];

		if((keystatus == IKBD_KEY_PRESSED)||(keystatus == IKBD_KEY_RELEASED))
		{
			/* current key in defined state -> read it and set as read */
			IKBD_Keyboard[scancode] = IKBD_KEY_UNDEFINED;
			return(keystatus);
		}
	}
	/* undefined key state -> means key not pressed */
	return(IKBD_KEY_RELEASED);
}
