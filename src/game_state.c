#include "engine.h"

void InitGameState()
{
    ResetTime();

    engine.currentState = State_Game;

    engine.gameState.shipParams.precX = 50 * 100;
    engine.gameState.shipParams.precZ = 0;
    engine.gameState.shipParams.pX = engine.gameState.shipParams.precX >> 5;
    engine.gameState.shipParams.pZ = 0;

    OverwriteMap();

    UWORD terrainHeight = getTerrainHeight(engine.gameState.shipParams, engine.renderer.mapHigh);
    engine.gameState.startHeight = terrainHeight + 2;
    engine.gameState.shipParams.pY = engine.gameState.startHeight;
    
    engine.gameState.shipParams.precY = engine.gameState.shipParams.pY << 5;
    engine.gameState.shipParams.dPDenom = 160;
    engine.gameState.shipParams.dP = 0;
    engine.gameState.shipParams.ddP = 0;
    engine.gameState.shipParams.relHeight = 0;

    engine.gameState.crossHairX = 0;
    engine.gameState.crossHairY = 0;
	engine.gameState.points = 0;
    engine.gameState.animDuration = 0;
    engine.gameState.runOver = 0;
    engine.gameState.playerDeath = 0;
    engine.gameState.takeoff = 1;
    engine.renderer.lastOverwrittenLine = 0;

    UWORD depthBufferSize = (UWORD)engine.renderer.depthBufferHeight*(UWORD)engine.renderer.depthBufferWidth;

    for(UWORD i = 0; i < depthBufferSize; ++i)
    {
        engine.renderer.depthBuffer[i] = 0xFF;
    }
 

    engine.renderer.mapHigh = engine.renderer.mapSource;
    
    ClearBuffor();
    SetGamePaletter();
	//CopyMapWord(engine.renderer.mapSource, engine.renderer.mapHigh);
    
}

void UpdatePlayerPosition()
{
    ProcessInput();


    UWORD terrainHeight = getTerrainHeight(engine.gameState.shipParams, engine.renderer.mapHigh);

    ULONG lowerDelta = (engine.deltaTime/10000);
    updateShipParams(lowerDelta, terrainHeight);

    LONG addedpoints = (lowerDelta)*(110 - engine.gameState.shipParams.relHeight);
    if(addedpoints > 0 && engine.gameState.shipParams.pZ < 2816)
    {
        engine.gameState.points += addedpoints;
    }


}

BYTE CheckPlayerCollision()
{
    UBYTE shipCollisionY = (engine.gameState.shipParams.pY - 2);
    UWORD shipCollisionX = engine.gameState.shipParams.pX;

    return shipCollisionY < (UBYTE)(engine.renderer.mapHigh[(((UBYTE)(shipCollisionX)) >> 1)*11*MAPSIZE + (((UBYTE)(2*engine.renderer.zStart)) >> 1)]);
    
    //return shipCollisionY < (UBYTE)(engine.renderer.mapHigh[((UBYTE)(shipCollisionX)) >>1][((UBYTE)(engine.gameState.shipParams.pZ + engine.renderer.zStart + 8)) >>1]);
}

