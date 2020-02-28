/**
 *  @file sndh.c
 *
 *	SNDH player lib for GCC
 *
 *	@author (c) 2010/2014/2019 by Simon Sunnyboy / Paradize <marndt@asmsoftware.de>
 *	http://paradize.atari.org/
 *
 *	@brief   highlevel SNDH player library routines
 *	@details some parsing algorithms are derived from GODLIB
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

#include "sndh.h"
#include <stdint.h>

#if (!defined NULL)
#define NULL 0
#endif

/* internal variables */
static SNDHTune *SNDH_ActiveTune;	/**< this is not NULL if a tune has been hookedup for play */

/* prototype of lowlevel functions */

/**
 * @brief   configures the Timer C ISR to play the given subtune
 * @details Keyclick is turned off and the existing (system) Timer C ISR is daisychained.
 * @param   sndhdata points to SNDH data in memory
 * @param   freq is the desired replay frequency
 * @param   nr_subtune  (0 ... n-1)
 */
void 		SNDH_PlayTuneISR ( void *sndhdata, uint16_t freq, uint16_t nr_subtune );

/**
 * @brief stops the current ongoing SNDH replay and restores the old ISR
 */
void		SNDH_StopTuneISR ( void );

/* prototype of internal functions */
static char*	SNDH_FindLongInHeader ( char* chunk, char *tag );
static char*	SNDH_FindWordInHeader ( char* chunk, char *tag );
static uint16_t	SNDH_ParseDecimal ( char *chunk );

/* public functions */

/**
 * @brief     get SNDH tune info from file in memory and parse into tune object
 * @details   The replay frequency is determined here.
 * @attention Call once before SNDH_PlayTune!
 * @param     sndhdata points to unpacked SNDH data file in memory
 * @param     tune_handle points to handle for replay
 */
void SNDH_GetTuneInfo ( void *sndhdata, SNDHTune *tune_handle )
{
	char *parser, *freqtag;

	/* assert valid SNDH tune */
	if ( sndhdata == NULL )
	{
		tune_handle = NULL;
		return;
	}

	parser = ( char * ) sndhdata;

	if ( SNDH_FindLongInHeader ( parser, "SNDH" ) != 0 )
	{
		/* SNDH file is valid */
		tune_handle->tuneadr  = sndhdata;
		/* get pointers to SNDH meta data */
		tune_handle->title    = SNDH_FindLongInHeader ( parser, "TITL" );
		tune_handle->composer = SNDH_FindLongInHeader ( parser, "COMM" );
		tune_handle->ripper   = SNDH_FindLongInHeader ( parser, "RIPP" );
		tune_handle->conv     = SNDH_FindLongInHeader ( parser, "CONV" );

		/* try to parse replay frequency */
		if ( ( freqtag = SNDH_FindWordInHeader ( parser,"TC" ) ) )
		{
			/* Timer C tunes have priority in detection */
			tune_handle->freq = SNDH_ParseDecimal ( freqtag );
		}
		else if ( ( freqtag = SNDH_FindWordInHeader ( parser,"TA" ) ) )
		{
			tune_handle->freq = SNDH_ParseDecimal ( freqtag );
		}
		else if ( ( freqtag = SNDH_FindWordInHeader ( parser,"TB" ) ) )
		{
			tune_handle->freq = SNDH_ParseDecimal ( freqtag );
		}
		else if ( ( freqtag = SNDH_FindWordInHeader ( parser,"TD" ) ) )
		{
			tune_handle->freq = SNDH_ParseDecimal ( freqtag );
		}
		else if ( ( freqtag = SNDH_FindWordInHeader ( parser,"V!" ) ) )
		{
			tune_handle->freq = SNDH_ParseDecimal ( freqtag );
		}
		else
		{
            /* unknown replay speed - default to 50Hz as best guess */
			tune_handle->freq = 50;
		}
	}
	else
	{
		tune_handle = NULL;
	}

	return;
}

/**
 * @brief   play subtune from given SNDH file
 * @details Any tune already playing is interrupted.
 * @param   tune_handle points to handle for replay
 * @param   nr_subtune  (0 ... n-1)
 */
void SNDH_PlayTune ( SNDHTune *tune_handle, uint16_t nr_subtune )
{
	if ( tune_handle == NULL )
	{
		/* tune is not valid */
		return;
	}

	/* stop any already playing tune */
	if ( SNDH_ActiveTune != NULL )
	{
		SNDH_StopTune();
	}

	/* start playing if valid data is provided */
	if ( ( tune_handle->tuneadr != NULL ) && ( tune_handle->freq > 0 ) )
	{
		SNDH_ActiveTune = tune_handle;
		SNDH_PlayTuneISR ( tune_handle->tuneadr, tune_handle->freq, nr_subtune );
	}

	return;
}

/**
 * @brief stop any playing SNDH tune
 */
void SNDH_StopTune ( void )
{
	SNDH_ActiveTune = NULL;
	SNDH_StopTuneISR();
	return;
}

/**
 * @brief   helper function to find data string in SNDH file header
 * @details Data is identified by a 32bit LONG preceeding the actual data.
 * @param   chunk points to data in memory to scan (normally start of tune data)
 * @param   tag identifies the data, normally a 4 character string like "TITL", see the SNDH file standard
 * @return  pointer to data string if found, NULL if not found
 */
static char* SNDH_FindLongInHeader ( char* chunk, char *tag )
{
	uint16_t i;

	for ( i=0; i<1024; i++ )
	{
		if (  ( chunk[i]   == tag[ 0 ] )
		        && ( chunk[i+1] == tag[ 1 ] )
		        && ( chunk[i+2] == tag[ 2 ] )
		        && ( chunk[i+3] == tag[ 3 ] )
		   )
		{
			return ( &chunk[i+4] );
		}
	}

	return ( NULL );
}

/**
 * @brief   helper function to find data string in SNDH file header
 * @details Data is identified by a 16bit WORD preceeding the actual data.
 * @param   chunk points to data in memory to scan (normally start of tune data)
 * @param   tag identifies the data, normally a 4 character string like "FR", see the SNDH file standard
 * @return  pointer to data string if found, NULL if not found
 */
static char* SNDH_FindWordInHeader ( char* chunk, char *tag )
{
	uint16_t i;

	for ( i=0; i<1024; i++ )
	{
		if (  ( chunk[i]   == tag[ 0 ] )
		        && ( chunk[i+1] == tag[ 1 ] )
		   )
		{
			return ( &chunk[i+2] );
		}
	}

	return ( NULL );
}

/**
 * @brief     parses an unsigned 16bit decimal from given ascii chunk in memory
 * @details   Data is scanned until ASCII NUL is detected.
 * @param     chunk points to string in memory, e.q. "125"
 * @return    converted decimal
 * @attention Overflow of the 16bit value is not checked.
 */
static uint16_t SNDH_ParseDecimal ( char *chunk )
{
	uint16_t value = 0;

	while ( *chunk != 0 )
	{
		value *= 10;
		value += ( *chunk++ ) - '0';
	}

	return ( value );
}

/** @} */
