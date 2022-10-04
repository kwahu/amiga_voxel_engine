
#include "memory_platform.h"
#include "platform.h"
#include "engine.h"
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
docker run --rm -v ${PWD}:/work -v /path/to/extra/m68k-amigaos/lib:/tools/usr/lib -v /path/to/extra/m68k-amigaos/include:/tools/usr/include -it amigadev/crosstools:m68k-amigaos bash
make -f makefile_amiga
*/

void InitializeGameChipMemory()
{

    FILE *file = OpenFile("data/verge.mod", "rb");

    ULONG fileSize = GetFileSize("data/verge.mod");

	NewChipArena(&engine.chipArena, fileSize);


	engine.music = (UBYTE *)AllocateFromArena(&engine.chipArena, fileSize);

    ReadFile(file, engine.music, fileSize);
    CloseFile(file);



}


//****************************** CREATE
void InitEngine(void)
{
	engine.exitFlag = 0;

	engine.musicOn = 0;

	engine.gameState.resourcesLoaded = 0;
	
	//InitializeGameChipMemory();
	InitScreen();
	InitInput();
	



    ULONG fontSize = GetFileSize("data/ss.fnt") + sizeof(Font);

    ULONG screenPlaneSize = PLANEWIDTHWORD*PLANEHEIGHT*sizeof(UWORD);


	NewArena(&engine.fontArena, fontSize + 4*screenPlaneSize);


	engine.font = InitFont("data/ss.fnt");

	engine.renderer.plane1W = (UWORD *)AllocateFromArena(&engine.fontArena, screenPlaneSize);
	engine.renderer.plane2W = (UWORD *)AllocateFromArena(&engine.fontArena, screenPlaneSize);
	engine.renderer.plane3W = (UWORD *)AllocateFromArena(&engine.fontArena, screenPlaneSize);
	engine.renderer.plane4W = (UWORD *)AllocateFromArena(&engine.fontArena, screenPlaneSize);



	NewArena(&engine.temporaryArena, 32*1024);

	MakeArenasForMaps(MAPSIZE*MAPSIZE, 11);

	SetupMaps();

	engine.paletteBitmap = LoadBitmapFile("data/plt", &engine.paletteHeader, engine.activePalette, 1, 0);

	//process paletter from an image
	SetBitmapPalette(engine.activePalette);
	// Load font



	engine.cutsceneDuration = 0;
	ResetTime();


	UnuseSystem();

	ClearBuffor();

	// //*********************************** SELECT HARDWARE ***********************************************
	engine.informationText = CreateBitmapFromText(engine.font,
	"KEY 1 = 1 MB RAM   KEY 2 = MORE THAN 1 MB RAM"
	);
	DrawTextBitmap(engine.informationText, 50, PLANEHEIGHT/2, 3);

	VSyncAndDraw();

	engine.renderer.ditherTable1 = 0;

	while(engine.renderer.ditherTable1 == 0)
	{
		GetPlayerMemorySetting();

	}




	ClearBuffor();

	FillTextBitmap(engine.font, engine.informationText,
	#ifdef AMIGA
	"KEY 3 = A500   KEY 4 = A1200   KEY 5 = A3000"
	#else
	"KEY 3 = ATARI ST   KEY 4 = ATARI FALCON OR TT"
	#endif
	);
	DrawTextBitmap(engine.informationText, 50, PLANEHEIGHT/2, 3);
	VSyncAndDraw();

	while(engine.renderer.renderingType == 0)
	{
		GetPlayerRendererSetting();

	}

	ClearBuffor();

	FillTextBitmap(engine.font, engine.informationText,
	"INVERT Y AXIS?   KEY 9 = YES   KEY 0 = NO"
	);
	DrawTextBitmap(engine.informationText, 50, PLANEHEIGHT/2, 3);

	VSyncAndDraw();

	engine.yAxis = 0;

	while(engine.yAxis == 0)
	{
		GetYAxisSetting();

	}

	ClearArena(&engine.temporaryArena);
	//InitLogoState();

	InitGameState();



}

//****************************** LOOP
void EngineLoop(void)
{


	ProcessJoystick();

  if(getJoy(1, FIRE))
  {
    engine.platformScreen.vPort->palette[0] = 0x0fff;

  }
  else
  {
    engine.platformScreen.vPort->palette[0] = 0x0000;

  }


  LoadGameView(engine.platformScreen.view);

	TimeStep();
	if (engine.currentState == State_Logo)   //turned off
	{
		RunLogoState();

		if (getKey(ESCAPE))
		{
			exitFlag = 1;
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

	if (getKey(ESCAPE) || exitFlag)
	{
		ExitGame();
		exitFlag = 1;
	}


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
	DestroyArena(&engine.firstMapArena);
	if(engine.secondMapArena.Memory != 0)
	{
		DestroyArena(&engine.secondMapArena);
	}
	DestroyArena(&engine.temporaryArena);
	DestroyArena(&engine.rendererArena);

}
