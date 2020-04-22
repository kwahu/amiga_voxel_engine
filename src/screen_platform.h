#pragma once

#ifdef AMIGA


#define USEA 0x800
#define USEB 0x400
#define USEC 0x200
#define USED 0x100

#define MINTERM_A 0xF0
#define MINTERM_B 0xCC
#define MINTERM_A_OR_C 0xFA
#define MINTERM_COOKIE 0xCA
#define MINTERM_COPY 0xC0



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

#define BMF_FASTMEM 1 << 5

UBYTE BitmapIsInterleaved(const BitMap *bitMap) {
	return (
		bitMap->Depth > 1 &&
		bitMap->Depth * ((ULONG)bitMap->Planes[1] - (ULONG)bitMap->Planes[0])
			== bitMap->BytesPerRow
	);
}


BitMap *BitmapCreate(
	UWORD width, UWORD height, UBYTE depth, UBYTE flags
) {
	BitMap *bitMap;
	UBYTE i;

	bitMap = (BitMap*) MemAllocFastClear(sizeof(BitMap));

	InitBitMap(bitMap, depth, width, height);

	if(flags & BMF_INTERLEAVED) {
		UWORD realWidth;
		realWidth = bitMap->BytesPerRow;
		bitMap->BytesPerRow *= depth;

		bitMap->Planes[0] = (PLANEPTR) MemAlloc(
			bitMap->BytesPerRow*height,
			(flags & BMF_FASTMEM) ? MEMF_ANY : MEMF_CHIP
		);
		if(!bitMap->Planes[0]) {
			MemFree(bitMap, sizeof(BitMap));
			return 0;
		}
		for(i = 1; i != depth; ++i) {
			bitMap->Planes[i] = bitMap->Planes[i-1] + realWidth;
		}

		if (flags & BMF_CLEAR) {
			memset(bitMap->Planes[0], 0, bitMap->Rows * bitMap->BytesPerRow);
		}
	}
	else {
		for(i = depth; i--;) {
			bitMap->Planes[i] = (PLANEPTR) MemAllocChip(bitMap->BytesPerRow*height);
			if(!bitMap->Planes[i]) {
				while(++i != depth) {
					MemFree(bitMap->Planes[i], bitMap->BytesPerRow*height);
				}
				MemFree(bitMap, sizeof(BitMap));
				return 0;
			}
			if (flags & BMF_CLEAR) {
				memset(bitMap->Planes[i], 0, bitMap->Rows * bitMap->BytesPerRow);
			}
		}
	}

	if (flags & BMF_CLEAR) {
		BlitWait();
	}

	return bitMap;
}


void BitmapDestroy(BitMap *bitMap) {
	if(bitMap) {
#ifdef AMIGA
		BlitWait();
#endif
		if(BitmapIsInterleaved(bitMap)) {
			bitMap->Depth = 1;
		}
		for(UBYTE i = bitMap->Depth; i--;) {
			MemFree(bitMap->Planes[i], bitMap->BytesPerRow*bitMap->Rows);
		}
		MemFree(bitMap, sizeof(BitMap));
	}
}


UWORD BitmapGetByteWidth(const BitMap *bitMap) {
	if(BitmapIsInterleaved(bitMap)) {
		return ((ULONG)bitMap->Planes[1] - (ULONG)bitMap->Planes[0]);
	}
	return bitMap->BytesPerRow;
}




// Copperlist mode: raw/block
#define TAG_VIEW_COPLIST_MODE      (TAG_USER|1)
// If in raw mode, specify copperlist instruction count
#define TAG_VIEW_COPLIST_RAW_COUNT (TAG_USER|2)
// If set with non-zero variable, view will use global palette & ignore vPorts'
#define TAG_VIEW_GLOBAL_CLUT       (TAG_USER|3)

// Values for TAG_VIEW_COPLIST_MODE
#define VIEW_COPLIST_MODE_BLOCK COPPER_MODE_BLOCK
#define VIEW_COPLIST_MODE_RAW   COPPER_MODE_RAW

// Ptr to parent view
#define TAG_VPORT_VIEW         (TAG_USER|1)
// vPort dimensions, in pixels
#define TAG_VPORT_WIDTH        (TAG_USER|2)
#define TAG_VPORT_HEIGHT       (TAG_USER|3)
// vPort depth, best effects on OCS with 4 or less since copper is faster
#define TAG_VPORT_BPP          (TAG_USER|4)
// Pointer to palette to initialize vPort with and its size, in color count.
#define TAG_VPORT_PALETTE_PTR  (TAG_USER|5)
#define TAG_VPORT_PALETTE_SIZE (TAG_USER|6)
// Specify vertical offset from previous VPort
// TODO auto CopBlocks for disabling bitplane DMA
// When in raw mode, you have to disable DMA yourself, 'cuz making it work
// automatically would mean passing additional 2 offsets for WAIT/MOVEs for
// disabling/enabling DMA and then wasting all cycles between it for VPort
// manager stuff without letting you including custom instructions in spare
// time.
#define TAG_VPORT_OFFSET_TOP   (TAG_USER|7)


#define TAG_COPPER_LIST_MODE (TAG_USER|1)
#define TAG_COPPER_RAW_COUNT (TAG_USER|2)

// Values for TAG_COPPER_LIST_TYPE
#define COPPER_MODE_BLOCK 0
#define COPPER_MODE_RAW   1

#define STATUS_REALLOC_PREV 1
#define STATUS_REALLOC_CURR 2
#define STATUS_REALLOC (1|2)  /// Block added/removed - realloc merged memory

#define STATUS_UPDATE_PREV 4
#define STATUS_UPDATE_CURR 8
#define STATUS_UPDATE (4|8)   /// Blocks changed content
#define STATUS_REORDER 16     /// Blocks changed order

