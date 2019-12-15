#pragma once
 #ifdef AMIGA

#include <ace/managers/timer.h>
#include <ace/managers/joy.h>
#include <ace/managers/key.h>

#define getJoy(index, button) joyCheck(JOY ##index## _ ##button)
#define getKey(name) keyCheck(KEY_ ##name)

void ProcessJoystick()
{
	joyProcess();
}


void CloseJoystick()
{
	joyClose();
}

void InitInput()
{
	keyCreate();

	joyOpen(0);
}

void UseSystem()
{
	systemUse();
}
void UnuseSystem()
{
	systemUnuse();
}

void ExitGame()
{
	gameClose();
}

void GetPlayerRendererSetting()
{
	if (getKey(1))
	{
		engine.renderer.renderingDepth = 16;
		engine.renderer.renderingType = 1;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 4;
		engine.renderer.renderingDepthStep = 2;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		engine.renderer.xFOV = 32;
		engine.renderer.yFOV = 12;
		RecalculateEven();
	}
	if (getKey(2)) //A1200
	{
		engine.renderer.renderingDepth = 32;
		engine.renderer.renderingType = 4;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		engine.renderer.xFOV = 20;
		engine.renderer.yFOV = 24;
		RecalculateOdd();
	}
	if (getKey(3)) //A3000
	{
		engine.renderer.renderingDepth = 64;
		engine.renderer.renderingType = 6;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		engine.renderer.xFOV = 10;
		engine.renderer.yFOV = 18;
		RecalculateOdd();
	}
}

void ProcessQualityInput()
{

	if (getKey(1) && engine.renderer.renderingType != 1)
	{
		engine.renderer.renderingDepth = 16;
		engine.renderer.renderingType = 1;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 4;
		engine.renderer.renderingDepthStep = 2;
		engine.renderer.stepModifier = 16;
		engine.renderer.xFOV = 32;
		engine.renderer.yFOV = 12;
		ClearBuffor();
	 	RecalculateEven();
	}
	if (getKey(2) && engine.renderer.renderingType != 2)
	{
		engine.renderer.renderingDepth = 16;
		engine.renderer.renderingType = 2;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 3;
		engine.renderer.renderingDepthStep = 2;
		engine.renderer.stepModifier = 16;
		engine.renderer.xFOV = 28;
		engine.renderer.yFOV = 12;
		ClearBuffor();
		RecalculateEven();
	}
	if (getKey(3) && engine.renderer.renderingType != 3)
	{
		engine.renderer.renderingDepth = 16;
		engine.renderer.renderingType = 3;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 2;
		engine.renderer.stepModifier = 16;
		engine.renderer.xFOV = 24;
		engine.renderer.yFOV = 12;
		ClearBuffor();
		RecalculateOdd();
	}
	if (getKey(4) && engine.renderer.renderingType != 4)
	{
		engine.renderer.renderingDepth = 32;
		engine.renderer.renderingType = 4;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.stepModifier = 16;
		engine.renderer.xFOV = 20;
		engine.renderer.yFOV = 24;
		ClearBuffor();
		RecalculateOdd();
	}
	if (getKey(5) && engine.renderer.renderingType != 5)
	{
		engine.renderer.renderingDepth = 32;
		engine.renderer.renderingType = 5;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		ClearBuffor();
		RecalculateOdd();
	}
	if (getKey(6) && engine.renderer.renderingType != 6)
	{
		engine.renderer.renderingDepth = 64;
		engine.renderer.renderingType = 6;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.stepModifier = 16;
		engine.renderer.xFOV = 10;
		engine.renderer.yFOV = 18;
		ClearBuffor();
		RecalculateOdd();
	}
	if (getKey(7) && engine.renderer.renderingType != 7 )
	{
		engine.renderer.renderingDepth = 64;
		engine.renderer.renderingType = 7;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		ClearBuffor();
		RecalculateOdd();
	}
	if (getKey(8) && engine.renderer.renderingType != 8 )
	{
		engine.renderer.renderingDepth = 64;
		engine.renderer.renderingType = 8;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		ClearBuffor();
		RecalculateOdd();
	}

}

 #else

#define _hz_200 *(volatile long *)0x4ba

#define timerGetPrec() _hz_200*12500;

#include "key_map_atari.h"
#include "../ikbd/ikbd.h"

#define getJoy(index, button) (IKBD_STICK ##index & IKBD_JOY_ ##button) ||IKBD_Keyboard[KEY_ ##button ]
#define getKey(name) IKBD_Keyboard[KEY_ ##name ]


void ExitGame()
{
}


void ProcessQualityInput()
{
	

	if(getKey(1) && engine.renderer.renderingType!=1)
	{
		engine.renderer.renderingDepth = TERRAINDEPTH;
		engine.renderer.renderingType = 1;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 4;
		engine.renderer.renderingDepthStep = 1;
		ClearBuffor();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(getKey(2) && engine.renderer.renderingType!=2)
	{
		engine.renderer.renderingDepth = TERRAINDEPTH;
		engine.renderer.renderingType = 2;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 4;
		engine.renderer.renderingDepthStep = 1;
		ClearBuffor();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(getKey(3) && engine.renderer.renderingType!=3)
	{
		engine.renderer.renderingDepth = TERRAINDEPTH;
		engine.renderer.renderingType = 3;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 4;
		engine.renderer.renderingDepthStep = 1;
		ClearBuffor();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(getKey(4)  && engine.renderer.renderingType!=4)
	{
		engine.renderer.renderingDepth = TERRAINDEPTH;
		engine.renderer.renderingType = 4;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 4;
		engine.renderer.renderingDepthStep = 1;
		ClearBuffor();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(getKey(5) && engine.renderer.renderingType!=5)
	{
		engine.renderer.renderingDepth = TERRAINDEPTH;
		engine.renderer.renderingType = 5;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 4;
		engine.renderer.renderingDepthStep = 1;
		ClearBuffor();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(getKey(6) && engine.renderer.renderingType!=6)
	{
		engine.renderer.renderingDepth = TERRAINDEPTH;
		engine.renderer.renderingType = 6;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 4;
		engine.renderer.renderingDepthStep = 1;
		ClearBuffor();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(getKey(7) && engine.renderer.renderingType!=7)
	{
		engine.renderer.renderingDepth = TERRAINDEPTH;
		engine.renderer.renderingType = 7;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		ClearBuffor();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if (getKey(8) && engine.renderer.renderingType!=8 )
	{
		engine.renderer.renderingDepth = TERRAINDEPTH;
		engine.renderer.renderingType = 8;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
	}


}

#endif

#define getCurrentTime() timerGetPrec()
