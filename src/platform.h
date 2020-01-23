

void InitEngine(void);

void EngineLoop(void);

void EngineDestroy(void);

#ifdef AMIGA
#include "SDI_compiler.h"
#include "SDI_hook.h"
#include "SDI_interrupt.h"
#include "SDI_lib.h"
#include "SDI_misc.h"
#include "SDI_stdarg.h"
#include <ace/generic/main.h>
void genericCreate(void)
{
	gamePushState(InitEngine, EngineLoop, EngineDestroy);
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
#include <osbind.h>
#include <mint/sysbind.h>

void main_supervisor() 
{
	InitEngine();
	EngineLoop();
	EngineDestroy();
}

int main(int argc, char **argv)
{
   Supexec(&main_supervisor, 0,0,0,0,0);
   return 0;
}
#endif
