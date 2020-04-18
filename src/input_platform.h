#pragma once
 #ifdef AMIGA


#define JPORT1 1
#define JPORT2 2

#define JOY1_FIRE 0
#define JOY1_UP 1
#define JOY1_DOWN 2
#define JOY1_LEFT 3
#define JOY1_RIGHT 4

#define JOY2_FIRE 5
#define JOY2_UP 6
#define JOY2_DOWN 7
#define JOY2_LEFT 8
#define JOY2_RIGHT 9

#define JOY3_FIRE 10
#define JOY3_UP 11
#define JOY3_DOWN 12
#define JOY3_LEFT 13
#define JOY3_RIGHT 14

#define JOY4_FIRE 15
#define JOY4_UP 16
#define JOY4_DOWN 17
#define JOY4_LEFT 18
#define JOY4_RIGHT 19

// Combined access: JOYn + JOY_action
#define JOY_FIRE 0
#define JOY_UP 1
#define JOY_DOWN 2
#define JOY_LEFT 3
#define JOY_RIGHT 4

#define JOY1 0
#define JOY2 5
#define JOY3 10
#define JOY4 15

#define JOY_NACTIVE 0
#define JOY_USED 1
#define JOY_ACTIVE 2



typedef struct {
	UBYTE states[20];
} JoyManager;

/* Globals */
JoyManager joyManager;
static UBYTE is4joy = 0;

void SetJoystickState(UBYTE joyCode, UBYTE joyState) {
	joyManager.states[joyCode] = joyState;
}

UBYTE CheckJoystick(UBYTE joyCode) {
	return joyManager.states[joyCode] != JOY_NACTIVE;
}

void OpenJoystick(UBYTE joy) {
	is4joy = joy;
// #if defined(CONFIG_SYSTEM_OS_FRIENDLY)
// 	if(is4joy) {
// 		// Open misc.resource for 3rd and 4th joy connected to parallel port
// 		static const char *szOwner = "ACE joy manager";
// 		MiscBase = (struct Library*)OpenResource(MISCNAME);
// 		if(!MiscBase) {
// 			logWrite("ERR: Couldn't open '%s'\n", MISCNAME);
// 			return;
// 		}

// 		// Are data/status line currently used?
// 		char *currentOwner;
// 		currentOwner = AllocMiscResource(MR_PARALLELPORT, owner);
// 		if(currentOwner) {
// 			return;
// 		}
// 		currentOwner = AllocMiscResource(MR_PARALLELBITS, owner);
// 		if(currentOwner) {
// 			// Free what was already allocated
// 			FreeMiscResource(MR_PARALLELPORT);
// 		}

// 		// Save old DDR & value regs
// 		oldDataDdr = CiaA->ddrb;
// 		oldStatusDdr = CiaB->ddra;
// 		oldDataVal = CiaA->prb;
// 		oldStatusVal = CiaB->pra;

// 		// Set data direction register to input. Status lines are as follows:
// 		// bit 0: BUSY
// 		// bit 2: SEL
// 		CiaB->ddra |= BV(0) | BV(2); // Status lines DDR
// 		CiaA->ddrb = 0xFF; // Data lines DDR

// 		CiaB->pra &= 0xFF^(BV(0) | BV(2)); // Status lines values
// 		CiaA->prb = 0; // Data lines values
// 	}
// #endif
}
// F-keys
#define KEY_F1 0x50
#define KEY_F2 0x51
#define KEY_F3 0x52
#define KEY_F4 0x53
#define KEY_F5 0x54
#define KEY_F6 0x55
#define KEY_F7 0x56
#define KEY_F8 0x57
#define KEY_F9 0x58
#define KEY_F10 0x59

// Digits
#define KEY_1 0x01
#define KEY_2 0x02
#define KEY_3 0x03
#define KEY_4 0x04
#define KEY_5 0x05
#define KEY_6 0x06
#define KEY_7 0x07
#define KEY_8 0x08
#define KEY_9 0x09
#define KEY_0 0x0A

