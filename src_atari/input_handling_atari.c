//#include <ace/managers/joy.h>
#include "../src/engine.h"
#include "key_map_atari.h"
#include "../ikbd/ikbd.h"

void ProcessQualityInputAtari()
{
	

	if(IKBD_Keyboard[KEY_1] && debugValue!=1)
	{
		renderingDepth = TERRAINDEPTH;
		debugValue = 1;
		debugValue2 = 2;
		debugValue3 = 4;
		debugValue4 = 1;
		DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(IKBD_Keyboard[KEY_2] && debugValue!=2)
	{
		renderingDepth = TERRAINDEPTH;
		debugValue = 2;
		debugValue2 = 2;
		debugValue3 = 4;
		debugValue4 = 1;
		DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(IKBD_Keyboard[KEY_3] && debugValue!=3)
	{
		renderingDepth = TERRAINDEPTH;
		debugValue = 3;
		debugValue2 = 2;
		debugValue3 = 4;
		debugValue4 = 1;
		DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(IKBD_Keyboard[KEY_4] && debugValue!=4)
	{
		renderingDepth = TERRAINDEPTH;
		debugValue = 4;
		debugValue2 = 2;
		debugValue3 = 4;
		debugValue4 = 1;
		DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(IKBD_Keyboard[KEY_5] && debugValue!=5)
	{
		renderingDepth = TERRAINDEPTH;
		debugValue = 5;
		debugValue2 = 2;
		debugValue3 = 4;
		debugValue4 = 1;
		DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(IKBD_Keyboard[KEY_6] && debugValue!=6)
	{
		renderingDepth = TERRAINDEPTH;
		debugValue = 6;
		debugValue2 = 2;
		debugValue3 = 4;
		debugValue4 = 1;
		DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if(IKBD_Keyboard[KEY_7] && debugValue!=7)
	{
		renderingDepth = TERRAINDEPTH;
		debugValue = 7;
		debugValue2 = 2;
		debugValue3 = 4;
		debugValue4 = 1;
		DrawBitmap8b(bitmap1, &bitmapHeader1);
	}
	if (IKBD_Keyboard[KEY_8] && debugValue!=8 )
	{
		renderingDepth = TERRAINDEPTH;
		debugValue = 8;
		debugValue2 = 2;
		debugValue3 = 4;
		debugValue4 = 1;
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

		if((IKBD_STICK1 & IKBD_JOY_RIGHT) || IKBD_Keyboard[KEY_RIGHT]) {	cx+=deltaTime/100; }
 		else if((IKBD_STICK1 & IKBD_JOY_LEFT) || IKBD_Keyboard[KEY_LEFT]) {		cx-=deltaTime/100; }
		else if(cx!=0) {cx = cx - cx/((LONG)(deltaTime)/1000);}

		if(cx > 0x4000) cx = 0x4000;
		else if(cx < -0x4000) cx = -0x4000;

		if(cy > 0x2000) cy = 0x2000;
		else if(cy < -0x2000) cy = -0x2000;

 		if((IKBD_STICK1 & IKBD_JOY_DOWN) || IKBD_Keyboard[KEY_DOWN]) {		cy+=deltaTime/100; }
 		else if((IKBD_STICK1 & IKBD_JOY_UP) || IKBD_Keyboard[KEY_UP]) {			cy-=deltaTime/100; }
		else if(cy!=0) {cy = cy - cy/((LONG)(deltaTime)/1000);}

 		if((IKBD_Joystick1 & IKBD_JOY_FIRE ) || IKBD_Keyboard[KEY_CTRL]) 
		 {		p1yf+=deltaTime/2000;}



		p1xf += (LONG)(deltaTime/10000) * cx/2000;
		p1hf -= (LONG)(deltaTime/10000) * cy/1000;

		if(p1hf > 7000) p1hf = 7000;//block going above hills
		if(p1hf<100) p1hf = 100;//block going below ground

		//p1yf+=deltaTime/4000;//(800 + (p1hf - (UBYTE)(mapHigh[p1xf/32][p1yf/32])*32 ) );

		xOffsetOdd = cx/300;
		xOffsetEven = cx/450;


	p1y = p1yf/32;
	p1x = p1xf/32;
	p1h = p1hf/32;
}