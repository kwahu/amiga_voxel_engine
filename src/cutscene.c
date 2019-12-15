#include "engine.h"
#include "font_platform.h"
#include "input_platform.h"

void ShowDeathCutscene()
{
    ClearBuffor();
    SetBitmapPalette(engine.activePalette);
    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
    VSyncAndDraw();

    UBYTE lines = 0;

    if(engine.accTime & 1)
    {
        engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "You are dead!");
        engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "The Revolt won't have any use of");
        engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "you in this state!");
        lines = 3;
    }
    else
    {
        switch((engine.accTime >> 1) % 10)
        {
            case 0:
            {
                engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Whoops");
                engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "This had to hurt");
                lines = 2;
        
            } break;
            case 1:
            {
                engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Maybe piloting the aircraft");
                engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "wasn't your strongest trait");
                lines = 2;
            } break;
            case 2:
            {
                engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Hope you had your");
                engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "seatbelts fastened");
                lines = 2;
            } break;
            case 3:
            {
                engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "You died.");
                engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "But don't worry, your problems");
                engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "are over, forever");
                lines = 3;
            } break;
            case 4:
            {
                engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Who taught you to fly?");
                lines = 1;
            } break;
            case 5:
            {
                engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Here lies Nix.");
                engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "Good friend, but a terrible pilot");
                lines = 2;
            } break;
            case 6:
            {
                engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Tis but a scratch!");
                lines = 1;
            } break;
            case 7:
            {
                engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Wasted!");
                lines = 1;
            } break;
            case 8:
            {
                engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "This is the best you can do?");
                engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "I've seen better");
                lines = 2;
            } break;
            case 9:
            {
                engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Rest in peace, Nix");
                lines = 1;
            } break;
        }
        
    }
    
    UBYTE y = 70;
    for(int i = 0; i < lines; ++i)
    {	
        DrawTextBitmap(engine.pBitmapInfo[i], 100, y, 4);
        y += 6;
    }
    ProcessJoystick();
    UBYTE cont = 0;
    //wait 2 seconds
    while(!cont)
    {
        ProcessJoystick();
        if (getKey(ESCAPE))
        {
            ExitGame();
            cont = 1;
        }
        else if(getJoy(1, FIRE))
        {
            cont = 1;
        }
        
        
    }


    for(int i = 0; i < lines; ++i)
    {
        FreeTextBitmap(engine.pBitmapInfo[i]);
    }

    ClearBuffor();
    SetGamePaletter();
    VSyncAndDraw();

    InitGameState();

}

void ShowTooLateCutscene()
{
    ClearBuffor();
    SetBitmapPalette(engine.activePalette);
    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
    VSyncAndDraw();

    UBYTE lines = 0;

    
    switch((engine.accTime) % 4)
    {
        case 0:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Is this all you've got?");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "You call yourself a pilot?");
            lines = 2;
    
        } break;
        case 1:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "At this speed you wouldn't outrace");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "a cargo ship!");
            engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "Get out and train!");
            lines = 3;
        } break;
        case 2:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "You wouldn't survive a second");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "as a Carrier!");
            engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "Get lost!");
            lines = 3;
        } break;
        case 3:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Well, about time!");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "No stop wasting our time and scram!");
            lines = 2;
        } break;
        
    }


    UBYTE y = 70;
    for(int i = 0; i < lines; ++i)
    {	
        DrawTextBitmap(engine.pBitmapInfo[i], 100, y, 4);
        y += 6;
    }
    ProcessJoystick();
    UBYTE cont = 0;
    //wait 2 seconds
    while(!cont)
    {

        joyProcess();
        if (getKey(ESCAPE))
        {
            ExitGame();
            cont = 1;
        }
        else if(getJoy(1, FIRE))
        {
            cont = 1;
        }
        
        
    }


    for(int i = 0; i < lines; ++i)
    {
        FreeTextBitmap(engine.pBitmapInfo[i]);
    }

    ClearBuffor();
    SetGamePaletter();
    VSyncAndDraw();
    InitGameState();
}