void RenderShipAndCrossHair()
{
    UWORD crossPX =  (160 + (engine.gameState.crossHairX / 400));
    UWORD crossPY = ( GAME_SHIP_POS - 5 + (engine.gameState.crossHairY / 400) );

    UWORD shipDirX = 160 + engine.renderer.xTurnOffset;
    UWORD shipDirY = GAME_SHIP_POS + (engine.gameState.crossHairY/1600);

    WORD spriteIndexX = 1;
    WORD spriteIndexY = 1;
    if(engine.gameState.crossHairX > 8000)
    {
        spriteIndexX = 2;
    }
    else if(engine.gameState.crossHairX < -8000)
    {
        spriteIndexX = 0;
    }
    if(engine.gameState.crossHairY > 4000)
    {
        spriteIndexY = 2;
    }
    else if(engine.gameState.crossHairY < -4000)
    {
        spriteIndexY = 0;
    }

                
    UBYTE bestFit = engine.renderer.depthBufferHeight + 1;
    WORD bestValue = 1024;
    UBYTE *depthPtr = engine.renderer.depthBuffer + engine.renderer.depthBufferWidth/2;

    for(BYTE i = 0; i < engine.renderer.depthBufferHeight/2; ++i)
    {
        UBYTE d = *depthPtr;
        WORD depth = (WORD)d;
        WORD offset = (WORD)engine.renderer.zStart + (WORD)((WORD)(engine.renderer.depthBufferHeight/4) - i);
        WORD value = (depth - offset);
        if((value > 0) && (value < bestValue) && (depth > engine.renderer.zStart + 1))
        {
            bestValue = value;
            bestFit = i;
        }
        depthPtr += engine.renderer.depthBufferWidth;
    }

    if(bestFit < (engine.renderer.depthBufferHeight)/2)
    {
        UBYTE relHeightCoeff = ((bestFit)/(engine.renderer.depthBufferHeight/10)) + 1;  
        
        bestFit = bestFit*engine.renderer.shadowStep;
     
        
        for(UBYTE y = 0; y < relHeightCoeff; y++)
        {
            DrawPixel(shipDirX - relHeightCoeff + y, GAME_SCREEN_BASELINE - bestFit - y*2-1, 0);
            DrawPixel(shipDirX + relHeightCoeff - y, GAME_SCREEN_BASELINE - bestFit - y*2-1, 0);   
            DrawPixel(shipDirX - relHeightCoeff + y, GAME_SCREEN_BASELINE - bestFit + y*2+1, 0);
            DrawPixel(shipDirX + relHeightCoeff - y, GAME_SCREEN_BASELINE - bestFit + y*2+1, 0);      
        }

    }


    DrawCrosshair( crossPX, crossPY + 4);
    DrawCrosshair( crossPX, crossPY - 4);
    
     DrawSprite4b(engine.shipBitmap, &engine.shipHeader, shipDirX, shipDirY,
                     spriteIndexX, spriteIndexY, 48, 48);
     
}

void DrawGameStats()
{
    ConvertIntToChar(engine.gameState.points, engine.gameState.sScore, 8);
    ConvertIntToChar(engine.gameState.shipParams.dP, engine.gameState.sVelocity, 5);
    ConvertIntToChar(engine.accTime/2500, engine.gameState.sTime, 8);
    ConvertIntToChar(engine.gameState.shipParams.relHeight, engine.gameState.sPlayerY, 5);
    

    FillTextBitmap(engine.font, engine.pBitmapScore, engine.gameState.sScore);
    FillTextBitmap(engine.font, engine.pBitmapVelocity, engine.gameState.sVelocity);
    FillTextBitmap(engine.font, engine.pBitmapTime, engine.gameState.sTime);
    FillTextBitmap(engine.font, engine.pBitmapHeight, engine.gameState.sPlayerY);

    DrawTextBitmapOverwrite(engine.pBitmapVelocity, 115, PLANEHEIGHT-6, 12);
    DrawTextBitmapOverwrite(engine.pBitmapScore, 30, PLANEHEIGHT-6, 12);
    DrawTextBitmapOverwrite(engine.pBitmapHeight, 215, PLANEHEIGHT-6, 12);
    DrawTextBitmapOverwrite(engine.pBitmapTime, 255, PLANEHEIGHT-6, 12);
    DrawTextBitmap(engine.pBitmapVelocityLabel, 70, PLANEHEIGHT-6, 12);
    DrawTextBitmap(engine.pBitmapScoreLabel, 0, PLANEHEIGHT-6, 12);
    DrawTextBitmap(engine.pBitmapHeightLabel, 140, PLANEHEIGHT-6, 12);
    DrawTextBitmap(engine.pBitmapTimeLabel, 240, PLANEHEIGHT-6, 12);

}

