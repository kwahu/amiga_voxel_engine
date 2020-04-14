
#ifdef AMIGA

#include <exec/types.h>
#include "ptplayer.h"

#include <ace/utils/custom.h>




void InitAudio()
{
 
	mt_install_cia(g_pCustom, 0, 1);
    //P61_Init(g_pCustom, engine.music, 0, 1, ((UBYTE *)(&g_pCustom->dmacon)) + 1);
}

void DestroyAudio()
{
}

void PlaySample(UBYTE pos)
{
    
    //P61_Init(custom, music, 0, 1);
    //P61_SetPosition(g_pCustom, pos);
    // mt_init(g_pCustom, engine.music, 0, pos);
  	// mt_Enable = 1;
    //   mt_MusicChannels = 4;
    mt_init(g_pCustom, engine.music, 0, pos);
  	mt_Enable = 1;
      mt_MusicChannels = 4;
    
}
void ContinueSample()
{
    //P61_Init(g_pCustom, engine.music, 0, 1, ((UBYTE *)(&g_pCustom->dmacon)) + 1);
	mt_install_cia(g_pCustom, 0, 1);
  	mt_Enable = 1;
      mt_MusicChannels = 4;
    
}

void StopSample()
{
    //P61_End();
    mt_Enable = 0;
    mt_remove_cia(g_pCustom);

}

void DestroySample()
{
    mt_remove_cia(g_pCustom);
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
    handle = Fopen ( "data/sndh.sndh", 0 );
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