// Chars - row 1
#define KEY_Q 0x10
#define KEY_W 0x11
#define KEY_E 0x12
#define KEY_R 0x13
#define KEY_T 0x14
#define KEY_Y 0x15
#define KEY_U 0x16
#define KEY_I 0x17
#define KEY_O 0x18
#define KEY_P 0x19

// Chars - row 2
#define KEY_A 0x20
#define KEY_S 0x21
#define KEY_D 0x22
#define KEY_F 0x23
#define KEY_G 0x24
#define KEY_H 0x25
#define KEY_J 0x26
#define KEY_K 0x27
#define KEY_L 0x28

// Chars - row 3
#define KEY_Z 0x31
#define KEY_X 0x32
#define KEY_C 0x33
#define KEY_V 0x34
#define KEY_B 0x35
#define KEY_N 0x36
#define KEY_M 0x37

// Numpad: digits
#define KEY_NUM0 0x0F
#define KEY_NUM1 0x1D
#define KEY_NUM2 0x1E
#define KEY_NUM3 0x1F
#define KEY_NUM4 0x2D
#define KEY_NUM5 0x2E
#define KEY_NUM6 0x2F
#define KEY_NUM7 0x3D
#define KEY_NUM8 0x3E
#define KEY_NUM9 0x3F

// Arrows
#define KEY_UP 0x4C
#define KEY_DOWN 0x4D
#define KEY_RIGHT 0x4E
#define KEY_LEFT 0x4F

// Numpad: ()/*+-. and Enter
#define KEY_NUMLPARENTHESES 0x5A
#define KEY_NUMRPARENTHESES 0x5B
#define KEY_NUMSLASH 0x5C
#define KEY_NUMMULTIPLY 0x5D
#define KEY_NUMPLUS 0x5E
#define KEY_NUMMINUS 0x4A
#define KEY_NUMPERIOD 0x3C
#define KEY_NUMENTER 0x43

// Misc
#define KEY_ESCAPE 0x45
#define KEY_ACCENT 0x00
#define KEY_MINUS 0x0B
#define KEY_EQUALS 0x0C
#define KEY_BACKSLASH 0x0D
#define KEY_TAB 0x42
#define KEY_LBRACKET 0x1A
#define KEY_RBRACKET 0x1B
#define KEY_RETURN 0x44
#define KEY_CONTROL 0x63
#define KEY_CAPSLOCK 0x62
#define KEY_SEMICOLON 0x29
#define KEY_APOSTROPHE 0x2A
#define KEY_REGIONAL1 0x2B
#define KEY_LSHIFT 0x60
#define KEY_REGIONAL2 0x30
#define KEY_COMMA 0x38
#define KEY_PERIOD 0x39
#define KEY_BACKSPACE 0x41
#define KEY_SLASH 0x3A
#define KEY_RSHIFT 0x61
#define KEY_LALT 0x64
#define KEY_LAMIGA 0x66
#define KEY_SPACE 0x40
#define KEY_RAMIGA 0x67
#define KEY_RALT 0x64
#define KEY_DEL 0x46
#define KEY_HELP 0x5F

// Key state flags
#define KEY_NACTIVE 0
#define KEY_USED 1
#define KEY_ACTIVE 2
#define KEY_RELEASED_BIT 1


typedef struct {
	UBYTE states[103];
	UBYTE lastKey;
} KeyManager;

KeyManager keyManager;


static inline UBYTE IntCheckKey(const KeyManager *manager, UBYTE keyCode) {
	return manager->states[keyCode] != KEY_NACTIVE;
}

UBYTE CheckKey(UBYTE keyCode) {
	return IntCheckKey(&keyManager, keyCode);
}

#define getJoy(index, button) CheckJoystick(JOY ##index## _ ##button)
#define getKey(name) CheckKey(KEY_ ##name)



