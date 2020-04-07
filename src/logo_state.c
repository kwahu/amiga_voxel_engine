
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
		UseSystem();
		free(engine.logoState.logo[0]);
		free(engine.logoState.logo[1]);
		free(engine.logoState.logo[2]);

		engine.explosionBitmap = LoadBitmapFile("data/iexpl", &engine.explosionHeader, engine.palettePalette, 2, 14);
		engine.landingBitmap = LoadBitmapFile("data/land", &engine.landingHeader, engine.palettePalette, 2, 14);
		engine.takeoffBitmap = LoadBitmapFile("data/take", &engine.takeoffHeader, engine.palettePalette, 2, 14);
		engine.shipBitmap = LoadBitmapFile("data/icar48", &engine.shipHeader, engine.activePalette, 2, 14);

		
		engine.pBitmapVelocity = CreateBitmapFromText(engine.font, "1234");
		engine.pBitmapScore = CreateBitmapFromText(engine.font, "1234567");
		engine.pBitmapTime = CreateBitmapFromText(engine.font, "1234567");
		engine.pBitmapHeight = CreateBitmapFromText(engine.font, "1234");
		engine.pBitmapVelocityLabel = CreateBitmapFromText(engine.font, "AIR SPEED");
		engine.pBitmapScoreLabel = CreateBitmapFromText(engine.font, "SCORE");
		engine.pBitmapTimeLabel = CreateBitmapFromText(engine.font, "TIME");
		engine.pBitmapHeightLabel = CreateBitmapFromText(engine.font, "RELATIVE ALTITUDE");
		for(int i = 0; i < 10; ++i)
		{
			engine.pBitmapInfo[i] = CreateFontBitmap(engine.font);
		}


		InitMenuState();
		UnuseSystem();
		InitAudio();
		PlaySample(0);
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
	if(engine.logoState.screenDuration < 7400000 && !engine.logoState.fadeInStatus[3])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = ((((engine.activePalette[i * 4 + 2]/4) >> PALETTE_SHIFT) << 8) +
								(((engine.activePalette[i * 4 + 1]/4) >> PALETTE_SHIFT) << 4) + ((engine.activePalette[i * 4 + 0]/4) >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeInStatus[3] = 1;
	}
	if(engine.logoState.screenDuration < 7300000 && !engine.logoState.fadeInStatus[2])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = ((((engine.activePalette[i * 4 + 2]/2) >> PALETTE_SHIFT) << 8) +
								(((engine.activePalette[i * 4 + 1]/2) >> PALETTE_SHIFT) << 4) + ((engine.activePalette[i * 4 + 0]/2) >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeInStatus[2] = 1;
	}
	if(engine.logoState.screenDuration < 7200000 && !engine.logoState.fadeInStatus[1])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = ((((engine.activePalette[i * 4 + 2]*3/4) >> PALETTE_SHIFT) << 8) +
								(((engine.activePalette[i * 4 + 1]*3/4) >> PALETTE_SHIFT) << 4) + ((engine.activePalette[i * 4 + 0]*3/4) >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeInStatus[1] = 1;
	}
	if(engine.logoState.screenDuration < 7100000 && !engine.logoState.fadeInStatus[0])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = (((engine.activePalette[i * 4 + 2] >> PALETTE_SHIFT) << 8) +
								((engine.activePalette[i * 4 + 1] >> PALETTE_SHIFT) << 4) + (engine.activePalette[i * 4 + 0] >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeInStatus[0] = 1;
	}

	if(engine.logoState.screenDuration < 400000 && !engine.logoState.fadeOutStatus[0])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = ((((engine.activePalette[i * 4 + 2]*3/4) >> PALETTE_SHIFT) << 8) +
								(((engine.activePalette[i * 4 + 1]*3/4) >> PALETTE_SHIFT) << 4) + ((engine.activePalette[i * 4 + 0]*3/4) >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeOutStatus[0] = 1;
	}
	if(engine.logoState.screenDuration < 300000 && !engine.logoState.fadeOutStatus[1])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = ((((engine.activePalette[i * 4 + 2]/2) >> PALETTE_SHIFT) << 8) +
								(((engine.activePalette[i * 4 + 1]/2) >> PALETTE_SHIFT) << 4) + ((engine.activePalette[i * 4 + 0]/2) >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeOutStatus[1] = 1;
	}
	if(engine.logoState.screenDuration < 200000 && !engine.logoState.fadeOutStatus[2])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = ((((engine.activePalette[i * 4 + 2]/4) >> PALETTE_SHIFT) << 8) +
								(((engine.activePalette[i * 4 + 1]/4) >> PALETTE_SHIFT) << 4) + ((engine.activePalette[i * 4 + 0]/4) >> PALETTE_SHIFT));
		}
		SetPalette();
		engine.logoState.fadeOutStatus[2] = 1;
	}
	if(engine.logoState.screenDuration < 100000 && !engine.logoState.fadeOutStatus[3])
	{
		for (int i = 0; i < 16; i++)
		{
			engine.renderer.bitmapPalette[i] = 0;
		}
		SetPalette();
		engine.logoState.fadeOutStatus[3] = 1;
	}
}
