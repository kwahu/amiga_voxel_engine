

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
#endif
