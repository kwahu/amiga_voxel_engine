//#include <ace/managers/joy.h>
#include "../src/engine.h"
#include "key_map_atari.h"
#include "../ikbd/ikbd.h"

void ProcessQualityInputAtari()
{
	

	if(IKBD_Keyboard[KEY_1] && renderingType!=1)
	{
		renderingDepth = TERRAINDEPTH;
		renderingType = 1;
		calculationDepthDivider = 2;
		calculationDepthStep = 4;
		renderingDepthStep = 1;
		ClearScreen();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(IKBD_Keyboard[KEY_2] && renderingType!=2)
	{
		renderingDepth = TERRAINDEPTH;
		renderingType = 2;
		calculationDepthDivider = 2;
		calculationDepthStep = 4;
		renderingDepthStep = 1;
		ClearScreen();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(IKBD_Keyboard[KEY_3] && renderingType!=3)
	{
		renderingDepth = TERRAINDEPTH;
		renderingType = 3;
		calculationDepthDivider = 2;
		calculationDepthStep = 4;
		renderingDepthStep = 1;
		ClearScreen();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(IKBD_Keyboard[KEY_4] && renderingType!=4)
	{
		renderingDepth = TERRAINDEPTH;
		renderingType = 4;
		calculationDepthDivider = 2;
		calculationDepthStep = 4;
		renderingDepthStep = 1;
		ClearScreen();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(IKBD_Keyboard[KEY_5] && renderingType!=5)
	{
		renderingDepth = TERRAINDEPTH;
		renderingType = 5;
		calculationDepthDivider = 2;
		calculationDepthStep = 4;
		renderingDepthStep = 1;
		ClearScreen();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(IKBD_Keyboard[KEY_6] && renderingType!=6)
	{
		renderingDepth = TERRAINDEPTH;
		renderingType = 6;
		calculationDepthDivider = 2;
		calculationDepthStep = 4;
		renderingDepthStep = 1;
		ClearScreen();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(IKBD_Keyboard[KEY_7] && renderingType!=7)
	{
		renderingDepth = TERRAINDEPTH;
		renderingType = 7;
		calculationDepthDivider = 2;
		calculationDepthStep = 2;
		renderingDepthStep = 1;
		ClearScreen();
		//DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if (IKBD_Keyboard[KEY_8] && renderingType!=8 )
	{
		renderingDepth = TERRAINDEPTH;
		renderingType = 8;
		calculationDepthDivider = 2;
		calculationDepthStep = 2;
		renderingDepthStep = 1;
	}


}
void ProcessPlayerInputAtari()
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

		//int denom = 1000;
		int denom = 1;//666;

		if((IKBD_STICK1 & IKBD_JOY_RIGHT) || IKBD_Keyboard[KEY_RIGHT]) {	cx+=deltaTime*2; }
 		else if((IKBD_STICK1 & IKBD_JOY_LEFT) || IKBD_Keyboard[KEY_LEFT]) {		cx-=deltaTime*2; }
		else if(cx!=0) {cx = cx - cx/((LONG)(deltaTime)/denom);}

		if(cx > 0x4000) cx = 0x4000;
		else if(cx < -0x4000) cx = -0x4000;

		if(cy > 0x2000) cy = 0x2000;
		else if(cy < -0x2000) cy = -0x2000;

 		if((IKBD_STICK1 & IKBD_JOY_DOWN) || IKBD_Keyboard[KEY_DOWN]) {		cy+=deltaTime; }
 		else if((IKBD_STICK1 & IKBD_JOY_UP) || IKBD_Keyboard[KEY_UP]) {			cy-=deltaTime; }
		else if(cy!=0) {cy = cy - cy/((LONG)(deltaTime)/denom);}

		ULONG lowerDelta = (deltaTime/(denom*4));

		relativeHeight = ((p1h - 3) - (UBYTE)(mapHigh[(UBYTE)(p1x)][(UBYTE)(p1y + 15)]));
		acceleration = 5*((256 - relativeHeight) - velocity/4);
		LONG addedpoints = (lowerDelta)*(128 - ((p1h - 3) - (UBYTE)(mapHigh[(UBYTE)(p1x)][(UBYTE)(p1y + 15)])));
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
		
		velocity += addedVelocity;

		if(velocity > 1000)
		{
			velocity = 1000;
		}
		if(velocity < 0)
		{
			velocity = 0;
		}

		p1yf += lowerDelta*velocity/velocityDenom;



		p1xf += (LONG)lowerDelta * cx/50;
		p1hf -= (LONG)lowerDelta * cy/20;

		if(p1hf > 7000) p1hf = 7000;//block going above hills
		if(p1hf<100) p1hf = 100;//block going below ground

		//p1yf+=deltaTime/4000;//(800 + (p1hf - (UBYTE)(mapHigh[p1xf/32][p1yf/32])*32 ) );

		xOffsetOdd = cx/30;
		xOffsetEven = cx/45;


	p1y = p1yf/32;
	p1x = p1xf/32;
	p1h = p1hf/32;
}