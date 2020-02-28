
#ifdef AMIGA

#include <exec/types.h>
#include "ptplayer.h"

#include <ace/utils/custom.h>

CHIP const unsigned char music[168660];


void ReadModFile(char *fileName)
{
    tFile *file = fileOpen(fileName, "rb");

    ULONG fileSize = fileGetSize(fileName);

    fileRead(file, music, fileSize);
    fileClose(file);

}

void InitAudio()
{
 
	mt_install_cia(g_pCustom, 0, 1);

}

void DestroyAudio()
{
}

void PlaySample(UBYTE pos)
{
    mt_init(g_pCustom, music, 0, pos);
  	mt_Enable = 1;
    
}
void ContinueSample()
{
	mt_install_cia(g_pCustom, 0, 1);
  	mt_Enable = 1;
    
}

void StopSample()
{
    mt_remove_cia(g_pCustom);
}

void DestroySample()
{
}

#else

#include <sndh.h>
UBYTE ramdisk[20000];

SNDHTune mytune;
UBYTE* tuneptr;

UBYTE *ReadModFile(char *fileName)
{

    tuneptr = &ramdisk[0];
    long handle;
    handle = Fopen ( "heroques.snd", 0 );
    Fseek ( 0,handle, 0 );
    Fread ( handle, 20000, tuneptr );
    Fclose ( handle );
 
    return 0;
}

void InitAudio()
{
}
void DestroyAudio()
{
}

void PlaySample(UBYTE pos)
{
    
    SNDH_GetTuneInfo ( tuneptr,&mytune );
    
    SNDH_PlayTune ( &mytune,0 );   
}

void ContinueSample()
{
}
void StopSample()
{
    
        SNDH_StopTune();
}

void DestroySample()
{
}

#endif