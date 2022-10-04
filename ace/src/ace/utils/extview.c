/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <ace/utils/extview.h>
#include <limits.h>
#include <ace/managers/system.h>
#include <ace/utils/tag.h>
#include <ace/generic/screen.h>
static UBYTE s_isPAL = 1;

tView *viewCreate(void *pTags, ...) {

	logBlockBegin("viewCreate(pTags: %p)", pTags);
#ifdef AMIGA

	s_isPAL = systemIsPal();

	
	// Create view stub
	tView *pView = memAllocFastClear(sizeof(tView));
	logWrite("created %s viewport addr: %p\n", s_isPAL ? "PAL" : "NTSC", pView);

	va_list vaTags;
	va_start(vaTags, pTags);

	// Process copperlist raw/block tags
	if(
		tagGet(pTags, vaTags, TAG_VIEW_COPLIST_MODE, VIEW_COPLIST_MODE_BLOCK)
		== VIEW_COPLIST_MODE_RAW
	) {
		ULONG ulCopListSize = tagGet(pTags, vaTags, TAG_VIEW_COPLIST_RAW_COUNT, -1);
		pView->pCopList = copListCreate(0,
			TAG_COPPER_LIST_MODE, COPPER_MODE_RAW,
			TAG_COPPER_RAW_COUNT, ulCopListSize,
			TAG_DONE
		);
		pView->uwFlags |= VIEW_FLAG_COPLIST_RAW;
	}
	else {
		pView->pCopList = copListCreate(0, TAG_DONE);
	}

	// Additional CLUT tags
	if(tagGet(pTags, vaTags, TAG_VIEW_GLOBAL_CLUT, 0)) {
		pView->uwFlags |= VIEW_FLAG_GLOBAL_CLUT;
		logWrite("Global CLUT mode enabled\n");
	}

	// Get the Y pos and height
	const UWORD uwDefaultHeight = -1;
	const UBYTE ubDefaultPosY = -1;
	UWORD uwHeight = tagGet(pTags, vaTags, TAG_VIEW_WINDOW_HEIGHT, uwDefaultHeight);
	UBYTE ubPosY = tagGet(pTags, vaTags, TAG_VIEW_WINDOW_START_Y, ubDefaultPosY);
	if(uwHeight != uwDefaultHeight && ubPosY == ubDefaultPosY) {
		// Only height is passed - calculate Y pos so that display is centered
		pView->uwHeight = uwHeight;
		if (s_isPAL) {
			pView->ubPosY = SCREEN_PAL_YOFFSET + (SCREEN_PAL_HEIGHT - uwHeight) / 2;
		}
		else{
			pView->ubPosY = SCREEN_NTSC_YOFFSET + (SCREEN_NTSC_HEIGHT - uwHeight) / 2;
		}
		
	}
	else if(uwHeight == uwDefaultHeight && ubPosY != ubDefaultPosY) {
		// Only Y pos is passed - calculate height as the remaining area of PAL display
		pView->ubPosY = ubPosY;
		if (s_isPAL) {
			pView->uwHeight = SCREEN_PAL_YOFFSET + SCREEN_PAL_HEIGHT - ubPosY;
		}
		else{
			pView->uwHeight = SCREEN_NTSC_YOFFSET + SCREEN_NTSC_HEIGHT - ubPosY;
		}
		
	}
	else if(uwHeight == uwDefaultHeight && ubPosY == ubDefaultPosY) {
		// All default - use PAL
		if (s_isPAL) {
			pView->ubPosY = SCREEN_PAL_YOFFSET;
			pView->uwHeight = SCREEN_PAL_HEIGHT;
		}
		else {
			pView->ubPosY = SCREEN_NTSC_YOFFSET;
			pView->uwHeight = SCREEN_NTSC_HEIGHT;
		}
	}
	else {
		// Use passed values
		pView->ubPosY = ubPosY;
		pView->uwHeight = uwHeight;
	}
	logWrite(
		"Display pos: %hhu,%hhu, size: %hu,%hu\n",
		0x81, pView->ubPosY, SCREEN_PAL_WIDTH, pView->uwHeight
	);

	va_end(vaTags);
	logBlockEnd("viewCreate()");
	return pView;
#else
	logBlockEnd("viewCreate()");
	return 0;
#endif // AMIGA
}

