/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <ace/managers/rand.h>

//-------------------------------------------------------------------- FUNCTIONS

void randInit(ULONG ulSeed) {
	g_sRandManager.ulRandX = 123456789 + ulSeed;
	g_sRandManager.ulRandY = 362436069 + ulSeed;
	g_sRandManager.ulRandZ = 521288629 + ulSeed;
	g_sRandManager.uwRandX = g_sRandManager.ulRandX & 0xFFFF;
	g_sRandManager.uwRandY = g_sRandManager.ulRandY & 0xFFFF;
	g_sRandManager.uwRandZ = g_sRandManager.ulRandZ & 0xFFFF;
	g_sRandManager.uwRandW = (88675123 + ulSeed) & 0xFFFF;
}

UWORD uwRand(void) {
	UWORD uwT = g_sRandManager.uwRandX ^ (g_sRandManager.uwRandX << 11);
	g_sRandManager.uwRandX = g_sRandManager.uwRandY;
	g_sRandManager.uwRandY = g_sRandManager.uwRandZ;
	g_sRandManager.uwRandZ = g_sRandManager.uwRandW;
	return g_sRandManager.uwRandW = (g_sRandManager.uwRandW ^ (g_sRandManager.uwRandW >> 19)) ^ (uwT ^ (uwT >> 8));
}

UWORD uwRandMax(UWORD uwMax) {
	return uwRand() % (uwMax + 1);
}

UWORD uwRandMinMax(UWORD uwMin, UWORD uwMax) {
	return uwMin + uwRandMax(uwMax - uwMin);
}

// ULONG is slower on Amiga, use UWORD instead
ULONG ulRand(void) {
	g_sRandManager.ulRandX ^= g_sRandManager.ulRandX << 16;
	g_sRandManager.ulRandX ^= g_sRandManager.ulRandX >> 5;
	g_sRandManager.ulRandX ^= g_sRandManager.ulRandX << 1;

	ULONG ulRandT = g_sRandManager.ulRandX;
	g_sRandManager.ulRandX = g_sRandManager.ulRandY;
	g_sRandManager.ulRandY = g_sRandManager.ulRandZ;
	g_sRandManager.ulRandZ = ulRandT ^ g_sRandManager.ulRandX ^ g_sRandManager.ulRandY;

	return g_sRandManager.ulRandZ;
}

ULONG ulRandMax(ULONG ulMax) {
	return ulRand() % (ulMax + 1);
}

ULONG ulRandMinMax(ULONG ulMin, ULONG ulMax) {
	return ulMin + ulRandMax(ulMax - ulMin);
}

//---------------------------------------------------------------------- GLOBALS

tRandManager g_sRandManager;