void RunGameState()
{


    //ProcessQualityInput();
    
    
    
    if(engine.gameState.shipParams.pZ > 2816)
    {
        engine.gameState.crossHairX = 0;

        if(((UWORD)engine.gameState.shipParams.relHeight < 255))
        {
            UWORD offset = (((UWORD)engine.gameState.shipParams.relHeight)*200);
            if(offset > 0x7FFF)
            {
                offset = 0x7FFF;
            }
            engine.gameState.crossHairY = offset;
        }
        else
        {
            engine.gameState.crossHairY = 0;
        }
        

        UWORD terrainHeight = getTerrainHeight(engine.gameState.shipParams, engine.renderer.mapHigh);

        ULONG lowerDelta = (engine.deltaTime/10000);
        updateShipParams(lowerDelta, engine.gameState.shipParams.pY - 200);

        
        

        engine.gameState.shipParams.dPDenom = engine.gameState.shipParams.dPDenom + 8;
	    engine.gameState.shipParams.relHeight = ((engine.gameState.shipParams.pY - 2) - terrainHeight);

        
        if(engine.gameState.shipParams.relHeight <= 4 && engine.gameState.shipParams.dPDenom > 256)
        {
            engine.gameState.runOver = 1;
        }
    }
    else
    {
        
        if(!engine.gameState.playerDeath && !engine.gameState.takeoff)
        {
            UpdatePlayerPosition(); 
            if(CheckPlayerCollision())
            {
                engine.gameState.playerDeath = 1;
            }
        }
        
        
        
    }
    

//draw crosshair

    
            //draw only even lines 
    
                        
    OverwriteMap(); //this is how we go through many different maps, we just overwrite the main array with new content



    engine.renderer.xTurnOffset = engine.gameState.crossHairX / engine.renderer.turnDenom;
    
    RenderQuality();  


    if(engine.gameState.takeoff)
    {
         ULONG addedTime = engine.deltaTime/2500;
            
        if((engine.gameState.animDuration + addedTime) < 1200)
        {
            engine.gameState.animDuration += addedTime;
            ULONG currentIndex = (ULONG)(engine.gameState.animDuration/100);
            WORD currentX = (WORD)(currentIndex%4);
            WORD currentY = (WORD)((currentIndex/4));
            
            UWORD takeoffX = 160 + engine.renderer.xTurnOffset;
            UWORD takeoffY = GAME_SHIP_POS + (engine.gameState.crossHairY/1600);


            DrawSprite4b(engine.takeoffBitmap, &engine.takeoffHeader, takeoffX, takeoffY,
                     currentX, currentY, 48, 48);

            engine.gameState.shipParams.pY = engine.gameState.startHeight + currentIndex;
            engine.gameState.shipParams.precY = engine.gameState.shipParams.pY << 5;
         }
        else
        {
            engine.gameState.animDuration = 0;
            engine.gameState.takeoff = 0;
            
        }
    }
       


    if(!engine.gameState.playerDeath && !engine.gameState.runOver && !engine.gameState.takeoff)
    {
        RenderShipAndCrossHair();
    }

    DrawGameStats();

    if(engine.gameState.playerDeath == 1)
    {
        ULONG addedTime = engine.deltaTime/2500;
            
        if((engine.gameState.animDuration + addedTime) < 675)
        {
            engine.gameState.animDuration += addedTime;
            ULONG currentIndex = (ULONG)(engine.gameState.animDuration/75);
            WORD currentX = (WORD)(currentIndex%3);
            WORD currentY = (WORD)((currentIndex/3));
            
            UWORD explosionX = 160 + engine.renderer.xTurnOffset;
            UWORD explosionY = GAME_SHIP_POS + (engine.gameState.crossHairY/1600);


            DrawSprite4b(engine.explosionBitmap, &engine.explosionHeader, explosionX, explosionY,
                     currentX, currentY, 48, 48);
        }
        else
        {
            ShowCutscene(Cutscene_Death, PATTERN_DURATION);
            
        }
        
    }


    // 	if(keyCheck(KEY_Q)){calculationDepthDivider=1;Recalculate();}
    // if(keyCheck(KEY_W)){calculationDepthDivider=2;Recalculate();}
    // if(keyCheck(KEY_E)){calculationDepthDivider=3;Recalculate();}
    // if(keyCheck(KEY_R)){calculationDepthDivider=4;Recalculate();}
    // if(keyCheck(KEY_T)){calculationDepthDivider=5;Recalculate();}
    // if(keyCheck(KEY_Y)){calculationDepthDivider=6;Recalculate();}
    // if(keyCheck(KEY_U)){calculationDepthDivider=7;Recalculate();}
    // if(keyCheck(KEY_I)){calculationDepthDivider=8;Recalculate();}
    // if(keyCheck(KEY_O)){calculationDepthDivider=9;Recalculate();}
    // if(keyCheck(KEY_P)){calculationDepthDivider=10;Recalculate();}

    // if(keyCheck(KEY_A)){calculationDepthStep=1;Recalculate();}
    // if(keyCheck(KEY_S)){calculationDepthStep=2;Recalculate();}
    // if(keyCheck(KEY_D)){calculationDepthStep=3;Recalculate();}
    // if(keyCheck(KEY_F)){calculationDepthStep=4;Recalculate();}
    // if(keyCheck(KEY_G)){calculationDepthStep=5;Recalculate();}
    // if(keyCheck(KEY_H)){calculationDepthStep=6;Recalculate();}
    // if(keyCheck(KEY_J)){calculationDepthStep=7;Recalculate();}
    // if(keyCheck(KEY_K)){calculationDepthStep=8;Recalculate();}
    // if(keyCheck(KEY_L)){calculationDepthStep=9;Recalculate();}
    // if(keyCheck(KEY_SEMICOLON)){calculationDepthStep=10;Recalculate();}

    // if(keyCheck(KEY_Z)){renderingDepthStep=1;}
    // if(keyCheck(KEY_X)){renderingDepthStep=2;}
    // if(keyCheck(KEY_C)){renderingDepthStep=3;}
    // if(keyCheck(KEY_V)){renderingDepthStep=4;}
    // if(keyCheck(KEY_B)){renderingDepthStep=5;}
    // if(keyCheck(KEY_N)){renderingDepthStep=6;}
    // if(keyCheck(KEY_M)){renderingDepthStep=7;}
    // if(keyCheck(KEY_COMMA)){renderingDepthStep=8;}
    // if(keyCheck(KEY_PERIOD)){renderingDepthStep=9;}
    // if(keyCheck(KEY_SLASH)){renderingDepthStep=10;}

    if(engine.gameState.runOver)
    {
         ULONG addedTime = engine.deltaTime/2500;
            
        if((engine.gameState.animDuration + addedTime) < 1300)
        {
            engine.gameState.animDuration += addedTime;
            ULONG currentIndex = (ULONG)(engine.gameState.animDuration/100);
            WORD currentX = (WORD)(currentIndex%4);
            WORD currentY = (WORD)((currentIndex/4));
            
            UWORD landingX = 160 + engine.renderer.xTurnOffset;
            UWORD landingY = GAME_SHIP_POS + (engine.gameState.crossHairY/1600);


            DrawSprite4b(engine.landingBitmap, &engine.landingHeader, landingX, landingY,
                     currentX, currentY, 48, 48);
        }
        else
        {
       
            if(engine.gameState.points < 1000000)
            {
                ShowCutscene(Cutscene_TooLate, PATTERN_DURATION);
            }
            else
            {
                ShowCutscene(Cutscene_Win, 11000);

            }     
        }
       
    }

    
    VSyncAndDraw();    
    
    


}