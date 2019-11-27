//#include <ace/managers/joy.h>
#include "engine.h"
#include "key_map.h"
#include "../ikbd/ikbd.h"

void ProcessQualityInputAtari()
{
	

	if(IKBD_Keyboard[KEY_1] && debugValue!=1)
	{
		renderingDepth = TERRAINDEPTH/4;
		debugValue=1;
		debugValue2 = 1;
		debugValue3 = 10;
		debugValue4 = 2;
		Recalculate();
	}
	if(IKBD_Keyboard[KEY_2] && debugValue!=2)
	{
		renderingDepth = TERRAINDEPTH/4;
		debugValue=2;
		debugValue2 = 1;
		debugValue3 = 10;
		debugValue4 = 2;
		Recalculate();
	}
	if(IKBD_Keyboard[KEY_3] && debugValue!=3)
	{
		renderingDepth = TERRAINDEPTH/4;
		debugValue=3;
		debugValue2 = 1;
		debugValue3 = 10;
		debugValue4 = 2;
		Recalculate();
	}
	if(IKBD_Keyboard[KEY_4] && debugValue!=4)
	{
		renderingDepth = TERRAINDEPTH/2;
		debugValue=4;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;
		Recalculate();
	}
	if(IKBD_Keyboard[KEY_5] && debugValue!=5)
	{
		renderingDepth = TERRAINDEPTH/2;
		debugValue=5;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;
		Recalculate();
	}
	if(IKBD_Keyboard[KEY_6] && debugValue!=6)
	{
		renderingDepth = TERRAINDEPTH/2;
		debugValue=6;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;
		Recalculate();
	}
	if(IKBD_Keyboard[KEY_7] && debugValue!=7)
	{
		renderingDepth = TERRAINDEPTH;
		debugValue = 6;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;
		// debugValue=7;     
		// debugValue2 = 7;
		// debugValue3 = 1;
		// debugValue4 = 0;
		if(debugValue != 8 && debugValue != 7)
			Recalculate();
	}
	if (IKBD_Keyboard[KEY_8] )
	{
		debugValue = 8;
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