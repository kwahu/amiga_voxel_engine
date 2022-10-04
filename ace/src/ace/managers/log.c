/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <ace/managers/log.h>
#include <string.h>
#include <ace/macros.h>
#include <ace/managers/system.h>
#ifdef ACE_DEBUG

// Globals
tLogManager g_sLogManager = {0};

#ifdef ACE_DEBUG_UAE

	#if defined(BARTMAN_GCC)
long (*bartmanLog)(long mode, const char *string) = (long (*)(long, const char *))0xf0ff60;
static inline void uaeWrite(const char *szMsg) {
	if (*((UWORD *)bartmanLog) == 0x4eb9 || *((UWORD *)bartmanLog) == 0xa00e) {
		bartmanLog(86, szMsg);
	}
}
	#else
static inline void uaeWrite(const char *szMsg) {
	volatile ULONG * const s_pUaeFmt = (ULONG *)0xBFFF04;
	*s_pUaeFmt = (ULONG)((UBYTE*)szMsg);
}
	#endif

#else
#define uaeWrite(x)
#endif

// Functions

/**
 * Base debug functions
 */

void _logOpen(const char *szFilePath) {
	g_sLogManager.pFile = szFilePath ? fileOpen(szFilePath, "w") : 0;
	g_sLogManager.ubIndent = 0;
	g_sLogManager.wasLastInline = 0;
	g_sLogManager.isBlockEmpty = 1;
	g_sLogManager.ubShutUp = 0;
}

void _logPushIndent(void) {
	++g_sLogManager.ubIndent;
}

void _logPopIndent(void) {
	--g_sLogManager.ubIndent;
}

void _logWrite(char *szFormat, ...) {
	va_list vaArgs;
	va_start(vaArgs, szFormat);
	logWriteVa(szFormat, vaArgs);
	va_end(vaArgs);
}

void _logWriteVa(char *szFormat, va_list vaArgs) {
	if(g_sLogManager.ubShutUp) {
		return;
	}

	// Bartman's UAE logger appends newline to each print, so the message must
	// be emitted in one print with indentation.
	char szMsg[1024];
	UWORD uwOffs = 0;
	g_sLogManager.isBlockEmpty = 0;
	if (!g_sLogManager.wasLastInline) {
		UBYTE ubLogIndent = g_sLogManager.ubIndent;
		while (ubLogIndent--) {
			szMsg[uwOffs++] = '\t';
		}
	}

	g_sLogManager.wasLastInline = szFormat[strlen(szFormat) - 1] != '\n';

	vsprintf(&szMsg[uwOffs], szFormat, vaArgs);
	uaeWrite(szMsg);
	if(g_sLogManager.pFile && !g_sLogManager.wIntDepth) {
		fileWrite(g_sLogManager.pFile, szMsg, strlen(szMsg));
		fileFlush(g_sLogManager.pFile);
	}
}

void _logClose(void) {
	logWrite("Log closed successfully\n");
	if(g_sLogManager.pFile) {
		fileClose(g_sLogManager.pFile);
		g_sLogManager.pFile = 0;
	}
}

/**
 * Extended debug functions
 */

// Log blocks
void _logBlockBegin(char *szBlockName, ...) {
	if(g_sLogManager.ubShutUp) {
		return;
	}
	systemUse();

	logWrite("Block begin: ");
	va_list vaArgs;
	va_start(vaArgs, szBlockName);
	logWriteVa(szBlockName, vaArgs);
	va_end(vaArgs);
	logWrite("\n");

	g_sLogManager.pTimeStack[g_sLogManager.ubIndent] = timerGetPrec();
	logPushIndent();
	g_sLogManager.isBlockEmpty = 1;
	memCheckIntegrity();
	systemUnuse();
}