void ProcessJoystick()
{

	UBYTE ciaAPra = CiaA->pra;
	UWORD joyDataPort1 = customRegister->joy0dat;
	UWORD joyDataPort2 = customRegister->joy1dat;

	UWORD joyLookup[] = {
		!BTST(ciaAPra, 7),                           // Joy 1 fire  (PORT 2)
		BTST(joyDataPort2 >> 1 ^ joyDataPort2, 8), // Joy 1 up    (PORT 2)
		BTST(joyDataPort2 >> 1 ^ joyDataPort2, 0), // Joy 1 down  (PORT 2)
		BTST(joyDataPort2, 9),                       // Joy 1 left  (PORT 2)
		BTST(joyDataPort2, 1),                       // Joy 1 right (PORT 2)

		!BTST(ciaAPra, 6),                           // Joy 2 fire  (PORT 1)
		BTST(joyDataPort1 >> 1 ^ joyDataPort1, 8), // Joy 2 up    (PORT 1)
		BTST(joyDataPort1 >> 1 ^ joyDataPort1, 0), // Joy 2 down  (PORT 1)
		BTST(joyDataPort1, 9),                       // Joy 2 left  (PORT 1)
		BTST(joyDataPort1, 1),						           // Joy 2 right (PORT 1)
	};

	UBYTE joyCode;
	if(is4joy) {
		joyCode = 20;
		UBYTE parData = CiaA->prb;
		UBYTE parStatus = CiaB->pra;

		joyLookup[10] = !BTST(parStatus, 2); // Joy 3 fire
		joyLookup[11] = !BTST(parData, 0);   // Joy 3 up
		joyLookup[12] = !BTST(parData, 1);   // Joy 3 down
		joyLookup[13] = !BTST(parData, 2);   // Joy 3 left
		joyLookup[14] = !BTST(parData, 3);   // Joy 3 right

		joyLookup[15] = !BTST(parStatus , 0); // Joy 4 fire
		joyLookup[16] = !BTST(parData , 4);   // Joy 4 up
		joyLookup[17] = !BTST(parData , 5);   // Joy 4 down
		joyLookup[18] = !BTST(parData , 6);   // Joy 4 left
		joyLookup[19] = !BTST(parData , 7);   // Joy 4 right
	}
	else {
		joyCode = 10;
	}
	while (joyCode--) {
		if (joyLookup[joyCode]) {
			if (joyManager.states[joyCode] == JOY_NACTIVE) {
				SetJoystickState(joyCode, JOY_ACTIVE);
			}
		}
		else {
			SetJoystickState(joyCode, JOY_NACTIVE);
		}
	}

}


void CloseJoystick()
{
	CloseJoystick();
}


static inline void KeyIntSetState(
	KeyManager *manager, UBYTE keyCode, UBYTE keyState
) {
	manager->states[keyCode] = keyState;
	if(keyState == KEY_ACTIVE) {
		manager->lastKey = keyCode;
	}
}

FN_HOTSPOT
void INTERRUPT KeyIntServer(
	REGARG(volatile Custom *custom, "a0"),
	REGARG(volatile void *data, "a1")
) {
	KeyManager *kManager = (KeyManager*)data;
	volatile RayPos *rayPos = (RayPos*)&custom->vposr;

	// Get key code and start handshake
	UBYTE keyCode = ~CiaA->sdr;
	CiaA->cra |= CIACRA_SPMODE;
	UWORD start = rayPos->posY;

	// Get keypress flag and shift key code
	UBYTE keyReleased = keyCode & KEY_RELEASED_BIT;
	keyCode >>= 1;
	KeyIntSetState(
		kManager, keyCode, keyReleased ? KEY_NACTIVE : KEY_ACTIVE
	);

	// End handshake
	UWORD delta;
	do {
		UWORD end = rayPos->posY;
		if(end > start) {
			delta = end - start;
		}
		else {
			delta = 0xFFFF - start + end;
		}
	} while(delta < 3);
	CiaA->cra &= ~CIACRA_SPMODE;
	INTERRUPT_END;
}



void TimerOnInterrupt(void) {
	++timerManager.frameCounter;
}

