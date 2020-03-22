

void ShowDeathCutscene()
{
    
    StopSample();
    PlaySample(16);
    ContinueSample();

    ClearBuffor();
    SetBitmapPalette(engine.activePalette);
    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
    VSyncAndDraw();

    UBYTE lines = 0;

    if(engine.accTime & 1)
    {
        FillTextBitmap(engine.font, engine.pBitmapInfo[0], "You are dead!");
        FillTextBitmap(engine.font, engine.pBitmapInfo[1], "The Revolt won't have any use of");
        FillTextBitmap(engine.font, engine.pBitmapInfo[2], "you in this state!");
        lines = 3;
    }
    else
    {
        switch((engine.accTime >> 1) % 10)
        {
            case 0:
            {
                FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Whoops");
                FillTextBitmap(engine.font, engine.pBitmapInfo[1], "This had to hurt");
                lines = 2;
        
            } break;
            case 1:
            {
                FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Maybe piloting the aircraft");
                FillTextBitmap(engine.font, engine.pBitmapInfo[1], "wasn't your strongest trait");
                lines = 2;
            } break;
            case 2:
            {
                FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Hope you had your");
                FillTextBitmap(engine.font, engine.pBitmapInfo[1], "seatbelts fastened");
                lines = 2;
            } break;
            case 3:
            {
                FillTextBitmap(engine.font, engine.pBitmapInfo[0], "You died.");
                FillTextBitmap(engine.font, engine.pBitmapInfo[1], "But don't worry, your problems");
                FillTextBitmap(engine.font, engine.pBitmapInfo[2], "are over, forever");
                lines = 3;
            } break;
            case 4:
            {
                FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Who taught you to fly?");
                lines = 1;
            } break;
            case 5:
            {
                FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Here lies Nix.");
                FillTextBitmap(engine.font, engine.pBitmapInfo[1], "Good friend, but a terrible pilot");
                lines = 2;
            } break;
            case 6:
            {
                FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Tis but a scratch!");
                lines = 1;
            } break;
            case 7:
            {
                FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Wasted!");
                lines = 1;
            } break;
            case 8:
            {
                FillTextBitmap(engine.font, engine.pBitmapInfo[0], "This is the best you can do?");
                FillTextBitmap(engine.font, engine.pBitmapInfo[1], "I've seen better");
                lines = 2;
            } break;
            case 9:
            {
                FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Rest in peace, Nix");
                lines = 1;
            } break;
        }
        
    }

    UBYTE y = BASELINE+42;
    for(int i = 0; i < lines; ++i)
    {	
        DrawTextBitmap(engine.pBitmapInfo[i], 100, y, 4);
        y += 6;
    }
    VSyncAndDraw();
    

}

void ShowTooLateCutscene()
{
    StopSample();
    PlaySample(16);
    ContinueSample();
    ClearBuffor();
 
   SetBitmapPalette(engine.activePalette);
    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
    VSyncAndDraw();

    UBYTE lines = 0;


    switch((engine.accTime) % 4)
    {
        case 0:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Is this all you've got?");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "You call yourself a pilot?");
            lines = 2;
    
        } break;
        case 1:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "At this speed you wouldn't outrace");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "a cargo ship!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[2], "Get out and train!");
            lines = 3;
        } break;
        case 2:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "You wouldn't survive a second");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "as a Carrier!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[2], "Get lost!");
            lines = 3;
        } break;
        case 3:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Well, about time!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "No stop wasting our time and scram!");
            lines = 2;
        } break;
        
    }

    UBYTE y = BASELINE+42;
    for(int i = 0; i < lines; ++i)
    {	
        DrawTextBitmap(engine.pBitmapInfo[i], 100, y, 4);
        y += 6;
    }
    VSyncAndDraw();

}

