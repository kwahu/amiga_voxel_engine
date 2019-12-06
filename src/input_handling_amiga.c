#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include "engine.h"
#include "settings_amiga.h"
#include "bitmap.c"


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