ULONG TimerGetPrec(void) {
	UWORD fr1, fr2; // frame counts
	RayPos rayP;
	ULONG *ray = (ULONG*)&rayP, *reg = (ULONG*)rayPos;

	// There are 4 cases how measurments may take place:
	// a) uwFr1, pRay, uwFr2 on frame A
	// b) uwFr1, pRay on frame A; uwFr2 on frame B
	// c) uwFr1 on frame A; pRay, uwFr2 on frame B
	// d) uwFr2, pRay, uwFr2 on frame B
	// So if pRay took place at low Y pos, it must be on frame B so use uwFr2,
	// Otherwise, pRay took place on A, so use uwFr1
	fr1 = timerManager.frameCounter;
	*ray = *reg;
	fr2 = timerManager.frameCounter;
	if(rayP.posY < 100) {
		return (fr2*160*313 + rayP.posY*160 + rayP.posX);
	}
	else {
		return (fr1*160*313 + rayP.posY*160 + rayP.posX);
	}
}



void InitInput()
{
	
	SystemSetInt(INTB_PORTS, KeyIntServer, &keyManager);

	OpenJoystick(0);
}

void UseSystem()
{
	SystemUse();
}
void UnuseSystem()
{
	SystemUnuse();
}

void ExitGame()
{
	//CloseGame();
}

void GetPlayerRendererSetting()
{
	engine.renderer.zStart = 12;
	if (getKey(3))
	{
		engine.renderer.renderingDepth = 24;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 4;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		if(engine.renderer.highMemory)
		{
		engine.renderer.turnDenom = 3000;
			engine.renderer.xFOV = 20;
			engine.renderer.yFOV = 30;
			engine.renderer.renderingType = 4;
		RecalculateOdd();
		}
		else
		{
		engine.renderer.turnDenom = 4500;
			engine.renderer.xFOV = 23;
			engine.renderer.yFOV = 15;
			engine.renderer.renderingType = 1;
			RecalculateEven();
		}
		
	}
	if (getKey(4)) //A1200
	{
		engine.renderer.renderingDepth = 32;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 3;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		if(engine.renderer.highMemory)
		{
		engine.renderer.turnDenom = 3000;
			engine.renderer.xFOV = 12;
			engine.renderer.yFOV = 20;
			engine.renderer.renderingType = 5;
		RecalculateOdd();
		}
		else
		{
		engine.renderer.turnDenom = 4500;
			engine.renderer.xFOV = 18;
			engine.renderer.yFOV = 12;
			engine.renderer.renderingType = 2;
			RecalculateEven();
		}
	}
	if (getKey(5)) //A3000
	{
		engine.renderer.renderingDepth = 64;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		if(engine.renderer.highMemory)
		{
		engine.renderer.turnDenom = 3000;
		engine.renderer.xFOV = 8;
		engine.renderer.yFOV = 18;
			engine.renderer.renderingType = 6;
		RecalculateOdd();
		}
		else
		{
		engine.renderer.turnDenom = 4500;
			engine.renderer.xFOV = 18;
			engine.renderer.yFOV = 10;
			engine.renderer.renderingType = 3;
			RecalculateEven();	
		}
	}
}

// void ProcessQualityInput()
// {