void ShowWinCutscene()
{

    StopSample();
    LoadBitmapToMemory("data/fin");
    PlaySample(18);
    ContinueSample();
    ClearBuffor();
    SetBitmapPalette(engine.activePalette);
    DrawBitmap4bCenter(engine.activeBitmap, &engine.activeBitmapHeader);
    VSyncAndDraw();


    UBYTE lines = 0;

    
    switch((engine.accTime) % 12)
    {
        case 0:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Good job, pilot!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "You have proven yourself");
            FillTextBitmap(engine.font, engine.pBitmapInfo[2], "worthy of becoming a Carrier");
            FillTextBitmap(engine.font, engine.pBitmapInfo[3], "of the Revolt!");
            lines = 4;
    
        } break;
        case 1:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "This is what I call flying!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "Great job!");
            lines = 2;
        } break;
        case 2:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "You're faster than a");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "swooping Vertiraptor!");
            lines = 2;
        } break;
        case 3:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Impressive!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "You surely know how to handle");
            FillTextBitmap(engine.font, engine.pBitmapInfo[2], "this baby!");
            lines = 3;
        } break;
        case 4:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "The Revolt is going to have");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "great use of you, pilot!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[2], "Welcome aboard!");
            lines = 3;
        } break;
        case 5:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Those pilots serving the Reign");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "are no match for you!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[2], "Time to kick some ass, pilot!");
            lines = 3;
        } break;
        case 6:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Congrats, you are in!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "Something tells me the Reign");
            FillTextBitmap(engine.font, engine.pBitmapInfo[2], "is going to remember your name");
            FillTextBitmap(engine.font, engine.pBitmapInfo[3], "real soon...");
            lines = 4;
        } break;
        case 7:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "This was crazy!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "With that speed, you can");
            FillTextBitmap(engine.font, engine.pBitmapInfo[2], "probably get out of the orbit!");
            lines = 3;
        } break;
        case 8:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "This is incredible!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "And you did it with a");
            FillTextBitmap(engine.font, engine.pBitmapInfo[2], "twin-engine drive!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[3], "I've seen quad-engine crafts");
            FillTextBitmap(engine.font, engine.pBitmapInfo[4], "do worse!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[5], "Respect, pilot!");
            lines = 6;
        } break;
        case 9:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "The Revolt will have good use");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "of you, pilot!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[2], "You're in!");
            lines = 3;
        } break;
        case 10:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "I haven't seen anyone fly like");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "this since my younger days!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[2], "Congrats!");
            lines = 3;
        } break;
        case 11:
        {
            FillTextBitmap(engine.font, engine.pBitmapInfo[0], "Great time!");
            FillTextBitmap(engine.font, engine.pBitmapInfo[1], "I hope you're ready to repeat");
            FillTextBitmap(engine.font, engine.pBitmapInfo[2], "it in the future, you're in!");
            lines = 3;
        } break;
        
    }


    UBYTE y = BASELINE+2;
    for(int i = 0; i < lines; ++i)
    {	
        DrawTextBitmap(engine.pBitmapInfo[i], 170, y, 1);
        y += 6;
    }

    VSyncAndDraw();
}

void ShowCutscene(Cutscene cutsceneType, ULONG duration)
{
    ResetTime();

    engine.cutsceneDuration = duration;
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
        VSyncWait();
        
        
        TimeStep();
        
        if(engine.accTime/2500 >= engine.cutsceneDuration)
        {
            cont = 1;
        }
    }


    UBYTE nextPattern = 0;
    
    
    StopSample();
    UseSystem();
    switch(cutsceneType)
    {
        case Cutscene_Death:
        {
            InitGameState();
            nextPattern = 6;
        } break;
        case Cutscene_TooLate:
        {
            InitGameState();
            nextPattern = 6;
        } break;
        case Cutscene_Win:
        {
            InitMenuState();
            nextPattern = 0;
        } break;
        
    }
    
    UnuseSystem();
    PlaySample(nextPattern);
    ContinueSample();

}