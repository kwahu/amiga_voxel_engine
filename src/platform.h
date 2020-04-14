

void InitEngine(void);

void EngineLoop(void);

void EngineDestroy(void);

#ifdef AMIGA
//#include <ace/generic/main.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <clib/graphics_protos.h>
#include <clib/dos_protos.h>
#include <hardware/intbits.h>
#include <hardware/dmabits.h>
#include <exec/execbase.h>
#include <proto/exec.h>
#include <graphics/gfxbase.h> // Required for GfxBase
#include <hardware/custom.h>
#include <exec/types.h>

#define FAR __far
#define REGPTR volatile * const
#define HWINTERRUPT __attribute__((interrupt))
#define FN_HOTSPOT __attribute__((hot))

typedef struct Custom Custom;

#define CUSTOM_BASE 0xDFF000

Custom FAR REGPTR customRegister = (Custom REGPTR)CUSTOM_BASE;


typedef struct Cia {
	volatile UBYTE pra;
	volatile UBYTE _0[0xff];
	volatile UBYTE prb;
	volatile UBYTE _1[0xff];
	volatile UBYTE ddra;
	volatile UBYTE _2[0xff];
	volatile UBYTE ddrb;
	volatile UBYTE _3[0xff];
	volatile UBYTE talo;
	volatile UBYTE _4[0xff];
	volatile UBYTE tahi;
	volatile UBYTE _5[0xff];
	volatile UBYTE tblo;
	volatile UBYTE _6[0xff];
	volatile UBYTE tbhi;
	volatile UBYTE _7[0xff];
	volatile UBYTE todlow;
	volatile UBYTE _8[0xff];
	volatile UBYTE todmid;
	volatile UBYTE _9[0xff];
	volatile UBYTE todhi;
	volatile UBYTE _a[0xff];
	volatile UBYTE b00;
	volatile UBYTE _b[0xff];
	volatile UBYTE sdr;
	volatile UBYTE _c[0xff];
	volatile UBYTE icr;
	volatile UBYTE _d[0xff];
	volatile UBYTE cra;
	volatile UBYTE _e[0xff];
	volatile UBYTE crb;
	volatile UBYTE _f[0xff];
} Cia;

#define BV(value) (1 << (value))

#define CIAAPRA_OVL  BV(0)
#define CIAAPRA_LED  BV(1)
#define CIAAPRA_CHNG BV(2)
#define CIAAPRA_WPRO BV(3)
#define CIAAPRA_TK0  BV(4)
#define CIAAPRA_RDY  BV(5)
#define CIAAPRA_FIR0 BV(6)
#define CIAAPRA_FIR1 BV(7)

#define CIABPRB_STEP BV(0)
#define CIABPRB_DIR  BV(1)
#define CIABPRB_SIDE BV(2)
#define CIABPRB_SEL0 BV(3)
#define CIABPRB_SEL1 BV(4)
#define CIABPRB_SEL2 BV(5)
#define CIABPRB_SEL3 BV(6)
#define CIABPRB_MTR  BV(7)

#define CIAICR_TIMER_A BV(0)
#define CIAICR_TIMER_B BV(1)
#define CIAICR_TOD     BV(2)
#define CIAICR_SERIAL  BV(3)
#define CIAICR_FLAG    BV(4)
#define CIAICR_SETCLR  BV(7)

#define CIACRA_START   BV(0)
#define CIACRA_PBON    BV(1)
#define CIACRA_OUTMODE BV(2)
#define CIACRA_RUNMODE BV(3)
#define CIACRA_LOAD    BV(4)
#define CIACRA_INMODE  BV(5)
#define CIACRA_SPMODE  BV(6)

#define CIACRB_START   BV(0)
#define CIACRB_PBON    BV(1)
#define CIACRB_OUTMODE BV(2)
#define CIACRB_RUNMODE BV(3)
#define CIACRB_LOAD    BV(4)
#define CIACRB_INMODE  (BV(5) | BV(6))
#define CIACRB_ALARM   BV(7)