void viewDestroy(tView *pView) {
	logBlockBegin("viewDestroy(pView: %p)", pView);
#ifdef AMIGA
	if(g_sCopManager.pCopList == pView->pCopList) {
		viewLoad(0);
	}

	// Free all attached viewports
	while(pView->pFirstVPort) {
		vPortDestroy(pView->pFirstVPort);
	}

	// Free view
	logWrite("Freeing copperlists...\n");
	copListDestroy(pView->pCopList);
	memFree(pView, sizeof(tView));
#endif // AMIGA
	logBlockEnd("viewDestroy()");
}

void vPortProcessManagers(tVPort *pVPort) {
	tVpManager *pManager = pVPort->pFirstManager;
	while(pManager) {
		pManager->process(pManager);
		pManager = pManager->pNext;
	}
}

void viewProcessManagers(tView *pView) {
	tVPort *pVPort = pView->pFirstVPort;
	while(pVPort) {
		vPortProcessManagers(pVPort);
		pVPort = pVPort->pNext;
	}
}

void viewUpdateCLUT(tView *pView) {
#ifdef AMIGA
	if(pView->uwFlags & VIEW_FLAG_GLOBAL_CLUT) {
		for(UBYTE i = 0; i < 32; ++i) {
			g_pCustom->color[i] = pView->pFirstVPort->pPalette[i];
		}
	}
	else {
		// na petli: vPortUpdateCLUT();
	}
#endif // AMIGA
}

/**
 *  @todo bplcon0 BPP is set up globally - make it only when all vports
 *        are truly of same BPP.
 */
void viewLoad(tView *pView) {
	logBlockBegin("viewLoad(pView: %p)", pView);

	s_isPAL = systemIsPal();
	UWORD uwWaitPos = (s_isPAL == 1) ? 300 : 260;
	// if we are setting a NULL viewport we need to know if pal/NTSC
	while(getRayPos().bfPosY < uwWaitPos) {}
#if defined(AMIGA)
	if(!pView) {
		
		g_sCopManager.pCopList = g_sCopManager.pBlankList;
		g_pCustom->bplcon0 = 0; // No output
		g_pCustom->bplcon3 = 0; // AGA fix
		g_pCustom->fmode = 0;   // AGA fix
		for(UBYTE i = 0; i < 6; ++i) {
			g_pCustom->bplpt[i] = 0;
		}
		g_pCustom->bpl1mod = 0;
		g_pCustom->bpl2mod = 0;
	}
	else {
#if defined(ACE_DEBUG)
		{
		
			// Check if view size matches size of last vport
			tVPort *pVp = pView->pFirstVPort;
			while(pVp->pNext) {
				pVp = pVp->pNext;
			}
			if(pVp->uwOffsY + pVp->uwHeight != pView->uwHeight) {
				logWrite(
					"ERR: View height %hu doesn't match the total VPort area: %hu",
					pView->uwHeight, pVp->uwOffsY + pVp->uwHeight
				);
			}
		}
#endif
		g_sCopManager.pCopList = pView->pCopList;
		g_pCustom->bplcon0 = (pView->pFirstVPort->ubBPP << 12) | BV(9); // BPP + composite output
		g_pCustom->fmode = 0;        // AGA fix
		g_pCustom->bplcon3 = 0;      // AGA fix
		g_pCustom->diwstrt = (pView->ubPosY << 8) | 0x81; // HSTART: 0x81
		UWORD uwDiwStopY = pView->ubPosY + pView->uwHeight;
		if(BTST(uwDiwStopY, 8) == BTST(uwDiwStopY, 7)) {
			logWrite(
				"ERR: DiwStopY (%hu) bit 8 (%hhu) must be different than bit 7 (%hhu)\n",
				uwDiwStopY, BTST(uwDiwStopY, 8), BTST(uwDiwStopY, 7)
			);
		}
		g_pCustom->diwstop = ((uwDiwStopY & 0xFF) << 8) | 0xC1; // HSTOP: 0xC1
		viewUpdateCLUT(pView);
	}
	copProcessBlocks();
	g_pCustom->copjmp1 = 1;
	systemSetDmaBit(DMAB_RASTER, pView != 0);

	// if we are setting a NULL viewport we need to know if pal/NTSC
	while(getRayPos().bfPosY < uwWaitPos) {}

#endif // AMIGA
	logBlockEnd("viewLoad()");
}