#define VPM_SCROLL       0
#define VPM_TILEBUFFER   1
#define VPM_DOUBLEBUFFER 2
#define VPM_CAMERA       128

// vPort ptr
#define TAG_SIMPLEBUFFER_VPORT          (TAG_USER|1)
// Scrollable area bounds, in pixels
#define TAG_SIMPLEBUFFER_BOUND_WIDTH    (TAG_USER|2)
#define TAG_SIMPLEBUFFER_BOUND_HEIGHT   (TAG_USER|3)
// Buffer bitmap creation flags
#define TAG_SIMPLEBUFFER_BITMAP_FLAGS   (TAG_USER|4)
#define TAG_SIMPLEBUFFER_IS_DBLBUF      (TAG_USER|5)
// If in raw mode, offset on copperlist for placing required copper
// instructions, specified in copper instruction count since beginning.
#define TAG_SIMPLEBUFFER_COPLIST_OFFSET (TAG_USER|6)


/* Types */

/**
 *  View flags.
 */
#define VIEW_FLAG_GLOBAL_CLUT 1
#define VIEW_FLAG_COPLIST_RAW 2

/**
 * Viewport flags.
 */
#define VIEWPORT_HAS_OWN_CLUT 1


void BlitManagerCreate(void) {
	SystemSetDma(DMAB_BLITTER, 1);
}

void BlitManagerDestroy(void) {
	SystemSetDma(DMAB_BLITTER, 0);
}

UBYTE BlitUnsafeCopyAligned(
	BitMap *src, WORD srcX, WORD srcY,
	BitMap *dst, WORD dstX, WORD dstY, WORD width, WORD height
) {
	#ifdef AMIGA
	UWORD blitWords, bltCon0;
	WORD dstModulo, srcModulo;
	ULONG srcOffs, dstOffs;

	blitWords = width >> 4;
	bltCon0 = USEA|USED | MINTERM_A;

	srcModulo = BitmapGetByteWidth(src) - (blitWords<<1);
	dstModulo = BitmapGetByteWidth(dst) - (blitWords<<1);
	srcOffs = src->BytesPerRow * srcY + (srcX>>3);
	dstOffs = dst->BytesPerRow * dstY + (dstX>>3);

	if(BitmapIsInterleaved(src) && BitmapIsInterleaved(dst)) {
		height *= src->Depth;
		BlitWait();
		customRegister->bltcon0 = bltCon0;
		customRegister->bltcon1 = 0;
		customRegister->bltafwm = 0xFFFF;
		customRegister->bltalwm = 0xFFFF;

		customRegister->bltamod = srcModulo;
		customRegister->bltdmod = dstModulo;

		// This hell of a casting must stay here or else large offsets get bugged!
		customRegister->bltapt = (UBYTE*)((ULONG)src->Planes[0] + srcOffs);
		customRegister->bltdpt = (UBYTE*)((ULONG)dst->Planes[0] + dstOffs);

		customRegister->bltsize = (height << 6) | blitWords;
	}
	else {
		UBYTE plane;

		if(BitmapIsInterleaved(src) || BitmapIsInterleaved(dst)) {
			}
		if(BitmapIsInterleaved(src)) {
			srcModulo += src->BytesPerRow * (src->Depth-1);
		}
		else if(BitmapIsInterleaved(dst)) {
			dstModulo += dst->BytesPerRow * (dst->Depth-1);
		}

		BlitWait();
		customRegister->bltcon0 = bltCon0;
		customRegister->bltcon1 = 0;
		customRegister->bltafwm = 0xFFFF;
		customRegister->bltalwm = 0xFFFF;

		customRegister->bltamod = srcModulo;
		customRegister->bltdmod = dstModulo;
		for(plane = src->Depth; plane--;) {
			BlitWait();
			// This hell of a casting must stay here or else large offsets get bugged!
			customRegister->bltapt = (UBYTE*)(((ULONG)(src->Planes[plane])) + srcOffs);
			customRegister->bltdpt = (UBYTE*)(((ULONG)(dst->Planes[plane])) + dstOffs);
			customRegister->bltsize = (height << 6) | blitWords;
		}
	}
#endif // AMIGA
	return 1;
}


void CopyFastToChipW(BitMap *bm)
{
	
	BlitUnsafeCopyAligned(&engine.renderer.buffer, 0, 0,
	bm, 0, 0, 320, PLANEHEIGHT);
}

#define VSyncAndDraw() \
    VPortWaitForEnd(engine.platformScreen.vPort);	\
    DrawPanelsToScreen();

#define VSyncWait() \
    VPortWaitForEnd(engine.platformScreen.vPort);

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
struct EngineCopList;
struct CopBlock;


typedef struct {
	// Higher word
	unsigned unused  :7;  /// Always set to 0
	unsigned destAddr:9;  /// Register offset from &custom segment
	                        /// LSBit must be set to 0 - WAIT check
	// Lower word
	unsigned value:16;    /// New value
} CopMoveCmd;

typedef struct {
	// Higher word
	unsigned waitY        :8; /// Y position
	unsigned waitX        :7; /// X position
	unsigned isWait       :1; /// Always set to 1
	// Lower word
	unsigned blitterIgnore:1; /// If set to 0, waits for pos and blit finish
	unsigned VE           :7; /// Y compare enable bits
	unsigned HE           :7; /// X compare enable bits
	unsigned isSkip       :1; /// Set to 1 for SKIP, 0 for WAIT
} CopWaitCmd;

typedef union {
	CopMoveCmd move;
	CopWaitCmd wait;
	ULONG code;
} CopCmd;
typedef struct {
	UWORD allocSize; /// Allocated memory size
	UWORD cmdCount;  /// Copper command count
	CopCmd *list;    /// HW Copperlist pointer
} CopBfr;