// 	if (getKey(1) && engine.renderer.renderingType != 1)
// 	{
// 		engine.renderer.renderingDepth = 16;
// 		engine.renderer.renderingType = 1;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 2;
// 		engine.renderer.stepModifier = 16;
// 		engine.renderer.xFOV = 32;
// 		engine.renderer.yFOV = 12;
// 		ClearBuffor();
// 	 	RecalculateEven();
// 	}
// 	if (getKey(2) && engine.renderer.renderingType != 2)
// 	{
// 		engine.renderer.renderingDepth = 16;
// 		engine.renderer.renderingType = 2;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 3;
// 		engine.renderer.renderingDepthStep = 2;
// 		engine.renderer.stepModifier = 16;
// 		engine.renderer.xFOV = 28;
// 		engine.renderer.yFOV = 12;
// 		ClearBuffor();
// 		RecalculateEven();
// 	}
// 	if (getKey(3) && engine.renderer.renderingType != 3)
// 	{
// 		engine.renderer.renderingDepth = 16;
// 		engine.renderer.renderingType = 3;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 2;
// 		engine.renderer.stepModifier = 16;
// 		engine.renderer.xFOV = 24;
// 		engine.renderer.yFOV = 12;
// 		ClearBuffor();
// 		RecalculateOdd();
// 	}
// 	if (getKey(4) && engine.renderer.renderingType != 4)
// 	{
// 		engine.renderer.renderingDepth = 32;
// 		engine.renderer.renderingType = 4;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 		engine.renderer.stepModifier = 16;
// 		engine.renderer.xFOV = 20;
// 		engine.renderer.yFOV = 24;
// 		ClearBuffor();
// 		RecalculateOdd();
// 	}
// 	if (getKey(5) && engine.renderer.renderingType != 5)
// 	{
// 		engine.renderer.renderingDepth = 32;
// 		engine.renderer.renderingType = 5;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		RecalculateOdd();
// 	}
// 	if (getKey(6) && engine.renderer.renderingType != 6)
// 	{
// 		engine.renderer.renderingDepth = 64;
// 		engine.renderer.renderingType = 6;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 		engine.renderer.stepModifier = 16;
// 		engine.renderer.xFOV = 10;
// 		engine.renderer.yFOV = 18;
// 		ClearBuffor();
// 		RecalculateOdd();
// 	}
// 	if (getKey(7) && engine.renderer.renderingType != 7 )
// 	{
// 		engine.renderer.renderingDepth = 64;
// 		engine.renderer.renderingType = 7;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		RecalculateOdd();
// 	}
// 	if (getKey(8) && engine.renderer.renderingType != 8 )
// 	{
// 		engine.renderer.renderingDepth = 64;
// 		engine.renderer.renderingType = 8;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		RecalculateOdd();
// 	}

// }

 #else

#define _hz_200 *(volatile long *)0x4ba

#define TimerGetPrec() _hz_200*12500;

#include "key_map_atari.h"
#include "../ikbd/ikbd.h"

#define getJoy(index, button) (IKBD_STICK ##index & IKBD_JOY_ ##button) ||IKBD_Keyboard[KEY_ ##button ]
#define getKey(name) IKBD_Keyboard[KEY_ ##name ]


void ProcessJoystick()
{
}


void CloseJoystick()
{
	IKBD_Flush();

	IKBD_Uninstall();
}

void UseSystem()
{
}
void UnuseSystem()
{
}

void ExitGame()
{
}


void InitInput()
{
	IKBD_Install();
}

void GetPlayerRendererSetting()
{
	engine.renderer.zStart = 12;
	if (getKey(3))
	{
		engine.renderer.renderingDepth = TERRAINDEPTH;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		if(engine.renderer.highMemory)
		{
		engine.renderer.turnDenom = 3000;
		engine.renderer.xFOV = 26;
		engine.renderer.yFOV = 22;
			engine.renderer.renderingType = 3;
		RecalculateOdd();
		}
		else
		{
		engine.renderer.turnDenom = 4500;
		engine.renderer.xFOV = 26;
		engine.renderer.yFOV = 22;
			engine.renderer.renderingType = 1;
			RecalculateEven();
		}
	}
	if (getKey(4)) 
	{
		engine.renderer.renderingDepth = TERRAINDEPTH;
		engine.renderer.calculationDepthDivider = 2;
		engine.renderer.calculationDepthStep = 2;
		engine.renderer.renderingDepthStep = 1;
		engine.renderer.lastOverwrittenLine = 0;

		engine.renderer.stepModifier = 16;
		if(engine.renderer.highMemory)
		{
		engine.renderer.turnDenom = 3000;
		engine.renderer.xFOV = 24;
		engine.renderer.yFOV = 22;
			engine.renderer.renderingType = 4;
		RecalculateOdd();
		}
		else
		{
		engine.renderer.turnDenom = 4500;
		engine.renderer.xFOV = 24;
		engine.renderer.yFOV = 22;
			engine.renderer.renderingType = 2;
			RecalculateEven();
		}
	}
}

// void ProcessQualityInput()
// {
	