tVPort *vPortCreate(void *pTagList, ...) {
	logBlockBegin("vPortCreate(pTagList: %p)", pTagList);
	va_list vaTags;
	va_start(vaTags, pTagList);
#ifdef AMIGA
	s_isPAL = systemIsPal();

	tVPort *pVPort = memAllocFastClear(sizeof(tVPort));
	logWrite("Addr: %p\n", pVPort);

	// Determine parent view
	tView *pView = (tView*)tagGet(pTagList, vaTags, TAG_VPORT_VIEW, 0);
	if(!pView) {
		logWrite("ERR: no view ptr in TAG_VPORT_VIEW specified!\n");
		goto fail;
	}
	pVPort->pView = pView;
	logWrite("Parent view: %p\n", pView);

	const UWORD uwDefaultWidth = s_isPAL ? SCREEN_PAL_WIDTH : SCREEN_NTSC_WIDTH;
	const UWORD uwDefaultHeight = -1;
	const UWORD uwDefaultBpp = 4; // 'Cuz copper is slower @ 5bpp & more in OCS

	// Calculate Y offset - beneath previous ViewPort
	pVPort->uwOffsY = 0;
	tVPort *pPrevVPort = pView->pFirstVPort;
	while(pPrevVPort) {
		pVPort->uwOffsY += pPrevVPort->uwHeight;
		pPrevVPort = pPrevVPort->pNext;
	}
	if(pVPort->uwOffsY && !(pView->uwFlags & VIEW_FLAG_GLOBAL_CLUT)) {
		pVPort->uwOffsY += 2; // TODO: not always required?
	}
	ULONG ulAddOffsY = tagGet(pTagList, vaTags, TAG_VPORT_OFFSET_TOP, 0);
	pVPort->uwOffsY += ulAddOffsY;
	logWrite("Offsets: %ux%u\n", pVPort->uwOffsX, pVPort->uwOffsY);

	// Get dimensions
	pVPort->uwWidth = tagGet(pTagList, vaTags, TAG_VPORT_WIDTH, uwDefaultWidth);
	pVPort->uwHeight = tagGet(pTagList, vaTags, TAG_VPORT_HEIGHT, uwDefaultHeight);
	if(pVPort->uwHeight == uwDefaultHeight) {
		pVPort->uwHeight = pView->uwHeight - pVPort->uwOffsY;
	}
	pVPort->ubBPP = tagGet(pTagList, vaTags, TAG_VPORT_BPP, uwDefaultBpp);
	logWrite(
		"Dimensions: %ux%u@%hu\n", pVPort->uwWidth, pVPort->uwHeight, pVPort->ubBPP
	);

	// Update view - add to vPort list
	++pView->ubVpCount;
	if(!pView->pFirstVPort) {
		pView->pFirstVPort = pVPort;
		logWrite("No prev VPorts - added to head\n");
	}
	else {
		pPrevVPort = pView->pFirstVPort;
		while(pPrevVPort->pNext) {
			pPrevVPort = pPrevVPort->pNext;
		}
		pPrevVPort->pNext = pVPort;
		logWrite("VPort added after %p\n", pPrevVPort);
	}

	// Palette tag
	UWORD *pSrcPalette = (UWORD*)tagGet(pTagList, vaTags, TAG_VPORT_PALETTE_PTR, 0);
	if(pSrcPalette) {
		UWORD uwPaletteSize = tagGet(pTagList, vaTags, TAG_VPORT_PALETTE_SIZE, 0xFFFF);
		if(uwPaletteSize == 0xFFFF) {
			logWrite("WARN: you must specify palette size in TAG_VPORT_PALETTE_SIZE\n");
		}
		else if(!uwPaletteSize || uwPaletteSize > 32) {
			logWrite("ERR: Wrong palette size: %hu\n", uwPaletteSize);
		}
		else {
			memcpy(pVPort->pPalette, pSrcPalette, uwPaletteSize * sizeof(UWORD));
		}
	}

	va_end(vaTags);
	logBlockEnd("vPortCreate()");
	return pVPort;
#endif // AMIGA
fail:
	va_end(vaTags);
	logBlockEnd("vPortCreate()");
	return 0;
}

