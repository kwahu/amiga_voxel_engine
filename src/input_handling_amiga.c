#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include "engine.h"
#include "settings_amiga.h"


void ProcessQualityInput()
{

	if (keyCheck(KEY_1) && renderingType != 1)
	{
		renderingDepth = 16;
		renderingType = 1;
		calculationDepthDivider = 1;
		calculationDepthStep = 10;
		renderingDepthStep = 2;
	//	Recalculate();
	}
	if (keyCheck(KEY_2) && renderingType != 2)
	{
		renderingDepth = 16;
		renderingType = 2;
		calculationDepthDivider = 1;
		calculationDepthStep = 10;
		renderingDepthStep = 2;
	//	Recalculate();
	}
	if (keyCheck(KEY_3) && renderingType != 3)
	{
		renderingDepth = 16;
		renderingType = 3;
		calculationDepthDivider = 1;
		calculationDepthStep = 10;
		renderingDepthStep = 2;
	//	Recalculate();
	}
	if (keyCheck(KEY_4) && renderingType != 4)
	{
		renderingDepth = 32;
		renderingType = 4;
		calculationDepthDivider = 2;
		calculationDepthStep = 2;
		renderingDepthStep = 1;
		RecalculateEven();
	}
	if (keyCheck(KEY_5) && renderingType != 5)
	{
		renderingDepth = 32;
		renderingType = 5;
		calculationDepthDivider = 2;
		calculationDepthStep = 2;
		renderingDepthStep = 1;
	//	Recalculate();
	}
	if (keyCheck(KEY_6) && renderingType != 6)
	{
		renderingDepth = 64;
		renderingType = 6;
		calculationDepthDivider = 2;
		calculationDepthStep = 2;
		renderingDepthStep = 1;
		RecalculateOdd();
	}
	if (keyCheck(KEY_7) && renderingType != 7 )
	{
		renderingDepth = 64;
		renderingType = 6;
		calculationDepthDivider = 2;
		calculationDepthStep = 2;
		renderingDepthStep = 1;
		RecalculateOdd();
	}
	if (keyCheck(KEY_8) && renderingType != 8 )
	{
		renderingDepth = 32;
		renderingType = 8;
			calculationDepthDivider = 2;
			calculationDepthStep = 2;
			//renderingDepthStep = 2;
		ClearBuffor();
		RecalculateOdd();
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
		cx += deltaTime / 100;
	}
	else if (joyCheck(JOY1_LEFT))
	{
		cx -= deltaTime / 100;
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
		//cy += deltaTime / 10;
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

	relativeHeight = ((p1h - 3) - (UBYTE)(mapHigh[(UBYTE)(p1x)][(UBYTE)(p1y + 15)]));
	acceleration = 5*((256 - relativeHeight) - (velocity/4));
	LONG addedpoints = (deltaTime/10000)*(128 - ((p1h - 3) - (UBYTE)(mapHigh[(UBYTE)(p1x)][(UBYTE)(p1y + 15)])));
	if(addedpoints > 0)
	{
		points += addedpoints;
	}
	LONG addedVelocity = 5*(acceleration/48);
	if(addedVelocity > 1000)
	{
		if(acceleration < 0)
		{
			acceleration = 5*((256 - relativeHeight) - (velocity/4));
			addedVelocity = 5*(acceleration/48);
		}
		else
		{
			addedVelocity = 1000 - velocity;	
		}
		
		
	}
	velocity = velocity + addedVelocity;

	if(velocity > 1000)
	{
		velocity = 1000;
	}
	if(velocity < 0)
	{
		velocity = 0;
	}

	p1yf += lowerDelta*velocity/velocityDenom;


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