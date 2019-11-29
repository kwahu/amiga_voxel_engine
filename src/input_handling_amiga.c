#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include "engine.h"
#include "settings_amiga.h"


void ProcessQualityInput()
{

	if (keyCheck(KEY_1) && debugValue != 1)
	{
		renderingDepth = 16;
		debugValue = 1;
		debugValue2 = 1;
		debugValue3 = 10;
		debugValue4 = 2;
	//	Recalculate();
	}
	if (keyCheck(KEY_2) && debugValue != 2)
	{
		renderingDepth = 16;
		debugValue = 2;
		debugValue2 = 1;
		debugValue3 = 10;
		debugValue4 = 2;
	//	Recalculate();
	}
	if (keyCheck(KEY_3) && debugValue != 3)
	{
		renderingDepth = 16;
		debugValue = 3;
		debugValue2 = 1;
		debugValue3 = 10;
		debugValue4 = 2;
	//	Recalculate();
	}
	if (keyCheck(KEY_4) && debugValue != 4)
	{
		renderingDepth = 32;
		debugValue = 4;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;
	//	Recalculate();
	}
	if (keyCheck(KEY_5) && debugValue != 5)
	{
		renderingDepth = 32;
		debugValue = 5;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;
	//	Recalculate();
	}
	if (keyCheck(KEY_6) && debugValue != 6)
	{
		renderingDepth = 32;
		debugValue = 6;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;
		RecalculateOdd();
	}
	if (keyCheck(KEY_7) && debugValue != 7 )
	{
		renderingDepth = 64;
		debugValue = 6;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;
		// debugValue = 7;
		// debugValue2 = 7;
		// debugValue3 = 1;
		// debugValue4 = 0;
		if(debugValue != 8 && debugValue != 7)
			RecalculateOdd();
	}
	if (keyCheck(KEY_8) )
	{
		debugValue = 8;
	}

}
void ProcessPlayerInput()
{
	/* if(joyCheck(JOY1_RIGHT)) { p1xf+=deltaTime/10000*3; }
		if(joyCheck(JOY1_LEFT)) {p1xf-=deltaTime/10000*3; }
		if(joyCheck(JOY1_DOWN)) { p1hf-=deltaTime/10000*3; }
		if(joyCheck(JOY1_UP)) { p1hf+=deltaTime/10000*3; }
		if(joyCheck(JOY1_FIRE)) { p1yf+=deltaTime/10000; }

		if(keyCheck(KEY_UP))p1yf+=deltaTime/10000;
		if(keyCheck(KEY_DOWN))p1yf-=deltaTime/10000;
		if(keyCheck(KEY_RIGHT))p1xf+=deltaTime/10000*3;
		if(keyCheck(KEY_LEFT))p1xf-=deltaTime/10000*3;
		if(keyCheck(KEY_RALT))p1hf+=deltaTime/10000*3;
		if(keyCheck(KEY_CONTROL))p1hf-=deltaTime/10000*3;*/

	if (joyCheck(JOY1_RIGHT))
	{
		cx += deltaTime / 150;
	}
	else if (joyCheck(JOY1_LEFT))
	{
		cx -= deltaTime / 150;
	}
	else if (cx != 0)
	{
		cx = cx - cx / ((LONG)(deltaTime) / 1000);
	}

	if (cx > 0x4000)
		cx = 0x4000;
	else if (cx < -0x4000)
		cx = -0x4000;

	if (cy > 0x2000)
		cy = 0x2000;
	else if (cy < -0x2000)
		cy = -0x2000;

	if (joyCheck(JOY1_DOWN))
	{
		cy += deltaTime / 60;
	}
	else if (joyCheck(JOY1_UP))
	{
		cy -= deltaTime / 60;
	}
	else if (cy != 0)
	{
		cy = cy - cy / ((LONG)(deltaTime) / 1000);
	}

	ULONG lowerDelta = (deltaTime/10000);

	acceleration = (256 - ((p1h - 3) - (UBYTE)(mapHigh[(UBYTE)(p1x)][(UBYTE)(p1y + 15)]))) - lowerDelta*velocity/32;
	LONG addedpoints = (deltaTime/10000)*(128 - ((p1h - 3) - (UBYTE)(mapHigh[(UBYTE)(p1x)][(UBYTE)(p1y + 15)])));
	if(addedpoints > 0)
	{
		points += addedpoints;
	}

	velocity = velocity + lowerDelta*(acceleration/48);

	p1yf += lowerDelta*velocity/128;


	p1xf += (LONG)lowerDelta * cx / 2000;
	p1hf -= (LONG)lowerDelta * cy / 1000;

	if (p1hf > 7000)
		p1hf = 7000; //block going above hills
	if (p1hf < 100)
		p1hf = 100; //block going below ground

	//p1yf+=deltaTime/4000;//(800 + (p1hf - (UBYTE)(mapHigh[p1xf/32][p1yf/32])*32 ) );

	p1y = p1yf / 32;
	p1x = p1xf / 32;
	p1h = p1hf / 32;
}