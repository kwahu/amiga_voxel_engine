#include "engine.h"


void InitMenuState()
{
    ResetTime();
	engine.currentState = State_Menu;
    
    engine.menuState.infoScreen = 0;
}

void RunMenuState()
{
    LoadBitmapToMemory("data/m0");
    ClearBuffor();
    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
    
    SetBitmapPalette(engine.activePalette);
    VSyncAndDraw();

    UBYTE infoIndex = 0;
    UBYTE FireDown = 0;


    while(!engine.menuState.infoScreen)
    {
        joyProcess();
        if(getJoy(1, FIRE) && !FireDown)
        {
            infoIndex += 1;
            
            switch(infoIndex)
            {
                case 1:
                {
                    LoadBitmapToMemory("data/m1");
                    ClearBuffor();
                    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
                    
                    SetBitmapPalette(engine.activePalette);
                    VSyncAndDraw();

                    FireDown = 1;
                } break;
                case 2:
                {
                    LoadBitmapToMemory("data/m2");
                    ClearBuffor();
                    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
                    
                    SetBitmapPalette(engine.activePalette);
                    VSyncAndDraw();

                    FireDown = 1;
                } break;
                case 3:
                {

                    LoadBitmapToMemory("data/msg");
                    ClearBuffor();
                    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
                    
                    SetBitmapPalette(engine.activePalette);
                    VSyncAndDraw();

                    engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "The ship's Anti-G engine uses the");
                    engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "planet's magnetic field to move");
                    engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "vertically.");
                    engine.pBitmapInfo[3] = CreateBitmapFromText(engine.font, "The closer you get to the surface,");
                    engine.pBitmapInfo[4] = CreateBitmapFromText(engine.font, "the more energy is generated.");
                    engine.pBitmapInfo[5] = CreateBitmapFromText(engine.font, "This energy is converted to power");
                    engine.pBitmapInfo[6] = CreateBitmapFromText(engine.font, "ship's propulsion engines and");
                    engine.pBitmapInfo[7] = CreateBitmapFromText(engine.font, "accelerate it.");
                    engine.pBitmapInfo[8] = CreateBitmapFromText(engine.font, "Remember, the lower you fly,");
                    engine.pBitmapInfo[9] = CreateBitmapFromText(engine.font, "the faster you go.");

                    DrawTextBitmap(engine.pBitmapInfo[0], 100, 70, 4);
                    DrawTextBitmap(engine.pBitmapInfo[1], 100, 76, 4);
                    DrawTextBitmap(engine.pBitmapInfo[2], 100, 82, 4);
                    DrawTextBitmap(engine.pBitmapInfo[3], 100, 88, 4);
                    DrawTextBitmap(engine.pBitmapInfo[4], 100, 94, 4);
                    DrawTextBitmap(engine.pBitmapInfo[5], 100, 100, 4);
                    DrawTextBitmap(engine.pBitmapInfo[6], 100, 106, 4);
                    DrawTextBitmap(engine.pBitmapInfo[7], 100, 112, 4);
                    DrawTextBitmap(engine.pBitmapInfo[8], 100, 118, 4);
                    DrawTextBitmap(engine.pBitmapInfo[9], 100, 124, 4);
                        

                    FireDown = 1;
                } break;
                case 4:
                {
                    for(int i = 0; i < 10; ++i)
                    {
                        FreeTextBitmap(engine.pBitmapInfo[i]);
                    }
                    ClearBuffor();
                    SetGamePaletter();
                    VSyncAndDraw();
                    engine.menuState.infoScreen = 1;
                    
                    

                } break;
            }
        }
        else if(FireDown && !getJoy(1, FIRE))
        {
            FireDown = 0;
        }
        
        if (getKey(ESCAPE))
        {
            engine.menuState.infoScreen = 1;
            engine.exitFlag = 1;
        }
    }

    InitGameState();
}