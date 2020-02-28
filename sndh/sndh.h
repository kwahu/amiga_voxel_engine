/**
 *  @file sndh.h
 *
 *	SNDH player lib for GCC
 *
 *	@author (c) 2010/2014/2019 by Simon Sunnyboy / Paradize <marndt@asmsoftware.de>
 *	http://paradize.atari.org/
 *
 *	@brief   header for SNDH player lib
 *
 *  @copyright
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation; either
 *	version 2.1 of the License, or (at your option) any later version.
 *
 *	This library is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *	Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public
 *	License along with this library; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

/** @addtogroup SNDH
 *  @{
 */

#ifndef __SNDH_H
#define __SNDH_H

#include <stdint.h>

/**
 * @brief SNDH tune object
 */
typedef struct
{
	void *tuneadr;		/**< pointer to unpacked SNDH in memory */
	char *title;		/**< pointer to TITL in SNDH header     */
	char *composer;		/**< pointer to COMM in SNDH header     */
	char *ripper;		/**< pointer to RIPP in SNDH header     */
	char *conv;		    /**< pointer to CONV in SNDH header     */
	uint16_t freq;	    /**< parsed replay frequency            */
} SNDHTune;

/* function prototypes */
void SNDH_GetTuneInfo ( void *sndhdata, SNDHTune *tune_handle );
void SNDH_PlayTune ( SNDHTune *tune_handle, uint16_t nr_subtune );
void SNDH_StopTune ( void );

#endif

/** @} */