Cia FAR REGPTR CiaA = (Cia*)0xBFE001;
Cia FAR REGPTR CiaB = (Cia*)0xBFD000;



typedef void (*HwIntVector)(void);

typedef void (*GameIntHandler)(
	volatile Custom *custom __asm__("a0"), volatile void *Data __asm__("a1")
);

typedef struct GameInterrupt {
	volatile GameIntHandler Handler;
	volatile void *Data;
} GameInterrupt;


const UWORD minimalDma = DMAF_DISK | DMAF_BLITTER;
UBYTE systemUsed = 0;

UWORD osInterruptEnables;
UWORD osDMACon;
UWORD gameDMACon;
UWORD initialDMA;

#define SYSTEM_INT_VECTOR_FIRST (0x64/4)
#define SYSTEM_INT_VECTOR_COUNT 7
#define SYSTEM_INT_HANDLER_COUNT 15


static volatile HwIntVector osHwInterrupts[SYSTEM_INT_VECTOR_COUNT] = {0};
static volatile HwIntVector * hwVectors = 0;
static volatile GameInterrupt gameInterrupts[SYSTEM_INT_HANDLER_COUNT] = {{0}};




FN_HOTSPOT
void HWINTERRUPT Int1Handler(void) {
	// Soft / diskBlk / TBE (RS232 TX end)
}

FN_HOTSPOT
void HWINTERRUPT Int2Handler(void) {
	// Parallel, keyboard, mouse, "some of disk functions"
	UWORD intReq = customRegister->intreqr;
	if(intReq & INTF_PORTS) {
		UBYTE icrA = CiaA->icr; // Read clears interrupt flags
		if(icrA & CIAICR_SERIAL) {
			// Keyboard
			if(gameInterrupts[INTB_PORTS].Handler) {
				gameInterrupts[INTB_PORTS].Handler(
					customRegister, gameInterrupts[INTB_PORTS].Data
				);
			}
		}
		if(icrA & CIAICR_FLAG) {

		}
		if(icrA & CIAICR_TIMER_A) {

		}
		if(icrA & CIAICR_TIMER_B) {

		}
		if(icrA & CIAICR_TOD) {

		}
		// TODO: this could be re-enabled in vblank since we don't need it
		// to retrigger during same frame. Or perhaps it's needed so that kbd
		// controller won't overflow its queue
		customRegister->intreq = INTF_PORTS;
		customRegister->intreq = INTF_PORTS;
	}
}

FN_HOTSPOT
void HWINTERRUPT Int3Handler(void) {
	// VBL / Copper / Blitter
	UWORD uwIntReq = customRegister->intreqr;
	UWORD uwReqClr = 0;

	// Vertical blanking
	if(uwIntReq & INTF_VERTB) {
		// Do ACE-specific stuff
		// TODO when ACE gets ported to C++ this could be constexpr if'ed
		timerOnInterrupt();

		// Process handlers
		if(gameInterrupts[INTB_VERTB].Handler) {
			gameInterrupts[INTB_VERTB].Handler(
				customRegister, gameInterrupts[INTB_VERTB].Data
			);
		}
		uwReqClr = INTF_VERTB;
	}

	// Copper
	if(uwIntReq & INTF_COPER) {
		if(gameInterrupts[INTB_COPER].Handler) {
			gameInterrupts[INTB_COPER].Handler(
				customRegister, gameInterrupts[INTB_VERTB].Data
			);
		}
		uwReqClr |= INTF_COPER;
	}

	// Blitter
	if(uwIntReq & INTF_BLIT) {
		if(gameInterrupts[INTB_BLIT].Handler) {
			gameInterrupts[INTB_BLIT].Handler(
				customRegister, gameInterrupts[INTB_VERTB].Data
			);
		}
		uwReqClr |= INTF_BLIT;
	}
	customRegister->intreq = uwReqClr;
	customRegister->intreq = uwReqClr;
}

