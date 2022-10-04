/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _ACE_MANAGERS_SYSTEM_H_
#define _ACE_MANAGERS_SYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <graphics/gfxbase.h> // Required for GfxBase
#include <ace/types.h>
#include <ace/utils/custom.h>

//---------------------------------------------------------------------- DEFINES

//------------------------------------------------------------------------ TYPES

typedef void (*tAceIntHandler)(
	REGARG(volatile tCustom *pCustom, "a0"), REGARG(volatile void *pData, "a1")
);

//-------------------------------------------------------------------- FUNCTIONS

/**
 * @brief The startup code to give ACE somewhat initial state.
 * Prepares OS for enabling / disabling. Disables as much of it as possible,
 * but leaves it in enabled state.
 * This is the first thing you should call in your ACE app.
 */
void systemCreate(void);

/**
 * @brief Cleans up after app, restores anything that systemCreate took over.
 * After running the function, the system to its state before running your app.
 * This is the last thing you should call in your ACE app.
 */
void systemDestroy(void);

void systemKill(const char *szMsg);

void systemSetInt(
	UBYTE ubIntNumber, tAceIntHandler pHandler, volatile void *pIntData
);

void systemSetCiaInt(
	UBYTE ubCia, UBYTE ubIntBit, tAceIntHandler pHandler, volatile void *pIntData
);

void systemSetCiaCr(UBYTE ubCia, UBYTE isCrB, UBYTE ubCrValue);

void systemUse(void);

void systemUnuse(void);

UBYTE systemIsUsed(void);

void systemGetBlitterFromOs(void);

void systemReleaseBlitterToOs(void);

UBYTE systemBlitterIsUsed(void);

void systemDump(void);

void systemSetInt(
	UBYTE ubIntNumber, tAceIntHandler pHandler, volatile void *pIntData
);

void systemSetDmaBit(UBYTE ubDmaBit, UBYTE isEnabled);

void systemSetDmaMask(UWORD uwDmaMask, UBYTE isEnabled);

void systemSetTimer(UBYTE ubCia, UBYTE ubTimer, UWORD uwTicks);

void systemIdleBegin(void);

void systemIdleEnd(void);

UBYTE systemGetVerticalBlankFrequency(void);

UBYTE systemIsPal(void);
//---------------------------------------------------------------------- GLOBALS

extern struct GfxBase *GfxBase;

#ifdef __cplusplus
}
#endif

#endif // _ACE_MANAGERS_SYSTEM_H_
