#pragma once

#ifdef AMIGA

#include <clib/graphics_protos.h> // BitMap etc


#define USEA 0x800
#define USEB 0x400
#define USEC 0x200
#define USED 0x100

#define MINTERM_A 0xF0
#define MINTERM_B 0xCC
#define MINTERM_A_OR_C 0xFA
#define MINTERM_COOKIE 0xCA
#define MINTERM_COPY 0xC0

typedef struct BitMap BitMap;


UBYTE BlitIsIdle(void) {
	#ifdef AMIGA
	if(!(customRegister->dmaconr & DMAF_BLTDONE)) {
		if(!(customRegister->dmaconr & DMAF_BLTDONE)) {
			return 1;
		}
	}
	return 0;
	#else
		return 1;
	#endif // AMIGA
}

void BlitWait(void) {
	SystemSetDma(DMAB_BLITHOG, 1);
	while(!BlitIsIdle()) {}
	SystemSetDma(DMAB_BLITHOG, 0);
}

void CopyFastToChipW(BitMap *bm)
{
	UWORD blitWords, bltCon0;
	WORD dstModulo, srcModulo;
	ULONG srcOffs, dstOffs;

	blitWords = 320 >> 4;
	bltCon0 = USEA|USED | MINTERM_A;

	srcModulo = bm->BytesPerRow - (blitWords<<1);
	dstModulo = engine.renderer.buffer.BytesPerRow - (blitWords<<1);
	srcOffs = 0;
	dstOffs = 0;

	UBYTE plane;


	blitWait();
	customRegister->bltcon0 = bltCon0;
	customRegister->bltcon1 = 0;
	customRegister->bltafwm = 0xFFFF;
	customRegister->bltalwm = 0xFFFF;

	customRegister->bltamod = srcModulo;
	customRegister->bltdmod = dstModulo;
	for(plane = bm->Depth; plane--;) {
		blitWait();
		// This hell of a casting must stay here or else large offsets get bugged!
		customRegister->bltapt = (UBYTE*)(((ULONG)(bm->Planes[plane])) + srcOffs);
		customRegister->bltdpt = (UBYTE*)(((ULONG)((&engine.renderer.buffer)->Planes[plane])) + dstOffs);
		customRegister->bltsize = (PLANEHEIGHT << 6) | blitWords;
	}
	// blitUnsafeCopyAligned(&engine.renderer.buffer, 0, 0,
	// bm, 0, 0, 320, PLANEHEIGHT);
}

#define VSyncAndDraw() \
    vPortWaitForEnd(engine.platformScreen.vPort);	\
    DrawPanelsToScreen();

#define VSyncWait() \
    vPortWaitForEnd(engine.platformScreen.vPort);

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
		engine.renderer.bitmapPalette[i] = ((engine.activePalette[i * 4 + 2] >> 4) << 8) +	
						   ((engine.activePalette[i * 4 + 1] >> 4) << 4) + (engine.activePalette[i * 4 + 0] >> 4);	
	}	
	SetPalette();
}


struct VPort;
struct ScreenView;
struct SimpleBufferManager;
struct CopList;

typedef struct ViewPortManager {
	struct ViewPortManager *next;                      ///< Pointer to next manager.
	void  (*process)(struct ViewPortManager *manager); ///< Process fn handle.
	void  (*destroy)(struct ViewPortManager *manager); ///< Destroy fn handle.
	struct VPort *vPort;                         ///< Quick ref to VPort.
	UBYTE id;                                     ///< Manager ID.
} ViewPortManager;

typedef struct ScreenView {
	UBYTE vpCount;             ///< Viewport count.
	UWORD flags;               ///< Creation flags.
	struct CopList *copList;  ///< Pointer to copperlist.
	struct VPort *firstVPort; ///< Pointer to first VPort on list.
} ScreenView;

typedef struct VPort {
	// Main
	ScreenView *view;              ///< Pointer to parent tView.
	struct VPort *next;     ///< Pointer to next tVPort.
	ViewPortManager *firstManager; ///< Pointer to first viewport manager on list.
	UWORD flags;             ///< Creation flags.

	// VPort dimensions
	UWORD offsetX;  ///< Viewport's X offset on view.
	UWORD offsetY;  ///< Viewport's Y offset on view.
	UWORD width;  ///< Viewport's width
	UWORD height; ///< Viewport's height

	// Color info
	UBYTE BPP;        ///< Bitplane count
	UWORD palette[32]; ///< Destination palette
} VPort;

typedef struct SimpleBufferManager{
	ViewPortManager common;
	BitMap *front;       ///< Currently displayed buffer.
	BitMap *back;        ///< Buffer for drawing.
	UBYTE flags;         ///< Read only. See SIMPLEBUFFER_FLAG_*.
	UWORD copperOffset;  ///< Offset on copperlist in COP_RAW mode.
} SimpleBufferManager;


void DrawPanelsToScreen()
{
	CopyFastToChipW(engine.platformScreen.buffer->back);
}

