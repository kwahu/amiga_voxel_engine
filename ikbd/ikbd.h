/*
 *
 *	IKBD 6301 interrupt routine
 *	(c) 2010/11/14 by Simon Sunnyboy / Paradize <marndt@asmsoftware.de>
 *	http://paradize.atari.org/
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

#ifndef _IKBD_H
#define _IKBD_H

#include <stdint.h>

/* Constants for return values  */

#define IKBD_MOUSE_BUTTON_LEFT (uint8_t)(0x02)
#define IKBD_MOUSE_BUTTON_RIGHT (uint8_t)(0x01)
#define IKBD_MOUSE_BOTH_BUTTONS (uint8_t)(0x03)

#define IKBD_JOY_UP          (uint8_t)(1<<0)
#define IKBD_JOY_DOWN        (uint8_t)(1<<1)
#define IKBD_JOY_LEFT        (uint8_t)(1<<2)
#define IKBD_JOY_RIGHT       (uint8_t)(1<<3)
#define IKBD_JOY_FIRE        (uint8_t)(1<<7)

#define IKBD_KEY_PRESSED     (uint8_t)(0xff)
#define IKBD_KEY_UNDEFINED   (uint8_t)(0x80)
#define IKBD_KEY_RELEASED    (uint8_t)(0x00)

/* Constants for scancodes of important keys */
#define IKBD_KEY_F1          59
#define IKBD_KEY_F2          60
#define IKBD_KEY_F3          61
#define IKBD_KEY_F4          62
#define IKBD_KEY_F5          63
#define IKBD_KEY_F6          64
#define IKBD_KEY_F7          65
#define IKBD_KEY_F8          66
#define IKBD_KEY_F9          67
#define IKBD_KEY_F10         68

#define IKBD_KEY_SPACE       0x39
#define IKBD_KEY_RETURN      0x1c
#define IKBD_KEY_UP          0x48
#define IKBD_KEY_DOWN        0x50
#define IKBD_KEY_LEFT        0x4b
#define IKBD_KEY_RIGHT       0x4d
#define IKBD_KEY_BACKSPACE   0x0e
#define IKBD_KEY_DELETE      0x53
#define IKBD_KEY_ESC         0x01
#define IKBD_KEY_TAB         0x0F
#define IKBD_KEY_CONTROL     0x1d
#define IKBD_KEY_ALT         0x38
#define IKBD_KEY_LSHIFT      0x2a
#define IKBD_KEY_RSHIFT      0x36
#define IKBD_KEY_INSERT      0x52
#define IKBD_KEY_HELP        0x62
#define IKBD_KEY_UNDO        0x61
#define IKBD_KEY_CLRHOME     0x47
#define IKBD_KEY_CAPSLOCK    0x3a

/* data types */
typedef struct
{
	int16_t	x;
	int16_t	y;
	uint16_t  w;
	uint16_t  h;
	uint8_t   limit;
	uint8_t   b;
} IKBD_MouseData;

/* Variables */

extern volatile IKBD_MouseData      IKBD_Mouse;          /* struct for mouse data */

extern volatile uint8_t       IKBD_Keyboard[128];  /* Keyboard table */
extern volatile uint8_t       IKBD_Joystick0;      /* Joystick on port 0 */
extern volatile uint8_t       IKBD_Joystick1;      /* Joystick on port 1 */

/* function like access macros */
#define IKBD_MOUSEX (IKBD_Mouse.x)
#define IKBD_MOUSEY (IKBD_Mouse.y)
#define IKBD_MOUSEK (IKBD_Mouse.b)
#define IKBD_STICK0 (IKBD_Joystick0)
#define IKBD_STICK1 (IKBD_Joystick1)

/* Functions */
void IKBD_Install(void);
void IKBD_Uninstall(void);
void IKBD_MouseOn(void);
void IKBD_MouseOff(void);
void IKBD_ReadMouse(void);
void IKBD_SetMouseThreshold(uint8_t);
void IKBD_SetMouseOrigin(int16_t, int16_t, uint16_t, uint16_t);
void IKBD_Flush(void);
uint8_t IKBD_IsKeyPressed(uint8_t);
#endif /* _IKBD_H */
