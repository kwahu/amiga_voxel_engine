
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


UBYTE *ReadModFile(char *fileName)
{
    return 0;
}

void InitAudio()
{
}
void DestroyAudio()
{
}

void PlaySample(AudioSample *sample, UBYTE channelIndex, UBYTE volume)
{
    
}

void StopSample()
{
}

void DestroySample()
{
}

#endif