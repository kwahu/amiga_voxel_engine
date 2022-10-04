
void InitLogoState()
{
	
    ResetTime();
	engine.currentState = State_Logo;
	for(int i = 0; i < 4; i++)
	{
		engine.logoState.fadeInStatus[i] = 1;
		engine.logoState.fadeOutStatus[i] = 0;
	}
	engine.logoState.screenIndex = 1;
	engine.logoState.screenDuration = LOGORUNTIME;
	UseSystem();
	
	engine.logoState.logo[0] = LoadBitmapFile("data/l1", &engine.logoState.headers[0], engine.activePalette, 1, 0);
	engine.logoState.logo[1] = LoadBitmapFile("data/l2", &engine.logoState.headers[1], engine.activePalette, 1, 0);
	engine.logoState.logo[2] = LoadBitmapFile("data/l3", &engine.logoState.headers[2], engine.activePalette, 1, 0);
	UnuseSystem();

	ClearBuffor();
	DrawBitmap4bCenter(engine.logoState.logo[0], &engine.logoState.headers[0]);
	
	VSyncAndDraw();
	
	
}

void RunLogoState()
{
	
    animateIntro();

    engine.logoState.screenDuration -= engine.deltaTime;

    if(engine.logoState.screenDuration > LOGORUNTIME)
    {
        engine.logoState.screenDuration = LOGORUNTIME;
        engine.logoState.screenIndex = (engine.logoState.screenIndex + 1) % 4;
        switchIntroScreen();
    }
    VSyncWait();

}

void switchIntroScreen()
{

	switch (engine.logoState.screenIndex)
	{
	case 2:
	{
		
		ClearBuffor();
		DrawBitmap4bCenter(engine.logoState.logo[1], &engine.logoState.headers[1]);
		
        VSyncAndDraw();

		for(int i = 0; i < 4; i++)
		{
			engine.logoState.fadeInStatus[i] = 0;
			engine.logoState.fadeOutStatus[i] = 0;
		}
	}
	break;
	case 3:
	{

		ClearBuffor();
		DrawBitmap4bCenter(engine.logoState.logo[2], &engine.logoState.headers[2]);
		
        VSyncAndDraw();

		for(int i = 0; i < 4; i++)
		{
			engine.logoState.fadeInStatus[i] = 0;
			engine.logoState.fadeOutStatus[i] = 0;
		}
	}
	break;
	default:
	{
		InitMenuState();
		
	} break;
	}
}

#ifdef AMIGA
#define PALETTE_SHIFT 4
#else
#define PALETTE_SHIFT 5
#endif 

