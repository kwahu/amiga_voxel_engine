/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _ACE_UTILS_PALETTE_H
#define _ACE_UTILS_PALETTE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Palette utilities.
 */

#include <ace/types.h>
#include <ace/utils/extview.h>

/* Types */

/* Globals */

/* Functions */

/**
 *  @brief Loads palette from supplied .plt file to given address.
 *  @param szFileName  Palette source path.
 *  @param pPalette    Palette destination pointer.
 *  @param ubMaxLength Maximum number of colors in palette.
 */
void paletteLoad(const char *szFileName, UWORD *pPalette, UBYTE ubMaxLength);

/**
 *  @brief Loads palette from supplied .plt stored in memory to given address.
 *  @param pData       Palette source pointer.
 *  @param pPalette    Palette destination pointer.
 *  @param ubMaxLength Maximum number of colors in palette.
 */
void paletteLoadFromMem(const UBYTE *pData , UWORD *pPalette, UBYTE ubMaxLength);


/**
 *  @brief Dims palette to given brightness level.
 *  @param pSource      Pointer to source palette.
 *  @param pDest        Pointer to destination palette. May be same as pSource.
 *  @param ubColorCount Number of colors in palette.
 *  @param ubLevel      Brightness level - 15 for no dim, 0 for total blackness.
 */
void paletteDim(UWORD *pSource, UWORD *pDest, UBYTE ubColorCount, UBYTE ubLevel);

/**
 *  @brief Dims a single input color to given brightness level.
 *  @param uwFullColor Full color used as a base to calculate percentage.
 *  @param ubLevel      Brightness level - 15 for no dim, 0 for total blackness.
 */
UWORD paletteColorDim(UWORD uwFullColor, UBYTE ubLevel);

void paletteDump(UWORD *pPalette, FUBYTE fubColorCnt, char *szPath);

#ifdef __cplusplus
}
#endif

#endif // _ACE_UTILS_PALETTE_H