FN_HOTSPOT
void HWINTERRUPT Int4Handler(void) {
	UWORD uwIntReq = customRegister->intreqr;
	UWORD uwReqClr = 0;

	// Audio channel 0
	if(uwIntReq & INTF_AUD0) {
		if(gameInterrupts[INTB_AUD0].Handler) {
			gameInterrupts[INTB_AUD0].Handler(
				customRegister, gameInterrupts[INTB_AUD0].Data
			);
		}
		uwReqClr |= INTF_AUD0;
	}

	// Audio channel 1
	if(uwIntReq & INTF_AUD1) {
		if(gameInterrupts[INTB_AUD1].Handler) {
			gameInterrupts[INTB_AUD1].Handler(
				customRegister, gameInterrupts[INTB_AUD1].Data
			);
		}
		uwReqClr |= INTF_AUD1;
	}

	// Audio channel 2
	if(uwIntReq & INTF_AUD2) {
		if(gameInterrupts[INTB_AUD2].Handler) {
			gameInterrupts[INTB_AUD2].Handler(
				customRegister, gameInterrupts[INTB_AUD2].Data
			);
		}
		uwReqClr |= INTF_AUD2;
	}

	// Audio channel 3
	if(uwIntReq & INTF_AUD3) {
		if(gameInterrupts[INTB_AUD3].Handler) {
			gameInterrupts[INTB_AUD3].Handler(
				customRegister, gameInterrupts[INTB_AUD3].Data
			);
		}
		uwReqClr |= INTF_AUD3;
	}
	customRegister->intreq = uwReqClr;
	customRegister->intreq = uwReqClr;
}

FN_HOTSPOT
void HWINTERRUPT Int5Handler(void) {
	// DskSyn / RBF
}

FN_HOTSPOT
void HWINTERRUPT Int6Handler(void) {
	// CIA B
}

FN_HOTSPOT
void HWINTERRUPT Int7Handler(void) {
	// EXTERNAL
}

static const HwIntVector gameHwInterrupts[SYSTEM_INT_VECTOR_COUNT] = {
	Int1Handler, Int2Handler, Int3Handler, Int4Handler,
	Int5Handler, Int6Handler, Int7Handler
};

static void SystemFlushIo() {
	struct StandardPacket *packet = (struct StandardPacket*)AllocMem(
		sizeof(struct StandardPacket), MEMF_CLEAR
	);

	if(!packet) {
		return;
	}

	// Get filesystem message port
	struct MsgPort *msgPort = DeviceProc((CONST_STRPTR)"sys");
	if (msgPort) {
		// Get our message port
		struct Process *process = (struct Process *)FindTask(0);
		struct MsgPort *processMsgPort = &process->pr_MsgPort;

		// Fill in packet
		struct DosPacket *dosPacket = &packet->sp_Pkt;
		struct Message *msg = &packet->sp_Msg;

		// It is how Tripos packet system was hacked to exec messaging system
		msg->mn_Node.ln_Name = (char*)dosPacket;
		msg->mn_ReplyPort = processMsgPort;

		dosPacket->dp_Link = msg;
		dosPacket->dp_Port = processMsgPort;
		dosPacket->dp_Type = ACTION_FLUSH;

		// Send packet
		PutMsg(msgPort, msg);

		// Wait for reply
		WaitPort(processMsgPort);
		GetMsg(processMsgPort);
	}

	FreeMem(packet, sizeof(struct StandardPacket));
}