void _logBlockEnd(char *szBlockName) {
	if(g_sLogManager.ubShutUp) {
		return;
	}
	systemUse();
	memCheckIntegrity();
	logPopIndent();
	timerFormatPrec(
		g_sLogManager.szTimeBfr,
		timerGetDelta(
			g_sLogManager.pTimeStack[g_sLogManager.ubIndent],
			timerGetPrec()
		)
	);
	if(g_sLogManager.isBlockEmpty) {
		// empty block - collapse to single line
		g_sLogManager.wasLastInline = 1;
		if(g_sLogManager.pFile) {
			fileSeek(g_sLogManager.pFile, -1, SEEK_CUR);
		}
		logWrite("...OK, time: %s\n", g_sLogManager.szTimeBfr);
	}
	else {
		logWrite("Block end: %s, time: %s\n", szBlockName, g_sLogManager.szTimeBfr);
	}
	g_sLogManager.isBlockEmpty = 0;
	systemUnuse();
}

// Average logging

tAvg *_logAvgCreate(char *szName, UWORD uwAllocCount) {
	tAvg *pAvg = memAllocFast(sizeof(tAvg));
	pAvg->szName = szName;
	pAvg->uwAllocCount = uwAllocCount;
	pAvg->uwCurrDelta = 0;
	pAvg->uwUsedCount = 0;
	pAvg->pDeltas = memAllocFast(uwAllocCount*sizeof(ULONG));
	pAvg->ulMin = 0xFFFFFFFF;
	pAvg->ulMax = 0;
	return pAvg;
}

void _logAvgDestroy(tAvg *pAvg) {
	logAvgWrite(pAvg);
	memFree(pAvg->pDeltas, pAvg->uwAllocCount*sizeof(ULONG));
	memFree(pAvg, sizeof(tAvg));
}

void _logAvgBegin(tAvg *pAvg) {
	pAvg->ulStartTime = timerGetPrec();
}

void _logAvgEnd(tAvg *pAvg) {
	// Calculate timestamp
	pAvg->pDeltas[pAvg->uwCurrDelta] = timerGetDelta(pAvg->ulStartTime, timerGetPrec());
	// Update min/max
	if(pAvg->pDeltas[pAvg->uwCurrDelta] > pAvg->ulMax) {
		pAvg->ulMax = pAvg->pDeltas[pAvg->uwCurrDelta];
	}
	if(pAvg->pDeltas[pAvg->uwCurrDelta] < pAvg->ulMin) {
		pAvg->ulMin = pAvg->pDeltas[pAvg->uwCurrDelta];
	}
	++pAvg->uwCurrDelta;
	// Roll
	if(pAvg->uwCurrDelta >= pAvg->uwAllocCount) {
		pAvg->uwCurrDelta = 0;
	}
	pAvg->uwUsedCount = MIN(pAvg->uwAllocCount, pAvg->uwUsedCount + 1);
}

void _logAvgWrite(tAvg *pAvg) {
	ULONG ulAvg = 0;
	char szAvg[15];
	char szMin[15];
	char szMax[15];

	if(!pAvg->uwUsedCount) {
		logWrite("Avg %s: No measures taken!\n", pAvg->szName);
		return;
	}
	// Calculate average time
	for(UWORD i = pAvg->uwUsedCount; i--;) {
		ulAvg += pAvg->pDeltas[i];
	}
	ulAvg /= pAvg->uwUsedCount;

	// Display info
	timerFormatPrec(szAvg, ulAvg);
	timerFormatPrec(szMin, pAvg->ulMin);
	timerFormatPrec(szMax, pAvg->ulMax);
	logWrite("Avg %s: %s, min: %s, max: %s\n", pAvg->szName, szAvg, szMin, szMax);
}

void _logPushInt(void) {
	++g_sLogManager.wIntDepth;
}

void _logPopInt(void) {
	if(--g_sLogManager.wIntDepth < 0) {
		logWrite("ERR: INT DEPTH NEGATIVE!\n");
	}
}

#endif // ACE_DEBUG