typedef struct CopBlock {
	struct CopBlock *next;
	UwCoordYX waitPos;         /// Wait pos YX
	UWORD maxCmds;             /// Command limit
	UWORD currCount;           /// Curr instruction count
	UBYTE disabled;            /// 1: disabled, 0: enabled
	UBYTE updated;             /// 2: curr update, 1: prev update, 0: no update
	UBYTE resized;             /// 2: curr size change, 1: prev size change, 0: no change
	CopCmd *cmds;              /// Command pointer
} CopBlock;


typedef struct EngineCopList {
	UWORD blockCount;     /// Total number of blocks
	UBYTE status;         /// Status flags for processing
	UBYTE mode;           /// Sets block/raw mode
	CopBfr *frontBfr;     /// Currently displayed copperlist
	CopBfr *backBfr;      /// Editable copperlist
	CopBlock *firstBlock; /// Block list
} EngineCopList;

typedef struct {
	EngineCopList *copList;   /// Currently displayed tCopList
	EngineCopList *blankList; /// Empty copperlist - LoadView(0) equivalent
} CopManager;

CopManager copManager;

EngineCopList *CopListCreate(void *tagList, ...);
void CopCreate(void) {
	
	// TODO: save previous copperlist

	// Create blank copperlist
	//copManager.blankList = CopListCreate(0, TAG_DONE);

	// Set both buffers to blank copperlist
	//copManager.copList = copManager.blankList;
	//CopProcessBlocks();
	//CopProcessBlocks();
	// Update copper-related regs
	customRegister->copjmp1 = 1;
	SystemSetDma(DMAB_COPPER, 1);

}


typedef struct VpManager {
	struct VpManager *next;                      ///< Pointer to next manager.
	void  (*process)(struct VpManager *manager); ///< Process fn handle.
	void  (*destroy)(struct VpManager *manager); ///< Destroy fn handle.
	struct VPort *vPort;                         ///< Quick ref to VPort.
	UBYTE id;                                     ///< Manager ID.
} VpManager;


typedef struct ScreenView {
	UBYTE vpCount;             ///< Viewport count.
	UWORD flags;               ///< Creation flags.
	struct EngineCopList *copList;  ///< Pointer to copperlist.
	struct VPort *firstVPort; ///< Pointer to first VPort on list.
} ScreenView;