void animateIntro()
{

	WORD ZeroColor[3] = 
	{
		engine.activePalette[0],
		engine.activePalette[1],
		engine.activePalette[2]
	};

	if(engine.logoState.screenDuration < 800000 && !engine.logoState.fadeInStatus[3])
	{
		for (int i = 1; i < 16; i++)
		{
			WORD newR = (WORD)(ZeroColor[2]) + ((engine.activePalette[i * 4 + 2] - ZeroColor[2])/4);
			WORD newG = (WORD)(ZeroColor[1]) + ((engine.activePalette[i * 4 + 1] - ZeroColor[1])/4);
			WORD newB = (WORD)(ZeroColor[0]) + ((engine.activePalette[i * 4 + 0] - ZeroColor[0])/4);

			engine.renderer.bitmapPalette[i] = ((((newR) >> PALETTE_SHIFT) << 8) +
								(((newG) >> PALETTE_SHIFT) << 4) + ((newB) >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeInStatus[3] = 1;
	}
	if(engine.logoState.screenDuration < 700000 && !engine.logoState.fadeInStatus[2])
	{
		for (int i = 1; i < 16; i++)
		{
			WORD newR = (WORD)(ZeroColor[2]) + ((engine.activePalette[i * 4 + 2] - ZeroColor[2])/2);
			WORD newG = (WORD)(ZeroColor[1]) + ((engine.activePalette[i * 4 + 1] - ZeroColor[1])/2);
			WORD newB = (WORD)(ZeroColor[0]) + ((engine.activePalette[i * 4 + 0] - ZeroColor[0])/2);

			engine.renderer.bitmapPalette[i] = ((((newR) >> PALETTE_SHIFT) << 8) +
								(((newG) >> PALETTE_SHIFT) << 4) + ((newB) >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeInStatus[2] = 1;
	}
	if(engine.logoState.screenDuration < 600000 && !engine.logoState.fadeInStatus[1])
	{
		for (int i = 1; i < 16; i++)
		{
			WORD newR = (WORD)(ZeroColor[2]) + ((engine.activePalette[i * 4 + 2] - ZeroColor[2])*3/4);
			WORD newG = (WORD)(ZeroColor[1]) + ((engine.activePalette[i * 4 + 1] - ZeroColor[1])*3/4);
			WORD newB = (WORD)(ZeroColor[0]) + ((engine.activePalette[i * 4 + 0] - ZeroColor[0])*3/4);

			engine.renderer.bitmapPalette[i] = ((((newR) >> PALETTE_SHIFT) << 8) +
								(((newG) >> PALETTE_SHIFT) << 4) + ((newB) >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeInStatus[1] = 1;
	}
	if(engine.logoState.screenDuration < 500000 && !engine.logoState.fadeInStatus[0])
	{
		for (int i = 1; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = (((engine.activePalette[i * 4 + 2] >> PALETTE_SHIFT) << 8) +
								((engine.activePalette[i * 4 + 1] >> PALETTE_SHIFT) << 4) + (engine.activePalette[i * 4 + 0] >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeInStatus[0] = 1;
	}

	if(engine.logoState.screenDuration < 400000 && !engine.logoState.fadeOutStatus[0])
	{
		for (int i = 1; i < 16; i++)
		{
			WORD newR = (WORD)(ZeroColor[2]) + ((engine.activePalette[i * 4 + 2] - ZeroColor[2])*3/4);
			WORD newG = (WORD)(ZeroColor[1]) + ((engine.activePalette[i * 4 + 1] - ZeroColor[1])*3/4);
			WORD newB = (WORD)(ZeroColor[0]) + ((engine.activePalette[i * 4 + 0] - ZeroColor[0])*3/4);

			engine.renderer.bitmapPalette[i] = ((((newR) >> PALETTE_SHIFT) << 8) +
								(((newG) >> PALETTE_SHIFT) << 4) + ((newB) >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeOutStatus[0] = 1;
	}
	if(engine.logoState.screenDuration < 300000 && !engine.logoState.fadeOutStatus[1])
	{
		for (int i = 1; i < 16; i++)
		{
			WORD newR = (WORD)(ZeroColor[2]) + ((engine.activePalette[i * 4 + 2] - ZeroColor[2])/2);
			WORD newG = (WORD)(ZeroColor[1]) + ((engine.activePalette[i * 4 + 1] - ZeroColor[1])/2);
			WORD newB = (WORD)(ZeroColor[0]) + ((engine.activePalette[i * 4 + 0] - ZeroColor[0])/2);

			engine.renderer.bitmapPalette[i] = ((((newR) >> PALETTE_SHIFT) << 8) +
								(((newG) >> PALETTE_SHIFT) << 4) + ((newB) >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeOutStatus[1] = 1;
	}
	if(engine.logoState.screenDuration < 200000 && !engine.logoState.fadeOutStatus[2])
	{
		for (int i = 1; i < 16; i++)
		{
			WORD newR = (WORD)(ZeroColor[2]) + ((engine.activePalette[i * 4 + 2] - ZeroColor[2])/4);
			WORD newG = (WORD)(ZeroColor[1]) + ((engine.activePalette[i * 4 + 1] - ZeroColor[1])/4);
			WORD newB = (WORD)(ZeroColor[0]) + ((engine.activePalette[i * 4 + 0] - ZeroColor[0])/4);

			engine.renderer.bitmapPalette[i] = ((((newR) >> PALETTE_SHIFT) << 8) +
								(((newG) >> PALETTE_SHIFT) << 4) + ((newB) >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeOutStatus[2] = 1;
	}
	if(engine.logoState.screenDuration < 100000 && !engine.logoState.fadeOutStatus[3])
	{
		for (int i = 1; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = (((ZeroColor[2] >> PALETTE_SHIFT) << 8) +
								((ZeroColor[1] >> PALETTE_SHIFT) << 4) + (ZeroColor[0] >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeOutStatus[3] = 1;
	}
}