// 	if(getKey(1) && engine.renderer.renderingType!=1)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 1;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if(getKey(2) && engine.renderer.renderingType!=2)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 2;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if(getKey(3) && engine.renderer.renderingType!=3)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 3;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if(getKey(4)  && engine.renderer.renderingType!=4)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 4;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if(getKey(5) && engine.renderer.renderingType!=5)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 5;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if(getKey(6) && engine.renderer.renderingType!=6)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 6;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 4;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if(getKey(7) && engine.renderer.renderingType!=7)
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 7;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 		ClearBuffor();
// 		//DrawBitmap8b(bitmap1, &bitmapHeader1);
// 	}
// 	if (getKey(8) && engine.renderer.renderingType!=8 )
// 	{
// 		engine.renderer.renderingDepth = TERRAINDEPTH;
// 		engine.renderer.renderingType = 8;
// 		engine.renderer.calculationDepthDivider = 2;
// 		engine.renderer.calculationDepthStep = 2;
// 		engine.renderer.renderingDepthStep = 1;
// 	}


// }

#endif

void GetYAxisSetting()
{
	if (getKey(9))
	{
		engine.yAxis = -1;
	}
	if (getKey(0))
	{
		engine.yAxis = 1;
	}
	
}

#if AMIGA
void GetPlayerMemorySetting()
{
	if(getKey(1))
	{
		engine.renderer.depthBufferHeight = YSIZEEVEN;
		engine.renderer.depthBufferWidth = 20;
		#ifdef AMIGA
		engine.renderer.shadowStep = 4;
		#else
		engine.renderer.shadowStep = 8;
		#endif

		ULONG depthBufferSize = engine.renderer.depthBufferWidth*YSIZEEVEN*sizeof(UBYTE);
		ULONG ditherTableSize = COLORS*COLORS*sizeof(UBYTE);
		ULONG screenPatchSize = 4*45*sizeof(UBYTE);
		ULONG rayCastXSize = XSIZEEVEN*TERRAINDEPTH*sizeof(WORD);
		ULONG rayCastYSize = YSIZEEVEN*TERRAINDEPTH*sizeof(WORD);

		NewArena(&engine.rendererArena, depthBufferSize + 4*ditherTableSize + screenPatchSize + rayCastXSize + rayCastYSize);


		engine.renderer.depthBuffer = (UBYTE *)AllocateFromArena(&engine.rendererArena, depthBufferSize);
		engine.renderer.ditherTable1 = (UBYTE *)AllocateFromArena(&engine.rendererArena, ditherTableSize);
		engine.renderer.ditherTable2 = (UBYTE *)AllocateFromArena(&engine.rendererArena, ditherTableSize);
		engine.renderer.ditherTable3 = (UBYTE *)AllocateFromArena(&engine.rendererArena, ditherTableSize);
		engine.renderer.ditherTable4 = (UBYTE *)AllocateFromArena(&engine.rendererArena, ditherTableSize);
		engine.renderer.screenPatch = (UBYTE *)AllocateFromArena(&engine.rendererArena, screenPatchSize);
		engine.renderer.rayCastX = (WORD *)AllocateFromArena(&engine.rendererArena, rayCastXSize);
		engine.renderer.rayCastY = (WORD *)AllocateFromArena(&engine.rendererArena, rayCastYSize);
		engine.renderer.highMemory = 0;
		GenerateColorBytesNoDither4x4();
	}
	if(getKey(2))
	{	
		engine.renderer.depthBufferHeight = YSIZEODD;
		engine.renderer.depthBufferWidth = 20;
		#ifdef AMIGA
		engine.renderer.shadowStep = 2;
		#else
		engine.renderer.shadowStep = 4;
		#endif

		ULONG depthBufferSize = engine.renderer.depthBufferWidth*YSIZEODD*sizeof(UBYTE);
		ULONG ditherTableSize = COLORS*COLORS*COLORS*sizeof(UBYTE);
		ULONG screenPatchSize = 6*90*sizeof(UBYTE);
		ULONG rayCastXSize = XSIZEODD*TERRAINDEPTH*sizeof(WORD);
		ULONG rayCastYSize = YSIZEODD*TERRAINDEPTH*sizeof(WORD);

		NewArena(&engine.rendererArena, depthBufferSize + 4*ditherTableSize + screenPatchSize + rayCastXSize + rayCastYSize);

		engine.renderer.depthBuffer = (UBYTE *)AllocateFromArena(&engine.rendererArena, depthBufferSize);
		engine.renderer.ditherTable1 = (UBYTE *)AllocateFromArena(&engine.rendererArena, ditherTableSize);
		engine.renderer.ditherTable2 = (UBYTE *)AllocateFromArena(&engine.rendererArena, ditherTableSize);
		engine.renderer.ditherTable3 = (UBYTE *)AllocateFromArena(&engine.rendererArena, ditherTableSize);
		engine.renderer.ditherTable4 = (UBYTE *)AllocateFromArena(&engine.rendererArena, ditherTableSize);
		engine.renderer.screenPatch = (UBYTE *)AllocateFromArena(&engine.rendererArena, screenPatchSize);
		engine.renderer.rayCastX = (WORD *)AllocateFromArena(&engine.rendererArena, rayCastXSize);
		engine.renderer.rayCastY = (WORD *)AllocateFromArena(&engine.rendererArena, rayCastYSize);
		engine.renderer.highMemory = 1;
		GenerateColorBytesDither3x2();
	}
}
#else
void GetPlayerMemorySetting()
{
	if(getKey(1))
	{
		engine.renderer.depthBufferHeight = YSIZEEVEN;
		engine.renderer.depthBufferWidth = 20;
		#ifdef AMIGA
		engine.renderer.shadowStep = 4;
		#else
		engine.renderer.shadowStep = 8;
		#endif
		engine.renderer.depthBuffer = (UBYTE *)malloc(engine.renderer.depthBufferWidth*YSIZEEVEN*sizeof(UBYTE));
		engine.renderer.ditherTable1 = (UBYTE *)malloc(4*COLORS*COLORS*sizeof(UBYTE));
		engine.renderer.ditherTable2 = engine.renderer.ditherTable1 + COLORS*COLORS;
		engine.renderer.ditherTable3 = engine.renderer.ditherTable1 + 2*COLORS*COLORS;
		engine.renderer.ditherTable4 = engine.renderer.ditherTable1 + 3*COLORS*COLORS;
		engine.renderer.screenPatch = (UBYTE *)malloc(4*45*sizeof(UBYTE));
		engine.renderer.rayCastX = (WORD *)malloc(XSIZEEVEN*TERRAINDEPTH*sizeof(WORD));
		engine.renderer.rayCastY = (WORD *)malloc(YSIZEEVEN*TERRAINDEPTH*sizeof(WORD));
		engine.renderer.highMemory = 0;
		GenerateColorBytesNoDither4x4();
	}
	if(getKey(2))
	{	
		engine.renderer.depthBufferHeight = YSIZEODD;
		engine.renderer.depthBufferWidth = 20;
		#ifdef AMIGA
		engine.renderer.shadowStep = 2;
		#else
		engine.renderer.shadowStep = 4;
		#endif
		engine.renderer.depthBuffer = (UBYTE *)malloc(engine.renderer.depthBufferWidth*YSIZEODD*sizeof(UBYTE));
		engine.renderer.ditherTable1 = (UBYTE *)malloc(4*COLORS*COLORS*COLORS*sizeof(UBYTE));
		engine.renderer.ditherTable2 = engine.renderer.ditherTable1 + COLORS*COLORS*COLORS;
		engine.renderer.ditherTable3 = engine.renderer.ditherTable1 + 2*COLORS*COLORS*COLORS;
		engine.renderer.ditherTable4 = engine.renderer.ditherTable1 + 3*COLORS*COLORS*COLORS;
		engine.renderer.screenPatch = (UBYTE *)malloc(6*90*sizeof(UBYTE));
		engine.renderer.rayCastX = (WORD *)malloc(XSIZEODD*TERRAINDEPTH*sizeof(WORD));
		engine.renderer.rayCastY = (WORD *)malloc(YSIZEODD*TERRAINDEPTH*sizeof(WORD));
		engine.renderer.highMemory = 1;
		GenerateColorBytesDither3x2();
	}
}
#endif

#define getCurrentTime() TimerGetPrec()