void vPortDestroy(tVPort *pVPort) {
	logBlockBegin("vPortDestroy(pVPort: %p)", pVPort);
	tView *pView;
	tVPort *pPrevVPort, *pCurrVPort;

	pView = pVPort->pView;
	logWrite("Parent extView: %p\n", pView);
	pPrevVPort = 0;
	pCurrVPort = pView->pFirstVPort;
	while(pCurrVPort) {
		logWrite("found VP: %p...", pCurrVPort);
		if(pCurrVPort == pVPort) {
			logWrite(" gotcha!\n");

			// Remove from list
			if(pPrevVPort) {
				pPrevVPort->pNext = pCurrVPort->pNext;
			}
			else {
				pView->pFirstVPort = pCurrVPort->pNext;
			}
			--pView->ubVpCount;

			// Destroy managers
			logBlockBegin("Destroying managers");
			while(pCurrVPort->pFirstManager) {
				vPortRmManager(pCurrVPort, pCurrVPort->pFirstManager);
			}
			logBlockEnd("Destroying managers");

			// Free stuff
			memFree(pVPort, sizeof(tVPort));
			break;
		}
		else {
			logWrite("\n");
		}
		pPrevVPort = pCurrVPort;
		pCurrVPort = pCurrVPort->pNext;
	}
	logBlockEnd("vPortDestroy()");
}


void vPortUpdateCLUT(tVPort *pVPort) {
	// TODO: If not same CLUTs on all vports, there are 2 strategies to do them:
	// 1) Using the copperlist (copblock/raw copper instructions)
	// 2) By using CPU
	// 1st approach is better since it will always work, doesn't require any waits
	// So the only problem is implementing it using copblocks or raw copperlist.
	// Also some viewports which are one after another may use shared pallette, so
	// adding CLUT copper instructions between them is unnecessary.
	// I propose following implementation:
	// - for quick check, view contains flag VIEW_GLOBAL_CLUT if all viewports use
	//   same palette
	// - if VIEW_GLOBAL_CLUT is not found, every vport is checked for
	// VPORT_HAS_OWN_CLUT and if it's present then cop instructions are created
	// during vport creation and updated using this fn.
	// There is a problem that VPorts may change vertical size and position
	// (like expanding HUD to fullscreen like we did in Goblin Villages).
	// On copblocks implementing it is somewhat easy, but on raw copperlist
	// something clever must be done.
	if(pVPort->uwFlags & VIEWPORT_HAS_OWN_CLUT) {
		logWrite("TODO: implement vPortUpdateCLUT!\n");
	}
}

