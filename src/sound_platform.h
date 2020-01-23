
#ifdef AMIGA

#include <exec/types.h>
#include "ptplayer.h"

//#include <ace/managers/audio.h>
#include <ace/utils/custom.h>
//#include <ace/utils/ptplayer.h>
//#include "SDI_compiler.h"
//#include "p61.h"



void Play()
{
    //loadsong(0xdff000);
}


UBYTE *ReadModFile(char *fileName)
{
    tFile *file = fileOpen(fileName, "rb");

    ULONG fileSize = fileGetSize(fileName);

    systemUse();
    UBYTE *fileMem = (UBYTE *)malloc(58360);
    systemUnuse();
    fileRead(file, fileMem, 58360);
    fileClose(file);

    return fileMem;
}

void InitAudio()
{
    //mt_install_cia(0,1);
}

void DestroyAudio()
{
}

void PlaySample(void *module, UBYTE pos)
{
    
    
    //mt_init(module, 0, pos);
    //mt_Enable = 1;
    //mt_MusicChannels = 4;
   // mt_music();
}

void StopSample(UBYTE channelIndex)
{
}

void DestroySample(UBYTE *sample)
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

void StopSample(UBYTE channelIndex)
{
}

void DestroySample(AudioSample *sample)
{
}

#endif