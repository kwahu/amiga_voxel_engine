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
	engine.renderer.zStart = 12;
	if (getKey(3))
	{
		engine.renderer.renderingDepth = 24;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 4;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		if(engine.renderer.highMemory)
		{
		engine.renderer.turnDenom = 2000;
			engine.renderer.xFOV = 20;
			engine.renderer.yFOV = 30;
			engine.renderer.renderingType = 4;
		RecalculateOdd();
		}
		else
		{
		engine.renderer.turnDenom = 3000;
			engine.renderer.xFOV = 23;
			engine.renderer.yFOV = 15;
			engine.renderer.renderingType = 1;
			RecalculateEven();
		}
		
	}
	if (getKey(4)) //A1200
	{
		engine.renderer.renderingDepth = 32;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 3;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		if(engine.renderer.highMemory)
		{
		engine.renderer.turnDenom = 2000;
			engine.renderer.xFOV = 12;
			engine.renderer.yFOV = 20;
			engine.renderer.renderingType = 5;
		RecalculateOdd();
		}
		else
		{
		engine.renderer.turnDenom = 3000;
			engine.renderer.xFOV = 18;
			engine.renderer.yFOV = 12;
			engine.renderer.renderingType = 2;
			RecalculateEven();
		}
	}
	if (getKey(5)) //A3000
	{
		engine.renderer.renderingDepth = 64;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		if(engine.renderer.highMemory)
		{
		engine.renderer.turnDenom = 2000;
		engine.renderer.xFOV = 8;
		engine.renderer.yFOV = 18;
			engine.renderer.renderingType = 6;
		RecalculateOdd();
		}
		else
		{
		engine.renderer.turnDenom = 3000;
			engine.renderer.xFOV = 18;
			engine.renderer.yFOV = 10;
			engine.renderer.renderingType = 3;
			RecalculateEven();	
		}
	}
}

// void ProcessQualityInput()
// {

// 	if (getKey(1) && engine.renderer.renderingType != 1)
// 	{
// 		engine.renderer.renderingDepth = 16;
// 		engine.renderer.renderingType = 1;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 2;
// 		engine.renderer.stepModifier = 16;
// 		engine.renderer.xFOV = 32;
// 		engine.renderer.yFOV = 12;
// 		ClearBuffor();
// 	 	RecalculateEven();
// 	}
// 	if (getKey(2) && engine.renderer.renderingType != 2)
// 	{
// 		engine.renderer.renderingDepth = 16;
// 		engine.renderer.renderingType = 2;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 3;
// 		engine.renderer.renderingDepthStep = 2;
// 		engine.renderer.stepModifier = 16;
// 		engine.renderer.xFOV = 28;
// 		engine.renderer.yFOV = 12;
// 		ClearBuffor();
// 		RecalculateEven();
// 	}
// 	if (getKey(3) && engine.renderer.renderingType != 3)
// 	{
// 		engine.renderer.renderingDepth = 16;
// 		engine.renderer.renderingType = 3;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 2;
// 		engine.renderer.stepModifier = 16;
// 		engine.renderer.xFOV = 24;
// 		engine.renderer.yFOV = 12;
// 		ClearBuffor();
// 		RecalculateOdd();
// 	}
// 	if (getKey(4) && engine.renderer.renderingType != 4)
// 	{
// 		engine.renderer.renderingDepth = 32;
// 		engine.renderer.renderingType = 4;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 		engine.renderer.stepModifier = 16;
// 		engine.renderer.xFOV = 20;
// 		engine.renderer.yFOV = 24;
// 		ClearBuffor();
// 		RecalculateOdd();
// 	}
// 	if (getKey(5) && engine.renderer.renderingType != 5)
// 	{
// 		engine.renderer.renderingDepth = 32;
// 		engine.renderer.renderingType = 5;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		RecalculateOdd();
// 	}
// 	if (getKey(6) && engine.renderer.renderingType != 6)
// 	{
// 		engine.renderer.renderingDepth = 64;
// 		engine.renderer.renderingType = 6;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 		engine.renderer.stepModifier = 16;
// 		engine.renderer.xFOV = 10;
// 		engine.renderer.yFOV = 18;
// 		ClearBuffor();
// 		RecalculateOdd();
// 	}
// 	if (getKey(7) && engine.renderer.renderingType != 7 )
// 	{
// 		engine.renderer.renderingDepth = 64;
// 		engine.renderer.renderingType = 7;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		RecalculateOdd();
// 	}
// 	if (getKey(8) && engine.renderer.renderingType != 8 )
// 	{
// 		engine.renderer.renderingDepth = 64;
// 		engine.renderer.renderingType = 8;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		RecalculateOdd();
// 	}

// }

 #else

#define _hz_200 *(volatile long *)0x4ba

#define timerGetPrec() _hz_200*12500;

#include "key_map_atari.h"
#include "../ikbd/ikbd.h"

#define getJoy(index, button) (IKBD_STICK ##index & IKBD_JOY_ ##button) ||IKBD_Keyboard[KEY_ ##button ]
#define getKey(name) IKBD_Keyboard[KEY_ ##name ]


