#include <ace/managers/joy.h>
#include "engine.h"

void ProcessQualityInput()
{
	

	if(keyCheck(KEY_1) && debugValue!=1)
	{
		renderingDepth = 16;
		debugValue=1;
		debugValue2 = 1;
		debugValue3 = 10;
		debugValue4 = 2;
		Recalculate();
	}
	if(keyCheck(KEY_2) && debugValue!=2)
	{
		renderingDepth = 16;
		debugValue=2;
		debugValue2 = 1;
		debugValue3 = 10;
		debugValue4 = 2;
		Recalculate();
	}
	if(keyCheck(KEY_3) && debugValue!=3)
	{
		renderingDepth = 16;
		debugValue=3;
		debugValue2 = 1;
		debugValue3 = 10;
		debugValue4 = 2;
		Recalculate();
	}
	if(keyCheck(KEY_4) && debugValue!=4)
	{
		renderingDepth = 32;
		debugValue=4;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;
		Recalculate();
	}
	if(keyCheck(KEY_5) && debugValue!=5)
	{
		renderingDepth = 32;
		debugValue=5;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;
		Recalculate();
	}
	if(keyCheck(KEY_6) && debugValue!=6)
	{
		renderingDepth = 32;
		debugValue=6;
		debugValue2 = 2;
		debugValue3 = 2;
		debugValue4 = 1;
		Recalculate();
	}
	if(keyCheck(KEY_7) && debugValue!=7)
	{
		renderingDepth = 64;
		debugValue=7;     
		debugValue2 = 7;
		debugValue3 = 1;
		debugValue4 = 0;
		Recalculate();
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

		if(joyCheck(JOY1_RIGHT)) {	cx+=deltaTime/100; }
 		else if(joyCheck(JOY1_LEFT)) {		cx-=deltaTime/100; }
		else if(cx!=0) {cx = cx - cx/((LONG)(deltaTime)/1000);}

		if(cx > 0x3000) cx = 0x3000;
		else if(cx < -0x3000) cx = -0x3000;

		if(cy > 0x2000) cy = 0x2000;
		else if(cy < -0x2000) cy = -0x2000;

 		if(joyCheck(JOY1_DOWN)) {		cy+=deltaTime/100; }
 		else if(joyCheck(JOY1_UP)) {			cy-=deltaTime/100; }
		else if(cy!=0) {cy = cy - cy/((LONG)(deltaTime)/1000);}

 		if(joyCheck(JOY1_FIRE)) {		p1yf+=deltaTime/2000;}



		p1xf += (LONG)(deltaTime/10000) * cx/2000;
		p1hf -= (LONG)(deltaTime/10000) * cy/1000;

		if(p1hf > 7000) p1hf = 7000;//block going above hills
		if(p1hf<100) p1hf = 100;//block going below ground

		//p1yf+=deltaTime/4000;//(800 + (p1hf - (UBYTE)(mapHigh[p1xf/32][p1yf/32])*32 ) );



	p1y = p1yf/32;
	p1x = p1xf/32;
	p1h = p1hf/32;
}