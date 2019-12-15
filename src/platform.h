

void engineGsCreate(void);

void engineGsLoop(void);

void engineGsDestroy(void);

#ifdef AMIGA
#include <ace/generic/main.h>
#include <ace/managers/game.h>
#include <ace/managers/timer.h>
#include <ace/managers/system.h>
#include <ace/managers/blit.h>
#include <ace/utils/file.h>
void genericCreate(void)
{
	gamePushState(engineGsCreate, engineGsLoop, engineGsDestroy);
}

void genericProcess(void)
{
	gameProcess();
}

void genericDestroy(void)
{
}
#else
#include "key_map_atari.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <osbind.h>
#include <mint/sysbind.h>

void main_supervisor() 
{
	engineGsCreate();
	engineGsLoop();
	engineGsDestroy();
}

int main(int argc, char **argv)
{
   Supexec(&main_supervisor, 0,0,0,0,0);
   return 0;
}
#endif
