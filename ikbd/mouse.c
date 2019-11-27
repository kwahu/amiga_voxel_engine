/*
 *
 *	IKBD 6301 interrupt routine
 *	(c) 2010/11/14 by Simon Sunnyboy / Paradize <marndt@asmsoftware.de>
 *	http://paradize.atari.org/
 *
 *	mouse data collection
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

#include "ikbd.h"

/* driver internal variables*/
extern volatile uint8_t        IKBD_MouseB;         /* Mouse on port 0, buttons (driver internal) */
extern volatile int16_t        IKBD_MouseX;         /* Mouse X position (driver internal) */
extern volatile int16_t        IKBD_MouseY;         /* Mouse Y position (driver internal) */

/* variables */
volatile IKBD_MouseData	IKBD_Mouse;          /* mouse data */

uint8_t MouseThreshold = 128;  /* mouse threshold (255 = 200%) */

/* reads the accumulated mouse packets and updates the mouse position */
void IKBD_ReadMouse()
{
	int16_t xoffset, yoffset;

	if(MouseThreshold > 0)
	{
		/* scale the mouse movement by the configurable threshold */
		xoffset = (int16_t)((int32_t)(IKBD_MouseX * MouseThreshold) / 128);
		yoffset = (int16_t)((int32_t)(IKBD_MouseY * MouseThreshold) / 128);
	}
	else
	{
		/* 0% scaling is not allowed -> scale as 100% */
		xoffset = IKBD_MouseX;
		yoffset = IKBD_MouseY;
	}

	IKBD_Mouse.b = IKBD_MouseB;
	IKBD_Mouse.x = IKBD_Mouse.x + xoffset;
	IKBD_Mouse.y = IKBD_Mouse.y + yoffset;

	if(IKBD_Mouse.limit != 0)
	{
		/* mouse coordinates limitation is active? */
		if(IKBD_Mouse.x < 0)
		{
			IKBD_Mouse.x = 0;
		}
		else if(IKBD_Mouse.x > IKBD_Mouse.w)
		{
			IKBD_Mouse.x = IKBD_Mouse.w;
		}
		if(IKBD_Mouse.y < 0)
		{
			IKBD_Mouse.y = 0;
		}
		else if(IKBD_Mouse.y > IKBD_Mouse.h)
		{
			IKBD_Mouse.y = IKBD_Mouse.h;
		}
	}

	/* acknowledge accumulated mouse vector (driver internals) */
	IKBD_MouseX = 0;
	IKBD_MouseY = 0;
	return;
}

/* allows to configure teh scaling of relative mouse movement */
void IKBD_SetMouseThreshold(uint8_t new_threshold)
{
	MouseThreshold = new_threshold;
	return;
}

/* allows to set current mouse position and define the screen size */
void IKBD_SetMouseOrigin(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	IKBD_Mouse.x = x;
	IKBD_Mouse.y = y;
	IKBD_Mouse.w = w;
	IKBD_Mouse.h = h;
	IKBD_Mouse.b = 0;
	IKBD_Mouse.limit = 1;
	IKBD_MouseX = 0;
	IKBD_MouseY = 0;
	return;
}