void vPortWaitForPos(const tVPort *pVPort, UWORD uwPosY, UBYTE isExact) {
#ifdef AMIGA
	// Determine VPort end position
	UWORD uwEndPos = pVPort->uwOffsY + uwPosY;
	uwEndPos += pVPort->pView->ubPosY; // Addition from DiWStrt
#if defined(ACE_DEBUG)
	UWORD yPos = s_isPAL ? 312 : 272;
	if(uwEndPos >= yPos) {
		logWrite("ERR: vPortWaitForPos - too big wait pos: %04hx (%hu)\n", uwEndPos, uwEndPos);
		logWrite("\tVPort offs: %hu, pos: %hu\n", pVPort->uwOffsY, uwPosY);
	}
#endif

	if(isExact) {
		// If current beam pos is on or past end pos, wait for start of next frame
		while (getRayPos().bfPosY >= uwEndPos) {}
	}
	// If current beam pos is before end pos, wait for it
	while (getRayPos().bfPosY < uwEndPos) {}
#endif // AMIGA
}

void vPortWaitUntilEnd(const tVPort *pVPort) {
	vPortWaitForPos(pVPort, pVPort->uwHeight, 0);
}

void vPortWaitForEnd(const tVPort *pVPort) {
	vPortWaitForPos(pVPort, pVPort->uwHeight, 1);
}

void vPortAddManager(tVPort *pVPort, tVpManager *pVpManager) {
	// Check if we have any other manager - if not, attach as head
	if(!pVPort->pFirstManager) {
		pVPort->pFirstManager = pVpManager;
		logWrite("Manager %p attached to head of VP %p\n", pVpManager, pVPort);
		return;
	}

	// Check if current manager has lesser priority number than head
	if(pVPort->pFirstManager->ubId > pVpManager->ubId) {
		logWrite(
			"Manager %p attached as head of VP %p before %p\n",
			pVpManager, pVPort, pVPort->pFirstManager
		);
		pVpManager->pNext = pVPort->pFirstManager;
		pVPort->pFirstManager = pVpManager;
		return;
	}

	// Insert before manager of bigger priority number
	tVpManager *pVpCurr = pVPort->pFirstManager;
	while(pVpCurr->pNext && pVpCurr->pNext->ubId <= pVpManager->ubId) {
		if(pVpCurr->ubId <= pVpManager->ubId) {
			pVpCurr = pVpCurr->pNext;
		}
	}
	pVpManager->pNext = pVpCurr->pNext;
	pVpCurr->pNext = pVpManager;
	logWrite(
		"Manager %p attached after manager %p of VP %p\n",
		pVpManager, pVpCurr, pVPort
	);
}

void vPortRmManager(tVPort *pVPort, tVpManager *pVpManager) {
	if(!pVPort->pFirstManager) {
		logWrite("ERR: vPort %p has no managers!\n", pVPort);
		return;
	}
	if(pVPort->pFirstManager == pVpManager) {
		logWrite("Destroying manager %u @addr: %p\n", pVpManager->ubId, pVpManager);
		pVPort->pFirstManager = pVpManager->pNext;
		pVpManager->destroy(pVpManager);
		return;
	}
	tVpManager *pParent = pVPort->pFirstManager;
	while(pParent->pNext) {
		if(pParent->pNext == pVpManager) {
			logWrite("Destroying manager %u @addr: %p\n", pVpManager->ubId, pVpManager);
			pParent->pNext = pVpManager->pNext;
			pVpManager->destroy(pVpManager);
			return;
		}
	}
	logWrite("ERR: vPort %p manager %p not found!\n", pVPort, pVpManager);
}

tVpManager *vPortGetManager(tVPort *pVPort, UBYTE ubId) {
	tVpManager *pManager;

	pManager = pVPort->pFirstManager;
	while(pManager) {
		if(pManager->ubId == ubId) {
			return pManager;
		}
		pManager = pManager->pNext;
	}
	return 0;
}