void SystemUnuse(void) {
	
	if(systemUsed) {
		systemUsed = 0;
		if(customRegister->dmaconr & DMAF_DISK) {
			// Flush disk activity if it was used
			// This 'if' is here because otherwise systemUnuse() called
			// by systemCreate() would indefinitely wait for OS when it's killed.
			// systemUse() restores disk DMA, so it's an easy check if OS was
			// actually restored.
			SystemFlushIo();
		}

		// Disable interrupts (this is the actual "kill system/OS" part)
		customRegister->intena = 0x7FFF;
		customRegister->intreq = 0x7FFF;

		// Game's bitplanes & copperlists are still used so don't disable them
		// Wait for vbl before disabling sprite DMA
		while (!(customRegister->intreqr & INTF_VERTB)) {}
		customRegister->dmacon = minimalDma;

		// Save OS interrupt vectors and enable ACE's
		customRegister->intreq = 0x7FFF;
		for(UWORD i = 0; i < SYSTEM_INT_VECTOR_COUNT; ++i) {
			osHwInterrupts[i] = hwVectors[SYSTEM_INT_VECTOR_FIRST + i];
			hwVectors[SYSTEM_INT_VECTOR_FIRST + i] = gameHwInterrupts[i];
		}

		// Enable needed DMA (and interrupt) channels
		customRegister->dmacon = DMAF_SETCLR | DMAF_MASTER | gameDMACon;
		// Everything that's supported by ACE to simplify things for now
		customRegister->intena = INTF_SETCLR | INTF_INTEN | (
			INTF_BLIT | INTF_COPER | INTF_VERTB |
			INTF_PORTS | INTF_AUD0 | INTF_AUD1 | INTF_AUD2 | INTF_AUD3
		);
	}
}


void SystemUse(void) {
	if(!systemUsed) {
		// Disable app interrupts/dma, keep display-related DMA
		customRegister->intena = 0x7FFF;
		customRegister->intreq = 0x7FFF;
		customRegister->dmacon = minimalDma;
		while (!(customRegister->intreqr & INTF_VERTB)) {}

		// Restore interrupt vectors
		for(UWORD i = 0; i < SYSTEM_INT_VECTOR_COUNT; ++i) {
			hwVectors[SYSTEM_INT_VECTOR_FIRST + i] = osHwInterrupts[i];
		}
		// restore old DMA/INTENA/ADKCON etc. settings
		// All interrupts but only needed DMA
		customRegister->dmacon = DMAF_SETCLR | DMAF_MASTER | (osDMACon & minimalDma);
		customRegister->intena = INTF_SETCLR | INTF_INTEN  | osInterruptEnables;
		systemUsed = 1;
	} 
}

struct GfxBase *gfxBase;

int main(int argc, char **argv)
{
	gfxBase = (struct GfxBase *)OpenLibrary((CONST_STRPTR)"graphics.library", 0L);
	OwnBlitter();
	WaitBlit();

	WaitTOF();
	LoadView(0);
	WaitTOF();
	WaitTOF();
	
	SystemFlushIo();

	// save the state of the hardware registers (INTENA, DMA, ADKCON etc.)
	osInterruptEnables = customRegister->intenar;
	osDMACon = customRegister->dmaconr;
	initialDMA = osDMACon;

	// Disable interrupts (this is the actual "kill system/OS" part)
	customRegister->intena = 0x7FFF;
	customRegister->intreq = 0x7FFF;

	// Disable all DMA - only once
	// Wait for vbl before disabling sprite DMA
	while (!(customRegister->intreqr & INTF_VERTB)) {}
	customRegister->dmacon = 0x07FF;

	// Unuse system so that it gets backed up once and then re-enable
	// as little as needed
	systemUsed = 1;
	SystemUnuse();
	SystemUse();

	int running = 1;
	InitEngine();
	while(running)
	{
		EngineLoop();
	}
	EngineDestroy();
}

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

void main_supervisor() 
{
	InitEngine();
	while(!engine.exitFlag)
	{
		EngineLoop();
	}
	EngineDestroy();
}

int main(int argc, char **argv)
{
   Supexec(&main_supervisor, 0,0,0,0,0);
   return 0;
}
#endif
