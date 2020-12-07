
#include "engine.h"
#include "platform.h"
#include "file_platform.h"
#include "bitmap.c"

#include "raycasting.c"
//#include "mipmaps.c"
#include "file_read.c"
#include "physics.c"
#include "dithering.c"
#include "drawing.c"
#include "map_streaming.c"
#include "setup_maps.c"
#include "rendering_quality.c"
#include "logo_state.c"
#include "menu_state.c"
#include "game_state.c"
#include "cutscene.c"


/*
docker run --rm \
-v ${PWD}:/work \
-v /path/to/extra/m68k-amigaos/lib:/tools/usr/lib \
-v /path/to/extra/m68k-amigaos/include:/tools/usr/include \
-it amigadev/crosstools:m68k-amigaos bash
*/

#ifdef AMIGA
void InitializeGameChipMemory()
{
	UBYTE fileName []= DATA_DIR(verge.mod); 
	
	
    File file = OpenFile(fileName);


    ULONG fileSize = FileGetSize(fileName);
	
	NewChipArena(&engine.chipArena, fileSize);


	engine.music = (UBYTE *)AllocateFromArena(&engine.chipArena, fileSize);
	
    ReadFile(file, engine.music, fileSize);
    
	CloseFile(file);

	
}
#endif


//****************************** CREATE
void InitEngine(void)
{
	engine.exitFlag = 0;
	engine.gamePaused = 0;

	UWORD i;
	for(i = 0; i < 256; ++i)
	{
		UBYTE j = i;
		flippedBytes[i] = (((j&1) << 7)|((j&2) << 5)|((j&4) << 3)|((j&8) << 1)|((j&16) >> 1)|((j&32) >> 3)|((j&64) >> 5)|((j&128) >> 7));
	}

	engine.musicOn = 0;
	#ifdef AMIGA
	InitializeGameChipMemory();
	#endif
	InitScreen();

	NewArena(&engine.temporaryArena, 109*1024);

	engine.paletteBitmap = LoadBitmapFile(DATA_DIR(plt), &engine.paletteHeader, engine.activePalette, 1, 0);
	

	//process paletter from an image
	SetBitmapPalette(engine.activePalette);

	InitInput();
	

    ULONG fontSize = 4096;//FileGetSize("\\data\\ss.fnt") + sizeof(Font);
	
    ULONG screenPlaneSize = PLANEWIDTHWORD*PLANEHEIGHT*sizeof(UWORD);

	#ifdef AMIGA
	NewArena(&engine.fontArena, fontSize + 4*screenPlaneSize);
	#else
	NewArena(&engine.fontArena, fontSize);
	#endif

	//TODO: This won't work on Amiga, make file directories platform-agnostic!
	engine.font = InitFont(DATA_DIR(ss.fnt));

	#ifdef AMIGA
	engine.renderer.plane1W = (UWORD *)AllocateFromArena(&engine.fontArena, screenPlaneSize);
	engine.renderer.plane2W = (UWORD *)AllocateFromArena(&engine.fontArena, screenPlaneSize);
	engine.renderer.plane3W = (UWORD *)AllocateFromArena(&engine.fontArena, screenPlaneSize);
	engine.renderer.plane4W = (UWORD *)AllocateFromArena(&engine.fontArena, screenPlaneSize);
	#endif

	MakeArenasForMaps(MAPSIZE*MAPSIZE, MAPLENGTH);	
	SetupMaps();


	// Load font




	engine.cutsceneDuration = 0;
	ResetTime();


	UnuseSystem();


	ClearBuffor();
	// //*********************************** SELECT HARDWARE ***********************************************
	engine.informationText = CreateBitmapFromText(engine.font, 
	"KEY 1 = 1 MB RAM   KEY 2 = MORE THAN 1 MB RAM"
	);
	
	engine.versionText = CreateBitmapFromText(engine.font, 
	"VERSION 1.03"
	);
	
	engine.renderer.ditherTable1 = 0;

	while(engine.renderer.ditherTable1 == 0)
	{
		DrawTextBitmap(engine.informationText, 50, PLANEHEIGHT/2, 13);
		DrawTextBitmap(engine.versionText, 0, PLANEHEIGHT - 6, 13);
		

		VSyncAndDraw();
		ClearBuffor();

		GetPlayerMemorySetting();
		
	}

	
	FillTextBitmap(engine.font, engine.informationText, 
	"KEY 3 = 14 MHz   KEY 4 = 25 MHz"
	);

	while(engine.renderer.renderingType == 0)
	{
		DrawTextBitmap(engine.informationText, 82, PLANEHEIGHT/2, 13);
	
		DrawTextBitmap(engine.versionText, 0, PLANEHEIGHT - 6, 13);
	
		VSyncAndDraw();
		ClearBuffor();

		GetPlayerRendererSetting();
		
	}
	ClearBuffor();
	

	
	FillTextBitmap(engine.font, engine.informationText, 
	"INVERT Y AXIS?   KEY 9 = YES   KEY 0 = NO"
	);

	engine.yAxis = 0;

	while(engine.yAxis == 0)
	{
		DrawTextBitmap(engine.informationText, 50, PLANEHEIGHT/2, 13);
		DrawTextBitmap(engine.versionText, 0, PLANEHEIGHT - 6, 13);
		

		VSyncAndDraw();
		ClearBuffor();

		GetYAxisSetting();
		
	}

	ClearArena(&engine.temporaryArena);
	InitLogoState();

	



}

UBYTE pauseHeld = 0;

//****************************** LOOP
void EngineLoop(void)
{


	ProcessJoystick();
	
	TimeStep();

	if(!engine.gamePaused)
	{
		if (engine.currentState == State_Logo)   //turned off
		{
			RunLogoState();
			
			if (getKey(ESCAPE))
			{
				engine.exitFlag = 1;
				ExitGame();
			}
				
		}
		else if(engine.currentState == State_Menu)
		{
			RunMenuState();
		}
		else if(engine.currentState == State_Game)
		{
				
			RunGameState();
		}
	}

	if (getKey(ESCAPE) || engine.exitFlag)
	{
		ExitGame();
		engine.exitFlag = 1;
	}

	if (getKey(P) && !pauseHeld)
	{
		
		pauseHeld = 1;
		engine.gamePaused = !engine.gamePaused;
	}
	else if(!getKey(P))
	{
		pauseHeld = 0;
	}

	ProcessSettingsInput();
	

	engine.loopEndTime = getCurrentTime();
}

//****************************** DESTROY
void EngineDestroy(void)
{
	UseSystem();
	CloseJoystick();
	ViewOff();
	FreeView();
	StopSample(0);
	DestroyAudio();

	DestroyArena(&engine.chipArena);
	DestroyArena(&engine.fontArena);
	DestroyArena(&engine.rendererArena);
	DestroyArena(&engine.temporaryArena);
	DestroyArena(&engine.firstMapArena);
	DestroyArena(&engine.secondMapArena);

}



	
	