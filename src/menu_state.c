
void InitMenuState()
{
    ClearArena(&engine.temporaryArena);
    ResetTime();
	engine.currentState = State_Menu;
    
 
    engine.menuState.infoScreen = 0;
    for(int i = 0; i < 10; ++i)
    {
        engine.pBitmapInfo[i] = CreateFontBitmap(engine.font);
    }
    engine.menu[0] = LoadBitmapFile(DATA_DIR(m0), &engine.headers[0], engine.activePalette, 1, 0);
    engine.menu[1] = LoadBitmapFile(DATA_DIR(m1), &engine.headers[1], engine.activePalette, 1, 0);
    engine.menu[2] = LoadBitmapFile(DATA_DIR(msg), &engine.headers[2], engine.activePalette, 1, 0);
    
}

void RunMenuState()
{
    DrawBitmap4bCenter(engine.menu[0], &engine.headers[0]);

                 
    FillTextBitmap(engine.font, engine.pBitmapInfo[0], "In the distant future, on the distant");
    FillTextBitmap(engine.font, engine.pBitmapInfo[1], "inhospitable planet. Mankind engages in one");
    FillTextBitmap(engine.font, engine.pBitmapInfo[2], "thing it knows well - war.");
    FillTextBitmap(engine.font, engine.pBitmapInfo[3], "The tyrannical corporate regime wants to");
    FillTextBitmap(engine.font, engine.pBitmapInfo[4], "enslave all inhabitants of \"The Kingdom\".");
    FillTextBitmap(engine.font, engine.pBitmapInfo[5], "Humanity's new home.");
    FillTextBitmap(engine.font, engine.pBitmapInfo[6], "As a young smuggler aboard the aircraft Icarus");
    FillTextBitmap(engine.font, engine.pBitmapInfo[7], "you want to help the oppressed by joining the");
    FillTextBitmap(engine.font, engine.pBitmapInfo[8], "Revolt, but you need to prove your worth first,");
    FillTextBitmap(engine.font, engine.pBitmapInfo[9], "by completing the infamous Death Run challenge.");

    DrawTextBitmap(engine.pBitmapInfo[0], 50, BASELINE+2, 7);
    DrawTextBitmap(engine.pBitmapInfo[1], 50, BASELINE+8, 7);
    DrawTextBitmap(engine.pBitmapInfo[2], 50, BASELINE+14, 7);
    DrawTextBitmap(engine.pBitmapInfo[3], 50, BASELINE+20, 7);
    DrawTextBitmap(engine.pBitmapInfo[4], 50, BASELINE+26, 7);
    DrawTextBitmap(engine.pBitmapInfo[5], 50, BASELINE+32, 7);
    DrawTextBitmap(engine.pBitmapInfo[6], 50, BASELINE+38, 7);
    DrawTextBitmap(engine.pBitmapInfo[7], 50, BASELINE+44, 7);
    DrawTextBitmap(engine.pBitmapInfo[8], 50, BASELINE+50, 7);
    DrawTextBitmap(engine.pBitmapInfo[9], 50, BASELINE+56, 7);
    
    SetGamePaletter();
    
    VSyncAndDraw();
    ClearBuffor();

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
                    
                    DrawBitmap4bCenter(engine.menu[1], &engine.headers[1]);
                 
                    FillTextBitmap(engine.font, engine.pBitmapInfo[0], "After setting off the Earth in enormous");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[1], "Generation ships, humanity reached the nearest");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[2], "star system and colonized it's only suitable");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[3], "planet, renaming it \"The Kingdom\".");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[4], "The harsh evironment was one problem, the");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[5], "ideological differences were the other.");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[6], "People of The Kingdom fell prey to their nature,");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[7], "which pushed them to the open conflict");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[8], "between The Reign, and The Revolt.");

                    DrawTextBitmap(engine.pBitmapInfo[0], 2, BASELINE+2, 13);
                    DrawTextBitmap(engine.pBitmapInfo[1], 2, BASELINE+8, 13);
                    DrawTextBitmap(engine.pBitmapInfo[2], 2, BASELINE+14, 13);
                    DrawTextBitmap(engine.pBitmapInfo[3], 2, BASELINE+20, 13);
                    DrawTextBitmap(engine.pBitmapInfo[4], 2, BASELINE+26, 13);
                    DrawTextBitmap(engine.pBitmapInfo[5], 2, BASELINE+32, 13);
                    DrawTextBitmap(engine.pBitmapInfo[6], 2, BASELINE+38, 13);
                    DrawTextBitmap(engine.pBitmapInfo[7], 2, BASELINE+44, 13);
                    DrawTextBitmap(engine.pBitmapInfo[8], 2, BASELINE+50, 13);
                    
                    
                    VSyncAndDraw();
                    ClearBuffor();
                    FireDown = 1;
                } break;
                case 2:
                {
                    DrawBitmap4bCenter(engine.menu[1], &engine.headers[1]);
                    

                    FillTextBitmap(engine.font, engine.pBitmapInfo[0], "You are Nix, the pilot of Icarus, who feels");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[1], "the urge to help The Revolt in overthrowing");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[2], "The Reign.");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[3], "You want to become The Carrier, who supplies");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[4], "the military units of The Revolt.");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[5], "As Carriers can move freely only in the deep");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[6], "canyons covering the planet, The Revolt wants");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[7], "you to prove your abilities and loyalty,");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[8], "by beating the route leading through");
                    FillTextBitmap(engine.font, engine.pBitmapInfo[9], "narrow valleys as fast as possible.");

                    DrawTextBitmap(engine.pBitmapInfo[0], 2, BASELINE+2, 13);
                    DrawTextBitmap(engine.pBitmapInfo[1], 2, BASELINE+8, 13);
                    DrawTextBitmap(engine.pBitmapInfo[2], 2, BASELINE+14, 13);
                    DrawTextBitmap(engine.pBitmapInfo[3], 2, BASELINE+20, 13);
                    DrawTextBitmap(engine.pBitmapInfo[4], 2, BASELINE+26, 13);
                    DrawTextBitmap(engine.pBitmapInfo[5], 2, BASELINE+32, 13);
                    DrawTextBitmap(engine.pBitmapInfo[6], 2, BASELINE+38, 13);
                    DrawTextBitmap(engine.pBitmapInfo[7], 2, BASELINE+44, 13);
                    DrawTextBitmap(engine.pBitmapInfo[8], 2, BASELINE+50, 13);
                    DrawTextBitmap(engine.pBitmapInfo[9], 2, BASELINE+56, 13);
                    VSyncAndDraw();
                    ClearBuffor();
                    
                        

                    FireDown = 1;
                } break;
                case 3:
                {
                    DrawBitmap4bCenter(engine.menu[2], &engine.headers[2]);
                    
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

                    DrawTextBitmap(engine.pBitmapInfo[0], 100, BASELINE+42, 4);
                    DrawTextBitmap(engine.pBitmapInfo[1], 100, BASELINE+48, 4);
                    DrawTextBitmap(engine.pBitmapInfo[2], 100, BASELINE+54, 4);
                    DrawTextBitmap(engine.pBitmapInfo[3], 100, BASELINE+60, 4);
                    DrawTextBitmap(engine.pBitmapInfo[4], 100, BASELINE+66, 4);
                    DrawTextBitmap(engine.pBitmapInfo[5], 100, BASELINE+72, 4);
                    DrawTextBitmap(engine.pBitmapInfo[6], 100, BASELINE+78, 4);
                    DrawTextBitmap(engine.pBitmapInfo[7], 100, BASELINE+84, 4);
                    DrawTextBitmap(engine.pBitmapInfo[8], 100, BASELINE+90, 4);
                    DrawTextBitmap(engine.pBitmapInfo[9], 100, BASELINE+96, 4);
                    VSyncAndDraw();
                    ClearBuffor();

                        

                    FireDown = 1;
                } break;
                case 4:
                {
                    StopSample();
                    UseSystem();


                    
                    ClearArena(&engine.temporaryArena);
                    for(int i = 0; i < 3; ++i)
                    {
                        engine.pBitmapInfo[i] = CreateFontBitmap(engine.font);
                    }
                    engine.menu[2] = LoadBitmapFile(DATA_DIR(msg), &engine.headers[2], engine.activePalette, 1, 0);
                    engine.menuState.infoScreen = 1;
                    
                    
                    engine.explosionBitmap = LoadBitmapFile(DATA_DIR(iexpl), &engine.explosionHeader, engine.activePalette, 2, 14);
                    engine.landingBitmap = LoadBitmapFile(DATA_DIR(land), &engine.landingHeader, engine.activePalette, 2, 14);
                    engine.takeoffBitmap = LoadBitmapFile(DATA_DIR(take), &engine.takeoffHeader, engine.activePalette, 2, 14);
                    engine.shipBitmap = LoadBitmapFile(DATA_DIR(icar48), &engine.shipHeader, engine.activePalette, 2, 14);

                    
                    engine.pBitmapVelocity = CreateBitmapFromText(engine.font, "1234");
                    engine.pBitmapScore = CreateBitmapFromText(engine.font, "1234567");
                    engine.pBitmapTime = CreateBitmapFromText(engine.font, "1234567");
                    engine.pBitmapHeight = CreateBitmapFromText(engine.font, "1234");
                    engine.pBitmapVelocityLabel = CreateBitmapFromText(engine.font, "AIR SPEED");
                    engine.pBitmapScoreLabel = CreateBitmapFromText(engine.font, "SCORE");
                    engine.pBitmapTimeLabel = CreateBitmapFromText(engine.font, "TIME");
                    engine.pBitmapHeightLabel = CreateBitmapFromText(engine.font, "RELATIVE ALTITUDE");
                    
                    UnuseSystem();
                    VSyncAndDraw();
                    ClearBuffor();

                    InitAudio();
                    PlaySample(5);
                    
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
        VSyncWait();
    }

    InitGameState();
}