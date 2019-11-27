/*
 * test program 2 for IKBD routines
 * (c) 2010/14 by Simon Sunnyboy / Paradize
 *
 * poll joysticks and display their values
 */

#include <stdint.h>
#include "ikbd.h"
#include <osbind.h>

#define COLOR0 (*(uint16_t*)0xFFFF8240UL)

#define PRINT(x) (void)Cconws(x)

struct joydecode
{
    char *txt;
    uint8_t mask;
};

struct joydecode joydecode[] =
{
    {"up ",IKBD_JOY_UP},
    {"down ",IKBD_JOY_DOWN},
    {"left ",IKBD_JOY_LEFT},
    {"right ",IKBD_JOY_RIGHT},
    {"fire ",IKBD_JOY_FIRE},
};


int main(void)
{
	uint8_t exitflag = 0;

    uint8_t idx,joy_id,joy[2];


	IKBD_Install();
	IKBD_MouseOff();
	PRINT("\033EIKBD routines for joystick installed!\r\n");

	while(exitflag == 0)
	{
		Vsync();
		IKBD_ReadMouse();
		if(IKBD_IsKeyPressed(IKBD_KEY_ESC))
		{
		    PRINT("ESC\r\n");
			exitflag = 1;
		}

        joy[0] = IKBD_Joystick0;
		joy[1] = IKBD_Joystick1;

            for(joy_id = 0; joy_id < 2; joy_id ++)
            {
                for(idx = 0; idx < 5; idx++)
                {
                    if(joy[joy_id] & joydecode[idx].mask)
                    {
                        Bconout(2,'0'+joy_id);
                        Bconout(2,' ');
                        PRINT(joydecode[idx].txt);
                        PRINT("\r\n");
                    }
                }
            }

	}

    PRINT("...exiting\r\n");

    IKBD_MouseOn();

	IKBD_Flush();

	IKBD_Uninstall();
	return 0;
}
