/*
 * test program 1 for IKBD routines
 * (c) 2010/14 by Simon Sunnyboy / Paradize
 *
 * check certain key events
 */

#include <stdint.h>
#include "ikbd.h"
#include <osbind.h>

#define COLOR0 (*(uint16_t*)0xFFFF8240UL)

#define PRINT(x) (void)Cconws(x)

int main(void)
{
	uint8_t exitflag = 0;

	IKBD_Install();
	IKBD_MouseOn();
	PRINT("IKBD routines installed!\r\n");

	while(exitflag == 0)
	{
		Vsync();
		IKBD_ReadMouse();
		if(IKBD_IsKeyPressed(IKBD_KEY_ESC))
		{
		    PRINT("ESC\r\n");
			exitflag = 1;
		}
		if(IKBD_IsKeyPressed(IKBD_KEY_SPACE) == IKBD_KEY_PRESSED)
		{
			COLOR0 = 0x0FF0;
		}
		else if(IKBD_IsKeyPressed(IKBD_KEY_TAB) == IKBD_KEY_PRESSED)
        {
            PRINT("Joystick\r\n");
        }
		else if(IKBD_MOUSEK == IKBD_MOUSE_BUTTON_LEFT)
		{
            COLOR0 = 0x000F;
		}
		else if(IKBD_MOUSEK == IKBD_MOUSE_BUTTON_RIGHT)
		{
			exitflag = 3;
			COLOR0 = 0x00F0;
		}
		else
		{
			COLOR0 = 0x0FFF;
		}
	}

    PRINT("...exiting\r\n");

	IKBD_Flush();

	IKBD_Uninstall();
	return 0;
}
