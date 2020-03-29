#pragma once

#ifdef AMIGA

#include <ace/managers/blit.h>

void CopyFastToChipW(tBitMap *bm)
{
	
	blitUnsafeCopyAligned(&engine.renderer.buffer, 0, 0,
	bm, 0, 0, 320, PLANEHEIGHT);
	
	// CopyMemQuick(engine.renderer.plane1W, bm->Planes[0], PLANEWIDTH*PLANEHEIGHT);
	// CopyMemQuick(engine.renderer.plane2W, bm->Planes[1], PLANEWIDTH*PLANEHEIGHT);
	// CopyMemQuick(engine.renderer.plane3W, bm->Planes[2], PLANEWIDTH*PLANEHEIGHT);
	// CopyMemQuick(engine.renderer.plane4W, bm->Planes[3], PLANEWIDTH*PLANEHEIGHT);
}

void DrawPanelsToScreen()
{
	CopyFastToChipW(engine.platformScreen.s_pBuffer->pBack);
}

#define VSyncAndDraw() \
    vPortWaitForEnd(engine.platformScreen.s_pVPort);	\
    DrawPanelsToScreen();

#define VSyncWait() \
    vPortWaitForEnd(engine.platformScreen.s_pVPort);

void SetBitmapPalette(UBYTE *Palette)	
{				
	for (int i = 0; i < 16; i++)	
	{	
		engine.renderer.bitmapPalette[i] = ((Palette[i * 4 + 2] >> 4) << 8) +	
							((Palette[i * 4 + 1] >> 4) << 4) + (Palette[i * 4 + 0] >> 4);	
	}	
	SetPalette();
}

void SetGamePaletter()	
{								
	//process paletter from an image	
	for (int i = 0; i < 16; i++)	
	{	
		engine.renderer.bitmapPalette[i] = ((engine.palettePalette[i * 4 + 2] >> 4) << 8) +	
						   ((engine.palettePalette[i * 4 + 1] >> 4) << 4) + (engine.palettePalette[i * 4 + 0] >> 4);	
	}	
	SetPalette();
}

void SetPalette()	
{							
	memcpy(engine.platformScreen.s_pVPort->pPalette, engine.renderer.bitmapPalette, 16 * sizeof(UWORD));	
	viewLoad(engine.platformScreen.s_pView);
}	


void InitScreen()
{
	engine.platformScreen.s_pView = viewCreate(0,
						 TAG_VIEW_GLOBAL_CLUT, 1,
						 TAG_DONE);

	engine.platformScreen.s_pVPort = vPortCreate(0,
						   TAG_VPORT_VIEW, engine.platformScreen.s_pView,
						   TAG_VPORT_BPP, DEPTH,
						   TAG_END);

	engine.platformScreen.s_pBuffer = simpleBufferCreate(0,
								   TAG_SIMPLEBUFFER_VPORT, engine.platformScreen.s_pVPort,
								   TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
								   TAG_DONE);

	engine.renderer.plane1W = planesWc;
	engine.renderer.plane2W = planesWc + PLANEWIDTHWORD*PLANEHEIGHT;
	engine.renderer.plane3W = planesWc + 2*PLANEWIDTHWORD*PLANEHEIGHT;
	engine.renderer.plane4W = planesWc + 3*PLANEWIDTHWORD*PLANEHEIGHT;   
	
	engine.renderer.buffer.pad = engine.platformScreen.s_pBuffer->pBack->pad;
	engine.renderer.buffer.Flags = engine.platformScreen.s_pBuffer->pBack->Flags;
	engine.renderer.buffer.Depth = 4;
	engine.renderer.buffer.BytesPerRow = engine.platformScreen.s_pBuffer->pBack->BytesPerRow;
	engine.renderer.buffer.Planes[0] = engine.renderer.plane1W; 
	engine.renderer.buffer.Planes[1] = engine.renderer.plane2W;
	engine.renderer.buffer.Planes[2] = engine.renderer.plane3W;
	engine.renderer.buffer.Planes[3] = engine.renderer.plane4W;
	engine.renderer.buffer.Rows = PLANEHEIGHT;
	blitManagerCreate();
}

void ViewOff()
{
	
	blitManagerDestroy();
	viewLoad(0);
}

void FreeView()
{
	
	viewDestroy(engine.platformScreen.s_pView);
}


#else


void DrawPanelsToScreen()
{
	
    // uint16_t *tmp;
    // tmp=engine.platformScreen.physBase;
    // engine.platformScreen.physBase=engine.platformScreen.logBase;
    // engine.platformScreen.logBase=tmp;
	// engine.renderer.planes = engine.platformScreen.physBase;
    //Setscreen(engine.platformScreen.logBase,engine.platformScreen.physBase,-1);
	//memcpy(engine.platformScreen.planesAtari, engine.renderer.planes, PLANEWIDTH*PLANEHEIGHT);
}

#define VSyncAndDraw() \
	Vsync();	\
	DrawPanelsToScreen();

#define VSyncWait() \
	Vsync();	



void SetBitmapPalette(UBYTE *Palette)	
{			
	for (int i = 0; i < 16; i++)	
	{	
		engine.renderer.bitmapPalette[i] = ((Palette[i * 4 + 2] >> 5) << 8) +	
							((Palette[i * 4 + 1] >> 5) << 4) + (Palette[i * 4 + 0] >> 5);	
	}	
			
	SetPalette();
}	

void SetGamePaletter()
{
	for (int i = 0; i < 16; i++)	
	{	
		engine.renderer.bitmapPalette[i] = ((engine.palettePalette[i*4+2]>>5) << 8) +	
		 ((engine.palettePalette[i*4+1]>>5) << 4) + (engine.palettePalette[i*4+0]>>5);	
	}	
	SetPalette();	
}

void SetPalette()	
{			
	Setpalette(engine.renderer.bitmapPalette);	
}	

void InitScreen()
{
	Cursconf(0, 0);
    engine.platformScreen.physBase=Physbase();
    engine.platformScreen.logBase=Logbase();
    memset(engine.platformScreen.logBase,0,32000);
    memset(engine.platformScreen.physBase,0,32000);
    VsetMode(0x80|2|0x30);
	VsetSync(1);
	
	for(int i=0;i<16;i++)
	{
		engine.platformScreen.systemPalette[i] = *(uint16_t *)(0xffff8240+(i*2));
	}
	SetPalette();

	//engine.platformScreen.planesAtari = engine.platformScreen.logBase;
	engine.renderer.planes = (uint16_t *)engine.platformScreen.physBase;
}

void ViewOff()
{

	Setpalette(engine.platformScreen.systemPalette);	
	Cursconf(1, 0);
}
void FreeView()
{
}



#endif