void ShowWinCutscene()
{
    LoadBitmapToMemory("data/fin");
    ClearBuffor();
    SetBitmapPalette(engine.activePalette);
    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
    VSyncAndDraw();


    UBYTE lines = 0;

    
    switch((engine.accTime) % 12)
    {
        case 0:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Good job, pilot!");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "You have proven yourself");
            engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "worthy of becoming a Carrier");
            engine.pBitmapInfo[3] = CreateBitmapFromText(engine.font, "of the Revolt!");
            lines = 4;
    
        } break;
        case 1:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "This is what I call flying!");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "Great job!");
            lines = 2;
        } break;
        case 2:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "You're faster than a");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "swooping Vertiraptor!");
            lines = 2;
        } break;
        case 3:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Impressive!");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "You surely know how to handle");
            engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "this baby!");
            lines = 3;
        } break;
        case 4:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "The Revolt is going to have");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "great use of you, pilot!");
            engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "Welcome aboard!");
            lines = 3;
        } break;
        case 5:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Those pilots serving the Reign");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "are no match for you!");
            engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "Time to kick some ass, pilot!");
            lines = 3;
        } break;
        case 6:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Congrats, you are in!");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "Something tells me the Reign");
            engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "is going to remember your name");
            engine.pBitmapInfo[3] = CreateBitmapFromText(engine.font, "real soon...");
            lines = 4;
        } break;
        case 7:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "This was crazy!");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "With that speed, you can");
            engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "probably get out of the orbit!");
            lines = 3;
        } break;
        case 8:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "This is incredible!");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "And you did it with a");
            engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "twin-engine drive!");
            engine.pBitmapInfo[3] = CreateBitmapFromText(engine.font, "I've seen quad-engine crafts");
            engine.pBitmapInfo[4] = CreateBitmapFromText(engine.font, "do worse!");
            engine.pBitmapInfo[5] = CreateBitmapFromText(engine.font, "Respect, pilot!");
            lines = 6;
        } break;
        case 9:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "The Revolt will have good use");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "of you, pilot!");
            engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "You're in!");
            lines = 3;
        } break;
        case 10:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "I haven't seen anyone fly like");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "this since my younger days!");
            engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "Congrats!");
            lines = 3;
        } break;
        case 11:
        {
            engine.pBitmapInfo[0] = CreateBitmapFromText(engine.font, "Great time!");
            engine.pBitmapInfo[1] = CreateBitmapFromText(engine.font, "I hope you're ready to repeat");
            engine.pBitmapInfo[2] = CreateBitmapFromText(engine.font, "it in the future, you're in!");
            lines = 3;
        } break;
        
    }


    UBYTE y = 30;
    for(int i = 0; i < lines; ++i)
    {	
        DrawTextBitmap(engine.pBitmapInfo[i], 170, y, 1);
        y += 6;
    }
    ProcessJoystick();
    UBYTE cont = 0;
    //wait 2 seconds
    while(!cont)
    {

    ProcessJoystick();
        if (getKey(ESCAPE))
        {
            ExitGame();
            cont = 1;
        }
        else if(getJoy(1, FIRE))
        {
            cont = 1;
        }
        
        
    }



    
    for(int i = 0; i < lines; ++i)
    {
        FreeTextBitmap(engine.pBitmapInfo[i]);
    }

    ClearBuffor();
    SetGamePaletter();
    VSyncAndDraw();
    InitMenuState();
}

void ShowCutscene(Cutscene cutsceneType)
{
    switch(cutsceneType)
    {
        case Cutscene_Death:
        {
            ShowDeathCutscene();
        } break;
        case Cutscene_TooLate:
        {
            ShowTooLateCutscene();
        } break;
        case Cutscene_Win:
        {
            ShowWinCutscene();
        } break;
        
    }
}