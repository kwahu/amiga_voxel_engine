#include "engine.h"


void InitMenuState()
{
    ResetTime();
	engine.currentState = State_Menu;
    
    engine.menuState.infoScreen = 0;
}

void RunMenuState()
{
    LoadBitmapToMemory(
        #ifdef AMIGA
        "data/m0"
        #else
        "data_a/i1"
        #endif
        );
    ClearBuffor();
    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
    
    SetBitmapPalette(engine.activePalette);
    VSyncAndDraw();

    UBYTE infoIndex = 0;
    UBYTE FireDown = 0;


    while(!engine.menuState.infoScreen)
    {
        ProcessJoystick();
        if(getJoy(1, FIRE) && !FireDown)
        {
            infoIndex += 1;
            
            switch(infoIndex)
            {
                case 1:
                {
                    LoadBitmapToMemory(
                        #ifdef AMIGA
                        "data/m1"
                        #else
                        "data_a/i2"
                        #endif
                        );
                    ClearBuffor();
                    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
                    
                    SetBitmapPalette(engine.activePalette);
                    VSyncAndDraw();
                    FillTextBitmap(engine.font, engine.pBitmapInfo[0], "After setting off the Earth in enormous");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[1], "Generation ships, humanity reached the nearest");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[2], "star system and colonized it's only suitable");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[3], "planet, renaming it The Kingdom.");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[4], "The harsh evironment was one problem, the");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[5], "ideological differences were the other.");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[6], "People of The Kingdom fell prey to their nature");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[7], "which pushed them to the open conflict");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[8], "between The Reign, and The Revolt.");

                    DrawTextBitmap(engine.pBitmapInfo[0], 2, 30, 14);
                    DrawTextBitmap(engine.pBitmapInfo[1], 2, 36, 14);
                    DrawTextBitmap(engine.pBitmapInfo[2], 2, 42, 14);
                    DrawTextBitmap(engine.pBitmapInfo[3], 2, 48, 14);
                    DrawTextBitmap(engine.pBitmapInfo[4], 2, 54, 14);
                    DrawTextBitmap(engine.pBitmapInfo[5], 2, 60, 14);
                    DrawTextBitmap(engine.pBitmapInfo[6], 2, 66, 14);
                    DrawTextBitmap(engine.pBitmapInfo[7], 2, 72, 14);
                    DrawTextBitmap(engine.pBitmapInfo[8], 2, 78, 14);
                    FireDown = 1;
                } break;
                case 2:
                {
                    LoadBitmapToMemory(
                        #ifdef AMIGA
                        "data/m1"
                        #else
                        "data_a/i3"
                        #endif
                    );
                    ClearBuffor();
                    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
                    
                    SetBitmapPalette(engine.activePalette);
                    VSyncAndDraw();

                    FillTextBitmap(engine.font, engine.pBitmapInfo[0], "You are Nix, the pilot of Icarus, who feels");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[1], "the urge to help The Revolt in overthrowing");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[2], "The Reign.");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[3], "You want to become The Carrier, who supplies");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[4], "the military units of The Revolt.");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[5], "As Carriers can move freely only in the deep");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[6], "canyons covering the planet, The Revolt wants");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[7], "you to prove your abilities and loyalty");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[8], "by beating the route leading through");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[9], "narrow valleys as fast as possible.");

                    DrawTextBitmap(engine.pBitmapInfo[0], 2, 30, 14);
                    DrawTextBitmap(engine.pBitmapInfo[1], 2, 36, 14);
                    DrawTextBitmap(engine.pBitmapInfo[2], 2, 42, 14);
                    DrawTextBitmap(engine.pBitmapInfo[3], 2, 48, 14);
                    DrawTextBitmap(engine.pBitmapInfo[4], 2, 54, 14);
                    DrawTextBitmap(engine.pBitmapInfo[5], 2, 60, 14);
                    DrawTextBitmap(engine.pBitmapInfo[6], 2, 66, 14);
                    DrawTextBitmap(engine.pBitmapInfo[7], 2, 72, 14);
                    DrawTextBitmap(engine.pBitmapInfo[8], 2, 78, 14);
                    DrawTextBitmap(engine.pBitmapInfo[9], 2, 84, 14);
                    
                        

                    FireDown = 1;
                } break;
                case 3:
                {

                    LoadBitmapToMemory(
                        #ifdef AMIGA
                        "data/msg"
                        #else
                        "data_a/i4"
                        #endif
                        );
                    ClearBuffor();
                    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
                    
                    SetBitmapPalette(engine.activePalette);
                    VSyncAndDraw();

                    FillTextBitmap(engine.font, engine.pBitmapInfo[0], "The ship's Anti-G engine uses the");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[1], "planet's magnetic field to move");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[2], "vertically.");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[3], "The closer you get to the surface,");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[4], "the more energy is generated.");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[5], "This energy is converted to power");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[6], "ship's propulsion engines and");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[7], "accelerate it.");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[8], "Remember, the lower you fly,");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[9], "the faster you go.");

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