void ProcessJoystick()
{
}


void CloseJoystick()
{
	IKBD_Flush();

	IKBD_Uninstall();
}

void UseSystem()
{
}
void UnuseSystem()
{
}

void ExitGame()
{
}


void InitInput()
{
	IKBD_Install();
}

void GetPlayerRendererSetting()
{
	engine.renderer.zStart = 12;
		engine.renderer.turnDenom = 2000;
	if (getKey(3))
	{
		engine.renderer.renderingDepth = TERRAINDEPTH;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		if(engine.renderer.highMemory)
		{
		engine.renderer.xFOV = 24;
		engine.renderer.yFOV = 14;
			engine.renderer.renderingType = 3;
		RecalculateOdd();
		}
		else
		{
		engine.renderer.xFOV = 24;
		engine.renderer.yFOV = 14;
			engine.renderer.renderingType = 1;
			RecalculateEven();
		}
	}
	if (getKey(4)) 
	{
		engine.renderer.renderingDepth = TERRAINDEPTH;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		if(engine.renderer.highMemory)
		{
		engine.renderer.xFOV = 28;
		engine.renderer.yFOV = 14;
			engine.renderer.renderingType = 4;
		RecalculateOdd();
		}
		else
		{
		engine.renderer.xFOV = 28;
		engine.renderer.yFOV = 14;
			engine.renderer.renderingType = 2;
			RecalculateEven();
		}
	}
}

// void ProcessQualityInput()
// {
	

// 	if(getKey(1) && engine.renderer.renderingType!=1)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 1;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if(getKey(2) && engine.renderer.renderingType!=2)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 2;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if(getKey(3) && engine.renderer.renderingType!=3)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 3;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if(getKey(4)  && engine.renderer.renderingType!=4)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 4;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if(getKey(5) && engine.renderer.renderingType!=5)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 5;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if(getKey(6) && engine.renderer.renderingType!=6)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 6;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if(getKey(7) && engine.renderer.renderingType!=7)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 7;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if (getKey(8) && engine.renderer.renderingType!=8 )
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 8;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 	}


// }

#endif

void GetYAxisSetting()
{
	if (getKey(9))
	{
		engine.yAxis = -1;
	}
	if (getKey(0))
	{
		engine.yAxis = 1;
	}
	
}

void GetPlayerMemorySetting()
{
	if(getKey(1))
	{
		engine.renderer.depthBufferHeight = YSIZEEVEN;
		#ifdef AMIGA
		engine.renderer.shadowStep = 4;
		#else
		engine.renderer.shadowStep = 8;
		#endif
		engine.renderer.depthBuffer = (UBYTE *)malloc(YSIZEEVEN*sizeof(UBYTE));
		engine.renderer.ditherTable1 = (UBYTE *)malloc(4*COLORS*COLORS*sizeof(UBYTE));
		engine.renderer.ditherTable2 = engine.renderer.ditherTable1 + COLORS*COLORS;
		engine.renderer.ditherTable3 = engine.renderer.ditherTable1 + 2*COLORS*COLORS;
		engine.renderer.ditherTable4 = engine.renderer.ditherTable1 + 3*COLORS*COLORS;
		engine.renderer.screenPatch = (UBYTE *)malloc(4*45*sizeof(UBYTE));
		engine.renderer.rayCastX = (WORD *)malloc(XSIZEEVEN*TERRAINDEPTH*sizeof(WORD));
		engine.renderer.rayCastY = (WORD *)malloc(YSIZEEVEN*TERRAINDEPTH*sizeof(WORD));
		engine.renderer.highMemory = 0;
		GenerateColorBytesNoDither4x4();
	}
	if(getKey(2))
	{	
		engine.renderer.depthBufferHeight = YSIZEODD;
		#ifdef AMIGA
		engine.renderer.shadowStep = 2;
		#else
		engine.renderer.shadowStep = 4;
		#endif
		engine.renderer.depthBuffer = (UBYTE *)malloc(YSIZEODD*sizeof(UBYTE));
		engine.renderer.ditherTable1 = (UBYTE *)malloc(4*COLORS*COLORS*COLORS*sizeof(UBYTE));
		engine.renderer.ditherTable2 = engine.renderer.ditherTable1 + COLORS*COLORS*COLORS;
		engine.renderer.ditherTable3 = engine.renderer.ditherTable1 + 2*COLORS*COLORS*COLORS;
		engine.renderer.ditherTable4 = engine.renderer.ditherTable1 + 3*COLORS*COLORS*COLORS;
		engine.renderer.screenPatch = (UBYTE *)malloc(6*90*sizeof(UBYTE));
		engine.renderer.rayCastX = (WORD *)malloc(XSIZEODD*TERRAINDEPTH*sizeof(WORD));
		engine.renderer.rayCastY = (WORD *)malloc(YSIZEODD*TERRAINDEPTH*sizeof(WORD));
		engine.renderer.highMemory = 1;
		GenerateColorBytesDither3x2();
	}
}

#define getCurrentTime() timerGetPrec()