typedef struct VPort {
	// Main
	ScreenView *view;              ///< Pointer to parent tView.
	struct VPort *next;     ///< Pointer to next tVPort.
	VpManager *firstManager; ///< Pointer to first viewport manager on list.
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


typedef void (*VpManagerFn)(VpManager *manager);


typedef struct {
	VpManager common;
	UwCoordYX pos;        ///< Current camera pos
	UwCoordYX lastPos[2]; ///< Previous camera pos
	UwCoordYX maxPos;     ///< Max camera pos: world W&H - camera W&H
	UBYTE bfr;
	UBYTE isDblBfr;
} CameraManager;


#define SIMPLEBUFFER_FLAG_X_SCROLLABLE 1
#define SIMPLEBUFFER_FLAG_COPLIST_RAW  2

typedef struct SimpleBufferManager{
	VpManager common;
	CameraManager *camera;
	BitMap *front;       ///< Currently displayed buffer.
	BitMap *back;        ///< Buffer for drawing.
	CopBlock *copBlock;  ///< CopBlock containing modulo/shift/bitplane cmds
	UwCoordYX bfrBounds; ///< Buffer bounds in pixels
	UBYTE flags;         ///< Read only. See SIMPLEBUFFER_FLAG_*.
	UWORD copperOffset;  ///< Offset on copperlist in COP_RAW mode.
} SimpleBufferManager;



typedef struct {
	UWORD hi; ///< upper WORD
	UWORD lo; ///< lower WORD
} CopperUlong;
CopperUlong FAR REGPTR bplFetch = (CopperUlong REGPTR)(
	CUSTOM_BASE + offsetof(Custom, bplpt)
);

void DrawPanelsToScreen()
{
	CopyFastToChipW(engine.platformScreen.buffer->back);
}

void SetPalette()	
{							
	memcpy(engine.platformScreen.vPort->palette, engine.renderer.bitmapPalette, 16 * sizeof(UWORD));	
	LoadGameView(engine.platformScreen.view);
}	



VPort *VPortCreate(void *tagList, ...) {
	va_list vaTags;
	va_start(vaTags, tagList);
#ifdef AMIGA

	VPort *vPort = MemAllocFastClear(sizeof(VPort));

	const UWORD defaultWidth = 320;
	const UWORD defaultHeight = -1;
	const UWORD defaultBpp = 4; // 'Cuz copper is slower @ 5bpp & more in OCS

	// Determine parent view
	ScreenView *view = (ScreenView*)GetTag(tagList, vaTags, TAG_VPORT_VIEW, 0);

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
	ULONG addOffsY = GetTag(tagList, vaTags, TAG_VPORT_OFFSET_TOP, 0);
	vPort->offsetY += addOffsY;
	
	// Get dimensions
	vPort->width = GetTag(tagList, vaTags, TAG_VPORT_WIDTH, defaultWidth);
	vPort->height = GetTag(tagList, vaTags, TAG_VPORT_HEIGHT, defaultHeight);
	if(vPort->height == defaultHeight) {
		vPort->height = 256-vPort->offsetY;
	}
	vPort->BPP = GetTag(tagList, vaTags, TAG_VPORT_BPP, defaultBpp);


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
	UWORD *srcPalette = (UWORD*)GetTag(tagList, vaTags, TAG_VPORT_PALETTE_PTR, 0);
	if(srcPalette) {
		UWORD paletteSize = GetTag(tagList, vaTags, TAG_VPORT_PALETTE_SIZE, 0xFFFF);
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


UBYTE CopBfrRealloc(void) {
	EngineCopList *copList;
	CopBfr *backBfr;
	CopBlock *block;
	UBYTE newStatus;

	copList = copManager.copList;
	backBfr = copList->backBfr;

	// Free memory
	if(backBfr->allocSize) {
		MemFree(backBfr->list, backBfr->allocSize);
	}

	// Calculate new list size
	if(copList->status & STATUS_REALLOC_CURR) {

		backBfr->allocSize = 0;
		for(block = copList->firstBlock; block; block = block->next) {
			backBfr->allocSize += block->maxCmds;
		}
		backBfr->allocSize += copList->blockCount + 1; // all WAITs + double WAIT
		backBfr->allocSize *= sizeof(CopCmd);
		// Pass realloc to next buffer
		newStatus = STATUS_REALLOC_PREV;
	}
	else {
		// If realloc just propagates to next buffer, calculations aren't necessary
		backBfr->allocSize = copList->frontBfr->allocSize;
		newStatus = 0;
	}

	// Alloc memory
	backBfr->list = MemAllocChip(backBfr->allocSize);
	return newStatus;
}

void CopReorderBlocks(void) {
	EngineCopList *copList = copManager.copList;
	UBYTE done;
	do {
		done = 1;
		CopBlock *block = copList->firstBlock;
		CopBlock *prev = 0;
		while(block->next) {
			if(block->waitPos.yx > block->next->waitPos.yx) {
				if(!prev) {
					copList->firstBlock = block->next;
					block->next = copList->firstBlock->next;
					copList->firstBlock->next = block;
				}
				else {
					prev->next = block->next;
					block->next = prev->next->next;
					prev->next->next = block;
				}
				done = 0;
				break;
			}
			prev = block;
			block = block->next;
		}

	} while(!done);
}


UBYTE CopUpdateFromBlocks(void) {
	EngineCopList *copList;
	CopBlock *block;
	CopBfr *backBfr;
	UWORD listPos;
	UBYTE wasLimitY;

	copList = copManager.copList;
	backBfr = copList->backBfr;

	block = copList->firstBlock;
	listPos = 0;
	wasLimitY = 0;

	for(; block; block = block->next) {
		if(block->disabled) {
			continue;
		}
		if(block->resized) {
			--block->resized;
		}
		if(block->updated) {
			--block->updated;
		}

		// Update WAIT
		if(block->waitPos.y > 0xFF) {
			// FIXME: double WAIT only when previous line ended before some pos
			if(!wasLimitY) {
				CopSetWait((CopWaitCmd*)&backBfr->list[listPos], 0xDF, 0xFF);
				++listPos;
				wasLimitY = 1;
			}
			CopSetWait(
				(CopWaitCmd*)&backBfr->list[listPos],
				block->waitPos.x, block->waitPos.y & 0xFF
			);
			++listPos;
		}
		else {
			CopSetWait(
				(CopWaitCmd*)&backBfr->list[listPos],
				block->waitPos.x, block->waitPos.y
			);
			++listPos;
		}

		// Copy MOVEs
		CopyMem(block->cmds, &backBfr->list[listPos], block->currCount*sizeof(CopCmd));
		listPos += block->currCount;
	}

	// Add 0xFFFF terminator
	CopSetWait((CopWaitCmd*)&backBfr->list[listPos], 0xFF, 0xFF);
	++listPos;

 	copList->backBfr->cmdCount = listPos;

	if(copList->status & STATUS_UPDATE_CURR) {
		return STATUS_UPDATE_PREV;
	}
	return 0;
}

void CopProcessBlocks(void) {
	EngineCopList *copList = copManager.copList;
	if(copList->mode == COPPER_MODE_BLOCK) {
		UBYTE newStatus = 0;
		// Realloc buffer memeory
		if(copList->status & STATUS_REALLOC) {
			newStatus = CopBfrRealloc();
		}

		// Sort blocks if needed
		if(copList->status & STATUS_REORDER) {
			CopReorderBlocks();
		}

		// Update buffer data
		if(copList->status & STATUS_UPDATE) {
			newStatus |= CopUpdateFromBlocks();
		}
		// Update status code
		copList->status = newStatus;
	}

	// Swap copper buffers
	CopSwapBuffers();
}

void CopSwapBuffers(void) {
	CopBfr *tmp;
	EngineCopList *copList;

	copList = copManager.copList;
	customRegister->cop1lc = (ULONG)((void *)copList->backBfr->list);
	tmp = copList->frontBfr;
	copList->frontBfr = copList->backBfr;
	copList->backBfr = tmp;
}

void UpdateViewCLUT(ScreenView *view) {
	if(view->flags & VIEW_FLAG_GLOBAL_CLUT) {
		for(UBYTE i = 0; i < 32; ++i) {
			customRegister->color[i] = view->firstVPort->palette[i];
		}
	}
}

void LoadGameView(ScreenView *view) {
	while(rayPos->posY < 300) {}
	if(!view) {
		copManager.copList = copManager.blankList;
		customRegister->bplcon0 = 0; // No output
		customRegister->bplcon3 = 0; // AGA fix
		customRegister->fmode = 0;   // AGA fix
		for(UBYTE i = 0; i < 6; ++i) {
			customRegister->bplpt[i] = 0;
		}
		customRegister->bpl1mod = 0;
		customRegister->bpl2mod = 0;
	}
	else {
		copManager.copList = view->copList;
		customRegister->bplcon0 = (view->firstVPort->BPP << 12) | BV(9); // BPP + composite output
		customRegister->fmode = 0;        // AGA fix
		customRegister->bplcon3 = 0;      // AGA fix
		customRegister->diwstrt = 0x2C81; // VSTART: 0x2C, HSTART: 0x81
		customRegister->diwstop = 0x2CC1; // VSTOP: 0x2C, HSTOP: 0xC1
		UpdateViewCLUT(view);
	}
	CopProcessBlocks();
	customRegister->copjmp1 = 1;
	SystemSetDma(DMAB_RASTER, view != 0);
	while(rayPos->posY < 300) {}
}


void VPortWaitForEnd(VPort *vPort) {
	UWORD endPos;
	UWORD currFrame;

	// Determine VPort end position
	endPos = vPort->offsetY + vPort->height + 0x2C; // Addition from DiWStrt
	if(rayPos->posY < endPos) {
		// If current beam is before pos, wait for pos @ current frame
		while(rayPos->posY < endPos) {}
	}
	else {
		// Otherwise wait for pos @ next frame
		currFrame = TimerGet();
		while(
			rayPos->posY < endPos ||
			TimerGet() == currFrame
		) {}
	}
}


void VPortAddManager(VPort *vPort, VpManager *vpManager) {
	// podpiecie
	if(!vPort->firstManager) {
		vPort->firstManager = vpManager;
		return;
	}
	VpManager *vpCurr = vPort->firstManager;
	// przewin przed menedzer o wyzszym numerze niz wstawiany
	while(vpCurr->next && vpCurr->next->id <= vpManager->id) {
		if(vpCurr->id <= vpManager->id) {
			vpCurr = vpCurr->next;
		}
	}
	vpManager->next = vpCurr->next;
	vpCurr->next = vpManager;
}



void VPortRmManager(VPort *vPort, VpManager *vpManager) {
	if(!vPort->firstManager) {
		return;
	}
	if(vPort->firstManager == vpManager) {
		vPort->firstManager = vpManager->next;
		vpManager->destroy(vpManager);
		return;
	}
	VpManager *parent = vPort->firstManager;
	while(parent->next) {
		if(parent->next == vpManager) {
			parent->next = vpManager->next;
			vpManager->destroy(vpManager);
			return;
		}
	}
}


void VPortDestroy(VPort *vPort) {
	ScreenView *view;
	VPort *prevVPort, *currVPort;

	view = vPort->view;
	prevVPort = 0;
	currVPort = view->firstVPort;
	while(currVPort) {
		if(currVPort == vPort) {

			// Remove from list
			if(prevVPort) {
				prevVPort->next = currVPort->next;
			}
			else {
				view->firstVPort = currVPort->next;
			}
			--view->vpCount;

			// Destroy managers
			while(currVPort->firstManager) {
				VPortRmManager(currVPort, currVPort->firstManager);
			}
		
			// Free stuff
			MemFree(vPort, sizeof(VPort));
			break;
		}
		prevVPort = currVPort;
		currVPort = currVPort->next;
	}
}


CopBlock *CopBlockCreate(EngineCopList *copList, UWORD maxCmds, UWORD waitX, UWORD waitY) {
	CopBlock *block;

	block = MemAllocFastClear(sizeof(CopBlock));
	block->maxCmds = maxCmds; // MOVEs only
	block->cmds     = MemAllocFast(sizeof(CopCmd) * block->maxCmds);

	CopBlockWait(copList, block, waitX, waitY);

	if(!copList->firstBlock || block->waitPos.yx < copList->firstBlock->waitPos.yx) {
		block->next = copList->firstBlock;
		copList->firstBlock = block;
	}
	else {
		CopBlock *prev;

		prev = copList->firstBlock;
		while(prev->next && prev->next->waitPos.yx < block->waitPos.yx) {
			prev = prev->next;
		}
		block->next = prev->next;
		prev->next = block;
	}
	copList->status |= STATUS_REALLOC_CURR | STATUS_REORDER | STATUS_UPDATE_CURR;
	++copList->blockCount;

	return block;
}


void CopBlockDestroy(EngineCopList *copList, CopBlock *block) {

	// Remove from list
	if(block == copList->firstBlock) {
		copList->firstBlock = block->next;
	}
	else {
		CopBlock *curr;

		curr = copList->firstBlock;
		while(curr->next && curr->next != block) {
			curr = curr->next;
		}

		if(curr->next) {
			curr->next = block->next;
		}
	}

	// Free mem
	MemFree(block->cmds, sizeof(CopCmd)*block->maxCmds);
	MemFree(block, sizeof(CopBlock));

	copList->status |= STATUS_REALLOC_CURR;
	--copList->blockCount;

}


EngineCopList *CopListCreate(void *tagList, ...) {
	va_list vaTags;
	va_start(vaTags, tagList);
	EngineCopList *copList;
	// Create copperlist stub
	copList = MemAllocFastClear(sizeof(EngineCopList));
	copList->frontBfr = MemAllocFastClear(sizeof(CopBfr));
	copList->backBfr = MemAllocFastClear(sizeof(CopBfr));

	// Handle raw copperlist creation
	 copList->mode = GetTag(tagList, vaTags, TAG_COPPER_LIST_MODE, COPPER_MODE_BLOCK);
	if(copList->mode	== COPPER_MODE_RAW) {
		const ULONG invalidSize = 0xFFFFFFFFUL;
		ULONG listSize = GetTag(
			tagList, vaTags, TAG_COPPER_RAW_COUNT, invalidSize
		);
		// Front bfr
		copList->frontBfr->cmdCount = listSize+1;
		copList->frontBfr->allocSize = (listSize+1)*sizeof(CopCmd);
		copList->frontBfr->list = MemAllocChipClear(copList->frontBfr->allocSize);
		CopSetWait(&copList->frontBfr->list[listSize].wait, 0xFF, 0xFF);
		// Back bfr
		copList->backBfr->cmdCount = listSize+1;
		copList->backBfr->allocSize = (listSize+1)*sizeof(CopCmd);
		copList->backBfr->list = MemAllocChipClear(copList->backBfr->allocSize);
		CopSetWait(&copList->backBfr->list[listSize].wait, 0xFF, 0xFF);
	}

	va_end(vaTags);
	return copList;

}



void CopListDestroy(EngineCopList *copList) {

	// Free copperlist buffers
	while(copList->firstBlock) {
		CopBlockDestroy(copList, copList->firstBlock);
	}

	// Free front buffer
	if(copList->frontBfr->allocSize) {
		MemFree(copList->frontBfr->list, copList->frontBfr->allocSize);
	}
	MemFree(copList->frontBfr, sizeof(CopBfr));

	// Free back buffer
	if(copList->backBfr->allocSize) {
		MemFree(copList->backBfr->list, copList->backBfr->allocSize);
	}
	MemFree(copList->backBfr, sizeof(CopBfr));

	// Free main struct
	MemFree(copList, sizeof(EngineCopList));

}


void DestroyView(ScreenView *view) {
	if(copManager.copList == view->copList) {
		LoadGameView(0);
	}

	// Free all attached viewports
	while(view->firstVPort) {
		VPortDestroy(view->firstVPort);
	}

	CopListDestroy(view->copList);
	MemFree(view, sizeof(ScreenView));
}


void CopBlockWait(EngineCopList *copList, CopBlock *block, UWORD x, UWORD y) {
	block->waitPos.y  = y;
	block->waitPos.x  = x;

	block->updated = 2;
	copList->status |= STATUS_UPDATE | STATUS_REORDER;
}


void CopSetMove(CopMoveCmd *moveCmd, volatile void *addr, UWORD value) {
	moveCmd->unused = 0;
	moveCmd->destAddr = (ULONG)addr - (ULONG)((UBYTE *)customRegister);
	moveCmd->value = value;
}

void CopMove(EngineCopList *copList, CopBlock *block, volatile void *addr, UWORD value) {
	CopSetMove((CopMoveCmd*)&block->cmds[block->currCount], addr, value);
	++block->currCount;

	block->updated = 2;
	block->resized = 2;
	copList->status |= STATUS_UPDATE;
}


void CopSetWait(CopWaitCmd *waitCmd, UBYTE X, UBYTE Y) {
	waitCmd->waitY         = Y;
	waitCmd->waitX         = X >> 1;
	waitCmd->isWait        = 1;
	waitCmd->blitterIgnore = 1;
	waitCmd->VE            = 0x7F;
	waitCmd->HE            = 0x7F;
	waitCmd->isSkip        = 0;
}

EngineCopList *CreateCopList(void *tagList, ...) {
	va_list vaTags;
	va_start(vaTags, tagList);
	EngineCopList *copList;
	
	// Create copperlist stub
	copList = MemAllocFastClear(sizeof(EngineCopList));
	copList->frontBfr = MemAllocFastClear(sizeof(CopBfr));
	copList->backBfr = MemAllocFastClear(sizeof(CopBfr));

	// Handle raw copperlist creation
	copList->mode = GetTag(tagList, vaTags, TAG_COPPER_LIST_MODE, COPPER_MODE_BLOCK);
	if(copList->mode	== COPPER_MODE_RAW) {
		const ULONG invalidSize = 0xFFFFFFFFUL;
		ULONG listSize = GetTag(
			tagList, vaTags, TAG_COPPER_RAW_COUNT, invalidSize
		);
		// Front bfr
		copList->frontBfr->cmdCount = listSize+1;
		copList->frontBfr->allocSize = (listSize+1)*sizeof(CopCmd);
		copList->frontBfr->list = MemAllocChipClear(copList->frontBfr->allocSize);
		CopSetWait(&copList->frontBfr->list[listSize].wait, 0xFF, 0xFF);
		// Back bfr
		copList->backBfr->cmdCount = listSize+1;
		copList->backBfr->allocSize = (listSize+1)*sizeof(CopCmd);
		copList->backBfr->list = MemAllocChipClear(copList->backBfr->allocSize);
		CopSetWait(&copList->backBfr->list[listSize].wait, 0xFF, 0xFF);
	}
	
	va_end(vaTags);
	return copList;
}

ScreenView * ViewCreate(void *tags, ...)
{
	ScreenView *view = MemAllocFastClear(sizeof(ScreenView));

	va_list vaTags;
	va_start(vaTags, tags);

	// Process copperlist raw/block tags
	if(
		GetTag(tags, vaTags, TAG_VIEW_COPLIST_MODE, VIEW_COPLIST_MODE_BLOCK)
		== VIEW_COPLIST_MODE_RAW
	) {
		ULONG copListSize = GetTag(tags, vaTags, TAG_VIEW_COPLIST_RAW_COUNT, -1);
		view->copList = CreateCopList(0,
			TAG_COPPER_LIST_MODE, COPPER_MODE_RAW,
			TAG_COPPER_RAW_COUNT, copListSize,
			TAG_DONE
		);
		view->flags |= VIEW_FLAG_COPLIST_RAW;
	}
	else {
		view->copList = CreateCopList(0, TAG_DONE);
	}

	// Additional CLUT tags
	if(GetTag(tags, vaTags, TAG_VIEW_GLOBAL_CLUT, 0)) {
		view->flags |= VIEW_FLAG_GLOBAL_CLUT;
	}

	va_end(vaTags);
	return view;

}


void CameraDestroy(CameraManager *manager) {
	MemFree(manager, sizeof(CameraManager));
	
}

void CameraProcess(CameraManager *manager) {
	manager->lastPos[manager->bfr].yx = manager->pos.yx;
	if(manager->isDblBfr) {
		manager->bfr = !manager->bfr;
	}
}

void CameraReset(
	CameraManager *manager,
	UWORD startX, UWORD startY, UWORD width, UWORD height, UBYTE isDblBfr
) {
	manager->pos.x = startX;
	manager->pos.y = startY;
	manager->lastPos[0].x = startX;
	manager->lastPos[0].y = startY;
	manager->lastPos[1].x = startX;
	manager->lastPos[1].y = startY;
	manager->isDblBfr = isDblBfr;
	manager->bfr = 0;

	// Max camera coords based on viewport size
	manager->maxPos.x = width -manager->common.vPort->width;
	manager->maxPos.y = height - manager->common.vPort->height;
}


CameraManager *CameraCreate(
	VPort *vPort, UWORD posX, UWORD posY, UWORD maxX, UWORD maxY,
	UBYTE isDblBfr
) {
	CameraManager *manager;

	manager = MemAllocFastClear(sizeof(CameraManager));
	manager->common.process = (VpManagerFn)CameraProcess;
	manager->common.destroy = (VpManagerFn)CameraDestroy;
	manager->common.vPort = vPort;
	manager->common.id = VPM_CAMERA;

	CameraReset(manager, posX, posY, maxX, maxY, isDblBfr);

	VPortAddManager(vPort, (VpManager*)manager);
	return manager;
}


void BufferDestroy(SimpleBufferManager *manager) {
	CopBlockDestroy(manager->common.vPort->view->copList, manager->copBlock);
	if(manager->back != manager->front) {
		BitmapDestroy(manager->back);
	}
	BitmapDestroy(manager->front);
	MemFree(manager, sizeof(SimpleBufferManager));
}

void BufferProcess(SimpleBufferManager *manager) {
	UWORD shift;
	ULONG bplOffs;
	ULONG planeAddr;

	const CameraManager *camera = manager->camera;
	EngineCopList *copList = manager->common.vPort->view->copList;

	// Calculate X movement: bitplane shift, starting word to fetch
	if(manager->flags & SIMPLEBUFFER_FLAG_X_SCROLLABLE) {
		shift = (16 - (camera->pos.x & 0xF)) & 0xF;  // Bitplane shift - single
		shift = (shift << 4) | shift;                         // Bitplane shift - PF1 | PF2
		bplOffs = ((camera->pos.x - 1) >> 4) << 1; // Must be ULONG!
	}
	else {
		shift = 0;
		bplOffs = (camera->pos.x >> 4) << 1;
	}

	// Calculate Y movement
	bplOffs += manager->back->BytesPerRow * camera->pos.y;

	// Copperlist - regen bitplane ptrs, update shift
	// TODO could be unified by using copSetMove in copBlock
	if(manager->flags & SIMPLEBUFFER_FLAG_COPLIST_RAW) {
		CopCmd *cmdList = &copList->backBfr->list[manager->copperOffset];
		CopSetMove(&cmdList[5].move, &customRegister->bplcon1, shift);
		for(UBYTE i = 0; i < manager->common.vPort->BPP; ++i) {
			planeAddr = ((ULONG)manager->back->Planes[i]) + bplOffs;
			CopSetMove(&cmdList[6 + i*2 + 0].move, &bplFetch[i].hi, planeAddr >> 16);
			CopSetMove(&cmdList[6 + i*2 + 1].move, &bplFetch[i].lo, planeAddr & 0xFFFF);
		}
	}
	else {
		manager->copBlock->currCount = 4; // Rewind to shift cmd pos
		CopMove(copList, manager->copBlock, &customRegister->bplcon1, shift);
		for(UBYTE i = 0; i < manager->back->Depth; ++i) {
			planeAddr = ((ULONG)manager->back->Planes[i]) + bplOffs;
			CopMove(copList, manager->copBlock, &bplFetch[i].hi, planeAddr >> 16);
			CopMove(copList, manager->copBlock, &bplFetch[i].lo, planeAddr & 0xFFFF);
		}
	}

	// Swap buffers if needed
	if(manager->back != manager->front) {
		BitMap *tmp = manager->back;
		manager->back = manager->front;
		manager->front = tmp;
	}
}


static void BufferSetBack(SimpleBufferManager *manager, BitMap *back) {

	manager->back = back;
}


void BufferSetFront(SimpleBufferManager *manager, BitMap *front) {


	manager->bfrBounds.x = BitmapGetByteWidth(front) << 3;
	manager->bfrBounds.y = front->Rows;
	manager->front = front;
	UWORD modulo = front->BytesPerRow - (manager->common.vPort->width >> 3);
	UWORD dDfStrt;
	if(manager->bfrBounds.x <= manager->common.vPort->width) {
		dDfStrt = 0x0038;
		manager->flags &= ~SIMPLEBUFFER_FLAG_X_SCROLLABLE;
	}
	else {
		manager->flags |= SIMPLEBUFFER_FLAG_X_SCROLLABLE;
		dDfStrt = 0x0030;
		modulo -= 2;
	}

	// Update (rewrite) copperlist
	// TODO this could be unified with copBlock being set with copSetMove too
	EngineCopList *copList = manager->common.vPort->view->copList;
	if(manager->flags & SIMPLEBUFFER_FLAG_COPLIST_RAW) {
		// Since simpleBufferProcess only updates bitplane ptrs and shift,
		// copperlist must be shaped here.
		// WAIT is calc'd in same way as in copBlockCreate in simpleBufferCreate().
		CopCmd *cmdList = &copList->backBfr->list[manager->copperOffset];
		
		CopSetWait(&cmdList[0].wait, 0xE2-7*4, manager->common.vPort->offsetY + 0x2C-1);
		CopSetMove(&cmdList[1].move, &customRegister->ddfstop, 0x00D0);    // Data fetch
		CopSetMove(&cmdList[2].move, &customRegister->ddfstrt, dDfStrt);
		CopSetMove(&cmdList[3].move, &customRegister->bpl1mod, modulo);  // Bitplane modulo
		CopSetMove(&cmdList[4].move, &customRegister->bpl2mod, modulo);
		CopSetMove(&cmdList[5].move, &customRegister->bplcon1, 0);         // Shift: 0
		UBYTE i;
		ULONG planeAddr;
		for (i = 0; i < manager->common.vPort->BPP; ++i) {
			planeAddr = (ULONG)manager->front->Planes[i];
			CopSetMove(&cmdList[6 + i*2 + 0].move, &bplFetch[i].hi, planeAddr >> 16);
			CopSetMove(&cmdList[6 + i*2 + 1].move, &bplFetch[i].lo, planeAddr & 0xFFFF);
		}
		// Copy to front buffer since it needs initialization there too
		CopyMem(
			&copList->backBfr->list[manager->copperOffset],
			&copList->frontBfr->list[manager->copperOffset],
			(6+2*manager->common.vPort->BPP)*sizeof(CopCmd)
		);
	}
	else {
		CopBlock *block = manager->copBlock;
		block->currCount = 0; // Rewind to beginning
		CopMove(copList, block, &customRegister->ddfstop, 0x00D0);     // Data fetch
		CopMove(copList, block, &customRegister->ddfstrt, dDfStrt);
		CopMove(copList, block, &customRegister->bpl1mod, modulo);   // Bitplane modulo
		CopMove(copList, block, &customRegister->bpl2mod, modulo);
		CopMove(copList, block, &customRegister->bplcon1, 0);          // Shift: 0
		for (UBYTE i = 0; i < manager->common.vPort->BPP; ++i) {
			ULONG planeAddr = (ULONG)manager->front->Planes[i];
			CopMove(copList, block, &bplFetch[i].hi, planeAddr >> 16);
			CopMove(copList, block, &bplFetch[i].lo, planeAddr & 0xFFFF);
		}
	}
}


VpManager *VPortGetManager(VPort *vPort, UBYTE id) {
	VpManager *manager;

	manager = vPort->firstManager;
	while(manager) {
		if(manager->id == id) {
			return manager;
		}
		manager = manager->next;
	}
	return 0;
}



SimpleBufferManager *SimpleBufferCreate(void *tags, ...) {
	va_list vaTags;
	EngineCopList *copList;
	SimpleBufferManager *manager;
	UWORD boundWidth, boundHeight;
	UBYTE bitmapFlags;
	BitMap *front = 0, *back = 0;
	UBYTE isCameraCreated = 0;

	va_start(vaTags, tags);

	// Init manager
	manager = MemAllocFastClear(sizeof(SimpleBufferManager));
	manager->common.process = (VpManagerFn)BufferProcess;
	manager->common.destroy = (VpManagerFn)BufferDestroy;
	manager->common.id = VPM_SCROLL;

	VPort *vPort = (VPort*)GetTag(tags, vaTags, TAG_SIMPLEBUFFER_VPORT, 0);
	manager->common.vPort = vPort;

	// Buffer bitmap
	boundWidth = GetTag(
		tags, vaTags, TAG_SIMPLEBUFFER_BOUND_WIDTH, vPort->width
	);
	boundHeight = GetTag(
		tags, vaTags, TAG_SIMPLEBUFFER_BOUND_HEIGHT, vPort->height
	);
	bitmapFlags = GetTag(
		tags, vaTags, TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR
	);
	front = BitmapCreate(
		boundWidth, boundHeight, vPort->BPP, bitmapFlags
	);
	
	UBYTE isDblBfr = GetTag(tags, vaTags, TAG_SIMPLEBUFFER_IS_DBLBUF, 0);
	if(isDblBfr) {
		back = BitmapCreate(
			boundWidth, boundHeight, vPort->BPP, bitmapFlags
		);
	}

	// Find camera manager, create if not exists
	manager->camera = (CameraManager*)VPortGetManager(vPort, VPM_CAMERA);
	if(!manager->camera) {
		isCameraCreated = 1;
		manager->camera = CameraCreate(
			vPort, 0, 0, boundWidth, boundHeight, isDblBfr
		);
	}

	copList = vPort->view->copList;
	if(copList->mode == COPPER_MODE_BLOCK) {
		// CopBlock contains: bitplanes + shiftX
		manager->copBlock = CopBlockCreate(
			copList,
			// Shift +2 ddf +2 modulos +2*bpp bpladdr
			2*vPort->BPP + 5,
			// Vertically addition from DiWStrt, horizontally a bit before last fetch.
			// First to set are ddf, modulos & shift so they are changed during fetch.
			0xE2-7*4, vPort->offsetY + 0x2C-1
		);
	}
	else {
		const UWORD invalidCopOffs = -1;
		manager->flags |= SIMPLEBUFFER_FLAG_COPLIST_RAW;
		manager->copperOffset = GetTag(
			tags, vaTags, TAG_SIMPLEBUFFER_COPLIST_OFFSET, invalidCopOffs
		);
	}

	BufferSetFront(manager, front);
	BufferSetBack(manager, back ? back : front);

	// Add manager to VPort
	VPortAddManager(vPort, (VpManager*)manager);
	va_end(vaTags);
	return manager;

}



void InitScreen()
{
	engine.platformScreen.view = ViewCreate(0,
						 TAG_VIEW_GLOBAL_CLUT, 1,
						 TAG_DONE);

	engine.platformScreen.vPort = VPortCreate(0,
						   TAG_VPORT_VIEW, engine.platformScreen.view,
						   TAG_VPORT_BPP, DEPTH,
						   TAG_END);

	engine.platformScreen.buffer = SimpleBufferCreate(0,
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
	
	BlitManagerDestroy();
	LoadGameView(0);
}

void FreeView()
{
	
	DestroyView(engine.platformScreen.view);
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