void SetPalette()	
{							
	memcpy(engine.platformScreen.vPort->palette, engine.renderer.bitmapPalette, 16 * sizeof(UWORD));	
	viewLoad(engine.platformScreen.view);
}	
ScreenView * CreateView(void *tags, ...)
{
	ScreenView *view = memAllocFastClear(sizeof(ScreenView));

	va_list vaTags;
	va_start(vaTags, tags);

	// Process copperlist raw/block tags
	if(
		tagGet(tags, vaTags, TAG_VIEW_COPLIST_MODE, VIEW_COPLIST_MODE_BLOCK)
		== VIEW_COPLIST_MODE_RAW
	) {
		ULONG copListSize = tagGet(tags, vaTags, TAG_VIEW_COPLIST_RAW_COUNT, -1);
		view->copList = copListCreate(0,
			TAG_COPPER_LIST_MODE, COPPER_MODE_RAW,
			TAG_COPPER_RAW_COUNT, copListSize,
			TAG_DONE
		);
		view->flags |= VIEW_FLAG_COPLIST_RAW;
	}
	else {
		view->copList = copListCreate(0, TAG_DONE);
	}

	// Additional CLUT tags
	if(tagGet(tags, vaTags, TAG_VIEW_GLOBAL_CLUT, 0)) {
		view->flags |= VIEW_FLAG_GLOBAL_CLUT;
	}

	va_end(vaTags);
	return view;

}

VPort *CreateVPort(void *tagList, ...) {
	va_list vaTags;
	va_start(vaTags, tagList);
#ifdef AMIGA

	VPort *vPort = memAllocFastClear(sizeof(VPort));

	const UWORD defaultWidth = 320;
	const UWORD defaultHeight = -1;
	const UWORD defaultBpp = 4; // 'Cuz copper is slower @ 5bpp & more in OCS

	// Determine parent view
	ScreenView *view = (ScreenView*)tagGet(tagList, vaTags, TAG_VPORT_VIEW, 0);

	vPort->view = view;

	// Calculate Y offset - beneath previous ViewPort
	vPort->offsetY = 0;
	VPort *prevVPort = view->firstVPort;
	while(prevVPort) {
		vPort->offsetY += prevVPort->height;
		prevVPort = prevVPort->next;
	}
	if(vPort->offsetY && !(view->flags & VIEW_FLAG_GLOBAL_CLUT)) {
		vPort->offsetY += 2; // TODO: not always required?
	}
	ULONG addOffsY = tagGet(tagList, vaTags, TAG_VPORT_OFFSET_TOP, 0);
	vPort->offsetY += addOffsY;
	
	// Get dimensions
	vPort->width = tagGet(tagList, vaTags, TAG_VPORT_WIDTH, defaultWidth);
	vPort->height = tagGet(tagList, vaTags, TAG_VPORT_HEIGHT, defaultHeight);
	if(vPort->height == defaultHeight) {
		vPort->height = 256-vPort->offsetY;
	}
	vPort->BPP = tagGet(tagList, vaTags, TAG_VPORT_BPP, defaultBpp);


	// Update view - add to vPort list
	++view->vpCount;
	if(!view->firstVPort) {
		view->firstVPort = vPort;
	}
	else {
		prevVPort = view->firstVPort;
		while(prevVPort->next) {
			prevVPort = prevVPort->next;
		}
		prevVPort->next = vPort;
	}

	// Palette tag
	UWORD *srcPalette = (UWORD*)tagGet(tagList, vaTags, TAG_VPORT_PALETTE_PTR, 0);
	if(srcPalette) {
		UWORD paletteSize = tagGet(tagList, vaTags, TAG_VPORT_PALETTE_SIZE, 0xFFFF);
		if(paletteSize == 0xFFFF) {
		}
		else if(!paletteSize || paletteSize > 32) {
		}
		else {
			memcpy(vPort->palette, srcPalette, paletteSize * sizeof(UWORD));
		}
	}

	va_end(vaTags);
	return vPort;
#endif // AMIGA
}



void InitScreen()
{
	engine.platformScreen.view = CreateView(0,
						 TAG_VIEW_GLOBAL_CLUT, 1,
						 TAG_DONE);

	engine.platformScreen.vPort = CreateVPort(0,
						   TAG_VPORT_VIEW, engine.platformScreen.view,
						   TAG_VPORT_BPP, DEPTH,
						   TAG_END);

	engine.platformScreen.buffer = simpleBufferCreate(0,
								   TAG_SIMPLEBUFFER_VPORT, engine.platformScreen.vPort,
								   TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
								   TAG_DONE);

	
	engine.renderer.buffer.pad = engine.platformScreen.buffer->back->pad;
	engine.renderer.buffer.Flags = engine.platformScreen.buffer->back->Flags;
	engine.renderer.buffer.Depth = 4;
	engine.renderer.buffer.BytesPerRow = engine.platformScreen.buffer->back->BytesPerRow;
	engine.renderer.buffer.Planes[0] = engine.renderer.plane1W; 
	engine.renderer.buffer.Planes[1] = engine.renderer.plane2W;
	engine.renderer.buffer.Planes[2] = engine.renderer.plane3W;
	engine.renderer.buffer.Planes[3] = engine.renderer.plane4W;
	engine.renderer.buffer.Rows = PLANEHEIGHT;
}

void ViewOff()
{
	
	blitManagerDestroy();
	viewLoad(0);
}

void FreeView()
{
	
	viewDestroy(engine.platformScreen.view);
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