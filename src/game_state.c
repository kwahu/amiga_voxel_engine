#include "engine.h"

void InitGameState()
{
    ResetTime();

    engine.currentState = State_Game;

    engine.gameState.shipParams.precX = 50 * 100;
    engine.gameState.shipParams.precZ = 0;
    engine.gameState.shipParams.pX = engine.gameState.shipParams.precX >> 5;
    engine.gameState.shipParams.pZ = 0;

    engine.renderer.lastOverwrittenLine = 0;
    engine.renderer.currentMap = engine.renderer.firstMap;
    engine.renderer.currentMapLength = engine.firstMapLength;
    engine.renderer.mapLengthRemainder = 0;
    engine.renderer.mapHigh = engine.renderer.firstMap;

    OverwriteMap();

    UWORD terrainHeight = getTerrainHeight(engine.gameState.shipParams, engine.renderer.mapHigh);
    engine.gameState.startHeight = terrainHeight + 16;
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

    UWORD depthBufferSize = (UWORD)engine.renderer.depthBufferHeight*(UWORD)engine.renderer.depthBufferWidth;

    for(UWORD i = 0; i < depthBufferSize; ++i)
    {
        engine.renderer.depthBuffer[i] = 0xFF;
    }
 

    
    //ClearBuffor();
	//CopyMapWord(engine.renderer.mapSource, engine.renderer.mapHigh);
    
}

void UpdatePlayerPosition()
{
    ProcessInput();


    UWORD terrainHeight = getTerrainHeight(engine.gameState.shipParams, engine.renderer.mapHigh);

    ULONG lowerDelta = (engine.deltaTime/10000);
    updateShipParams(lowerDelta, terrainHeight);

    LONG addedpoints = (lowerDelta)*(125 - engine.gameState.shipParams.relHeight);
    if(addedpoints > 0 && (engine.gameState.shipParams.pZ + (engine.renderer.mapLengthRemainder)) < 2816)
    {
        engine.gameState.points += addedpoints;
    }


}

BYTE CheckPlayerCollision()
{

    WORD shipOffX = ((engine.renderer.xTurnOffset<<1) + 8)/16;
    WORD shipOffY = (engine.gameState.crossHairY/1600 + 20)/ (PLANEHEIGHT/engine.renderer.depthBufferHeight);
    
    UBYTE *baseDepth = engine.renderer.depthBuffer + 
    (((engine.renderer.depthBufferHeight>>1) - shipOffY)*engine.renderer.depthBufferWidth)
     + (engine.renderer.depthBufferWidth>>1) - shipOffX;
     if(*baseDepth == engine.renderer.zStart)
     {
         return 1;
     }
     return 0;

    // UBYTE shipCollisionY = (engine.gameState.shipParams.pY - 2);
    // UWORD shipCollisionX = engine.gameState.shipParams.pX + (engine.renderer.xTurnOffset);

    // ULONG length = engine.renderer.currentMapLength;
    // if(engine.secondMapLength > 0)
    // {
    //     length += MAPSIZE;
    // }

    // return shipCollisionY < (UBYTE)(engine.renderer.mapHigh[(((UBYTE)(shipCollisionX)) >> 1)*length + (((UBYTE)(engine.renderer.zStart + 18)) >> 1)]);
    
    //return shipCollisionY < (UBYTE)(engine.renderer.mapHigh[((UBYTE)(shipCollisionX)) >>1][((UBYTE)(engine.gameState.shipParams.pZ + engine.renderer.zStart + 8)) >>1]);
}

void RenderShipAndCrossHair()
{
    UWORD crossPX =  (152 + (engine.gameState.crossHairX / 200));
    UWORD crossPY = ( GAME_SHIP_POS - 20 + (engine.gameState.crossHairY / 200) );

    UWORD shipDirX = 152 - (engine.renderer.xTurnOffset<<1);
    UWORD shipDirY = GAME_SHIP_POS - (engine.gameState.crossHairY/1600);

    WORD spriteIndexX = 0;
    WORD spriteIndexY = 1;
    if(engine.gameState.crossHairX > 10000)
    {
        spriteIndexX = 2;
    }
    else if(engine.gameState.crossHairX > 5000)
    {
        spriteIndexX = 1;
    }
    else if(engine.gameState.crossHairX < -10000)
    {
        spriteIndexX = -2;
    }
    else if(engine.gameState.crossHairX < -5000)
    {
        spriteIndexX = -1;
    }
    if(engine.gameState.crossHairY > 4000)
    {
        spriteIndexY = 0;
    }
    else if(engine.gameState.crossHairY < -4000)
    {
        spriteIndexY = 2;
    }


                
    UBYTE bestFit = engine.renderer.depthBufferHeight + 1;
    WORD bestValue = 1024;
    UBYTE *depthPtr = engine.renderer.depthBuffer + engine.renderer.depthBufferWidth/2 - (((engine.renderer.xTurnOffset<<1) - 8)/16);
    UBYTE *leftSideDepthPtr = depthPtr + (engine.renderer.depthBufferHeight>>1)*engine.renderer.depthBufferWidth - 1;
    UBYTE *rightSideDepthPtr = leftSideDepthPtr + 1;
    UBYTE leftStep = engine.renderer.dustStep;
    UBYTE rightStep = leftStep;
    UBYTE stepAmount = rightStep;
    UBYTE remainder = 8 - engine.renderer.dustStep;

    UWORD xIndex = (engine.accTime/250000) & 3;
    UWORD yIndex;

    while(leftStep > 0)
    {
        if(*leftSideDepthPtr < engine.renderer.zStart + 6)
        {
            yIndex = 2 - (leftStep + remainder)/3;

            UWORD basePos = shipDirX - 24 - (stepAmount-leftStep)*16;  
            if(basePos < 300)
            {           
                DrawSprite4b(engine.dustLeftBitmap, &engine.dustLeftHeader, basePos, shipDirY,
                        xIndex, yIndex, 32, 32, 0);

                // DrawPixel(basePos, shipDirY-3, 8, 13);
                // DrawPixel(basePos, shipDirY-2, 12, 13);
                // DrawPixel(basePos, shipDirY-1, 6, 13);
                // DrawPixel(basePos, shipDirY, 3, 13);
                // DrawPixel(basePos, shipDirY+1, 6, 13);
                // DrawPixel(basePos, shipDirY+2, 12, 13);
                // DrawPixel(basePos, shipDirY+3, 8, 13);
                
            }
            leftStep = 0;
        }
        else
        {
            leftStep--;
            leftSideDepthPtr--;
        }
        
    }
    while(rightStep > 0)
    {
        if(*rightSideDepthPtr < engine.renderer.zStart + 6)
        {
            yIndex = 2 - (rightStep + remainder)/3;
            UWORD basePos = shipDirX + 40 + (stepAmount-rightStep)*16; 
            if(basePos > 300)
            {
                basePos = 300;
            }           

            DrawSprite4b(engine.dustRightBitmap, &engine.dustRightHeader, basePos, shipDirY,
                     xIndex, yIndex, 32, 32, 0);

            // DrawPixel(basePos, shipDirY-3, 1, 13);
            // DrawPixel(basePos, shipDirY-2, 3, 13);
            // DrawPixel(basePos, shipDirY-1, 6, 13);
            // DrawPixel(basePos, shipDirY, 12, 13);
            // DrawPixel(basePos, shipDirY+1, 6, 13);
            // DrawPixel(basePos, shipDirY+2, 3, 13);
            // DrawPixel(basePos, shipDirY+3, 1, 13);
            rightStep = 0;
        }
        else
        {
            rightStep--;
            rightSideDepthPtr++;
        }
    
    }

    for(BYTE i = 0; i < (engine.renderer.depthBufferHeight>>1); ++i)
    {
        UBYTE d = *depthPtr;
        WORD depth = (WORD)d;
        WORD offset = (WORD)engine.renderer.zStart + (WORD)((WORD)(engine.renderer.depthBufferHeight>>2) - i);
        WORD value = (depth - offset);
        if((value > 0) && (value < bestValue) && (depth > engine.renderer.zStart + 1))
        {
            bestValue = value;
            bestFit = i;
        }
        depthPtr += engine.renderer.depthBufferWidth;
    }

    if(bestFit < (engine.renderer.depthBufferHeight)/3)
    {
        UBYTE relHeightCoeff = ((bestFit)/(engine.renderer.depthBufferHeight >> 4)) + 1;  
        
        bestFit = bestFit*engine.renderer.shadowStep;
     
        
        for(UBYTE y = 0; y < relHeightCoeff; y++)
        {
            DrawPixel(shipDirX - relHeightCoeff + y, GAME_SCREEN_BASELINE - bestFit - y, 10, 15);
            DrawPixel(shipDirX + relHeightCoeff - y, GAME_SCREEN_BASELINE - bestFit - y, 10, 15);   
            DrawPixel(shipDirX - relHeightCoeff + y, GAME_SCREEN_BASELINE - bestFit + y+1, 10, 15);
            DrawPixel(shipDirX + relHeightCoeff - y, GAME_SCREEN_BASELINE - bestFit + y+1, 10, 15);      
        }

    }


    DrawCrosshair( crossPX, crossPY + 4);
    DrawCrosshair( crossPX, crossPY - 4);
    

    UWORD horizontalFlip = spriteIndexX & 0x8000;

     DrawSprite4b(engine.shipBitmap, &engine.shipHeader, shipDirX, shipDirY,
                     spriteIndexX, spriteIndexY, 48, 48, horizontalFlip);

     
}


void DrawGameStats()
{
    ConvertIntToChar(engine.gameState.points, engine.gameState.sScore, 8);
    //ConvertIntToChar(engine.gameState.shipParams.dP, engine.gameState.sVelocity, 5);
    
    ConvertIntToChar(engine.accTime/2500000, engine.gameState.sTime, 8);
    ConvertIntToChar(engine.gameState.shipParams.relHeight, engine.gameState.sPlayerY, 5);
    

    FillTextBitmap(engine.font, engine.pBitmapScore, engine.gameState.sScore);
    //FillTextBitmap(engine.font, engine.pBitmapVelocity, engine.gameState.sVelocity);
    FillTextBitmap(engine.font, engine.pBitmapTime, engine.gameState.sTime);
    //FillTextBitmap(engine.font, engine.pBitmapHeight, engine.gameState.sPlayerY);

    //DrawTextBitmapOverwrite(engine.pBitmapVelocity, 115, PLANEHEIGHT-6, 12);
    DrawBar(192, PLANEHEIGHT-6, (engine.gameState.shipParams.dP >> 7) + 4);
    DrawTextBitmapOverwrite(engine.pBitmapScore, 30, PLANEHEIGHT-6, 12);
    //DrawTextBitmapOverwrite(engine.pBitmapHeight, 215, PLANEHEIGHT-6, 12);
    DrawBar(112, PLANEHEIGHT-6, ((240 - engine.gameState.shipParams.relHeight) >> 4));
    DrawTextBitmapOverwrite(engine.pBitmapTime, 255, PLANEHEIGHT-6, 12);
    DrawTextBitmap(engine.pBitmapVelocityLabel, 150, PLANEHEIGHT-6, 12);
    
    DrawTextBitmap(engine.pBitmapScoreLabel, 0, PLANEHEIGHT-6, 12);
    DrawTextBitmap(engine.pBitmapHeightLabel, 80, PLANEHEIGHT-6, 12);
    DrawTextBitmap(engine.pBitmapTimeLabel, 240, PLANEHEIGHT-6, 12);

}

void RunGameState()
{


    //ProcessQualityInput();
    
    
    
    if(engine.gameState.shipParams.pZ + (engine.renderer.mapLengthRemainder) > MAPLENGTH*MAPSIZE*2)
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
	    engine.gameState.shipParams.relHeight = ((engine.gameState.shipParams.pY - 16) - terrainHeight);

        
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



    engine.renderer.xTurnOffset = engine.gameState.crossHairX >> engine.renderer.turnDenom;
    
    RenderQuality();  


    if(engine.gameState.takeoff)
    {
         ULONG addedTime = engine.deltaTime/2500;
            
        if((engine.gameState.animDuration + addedTime) < 1200)
        {
            engine.gameState.animDuration += addedTime;
            ULONG currentIndex = (ULONG)(engine.gameState.animDuration/100);
            WORD currentX = (WORD)(currentIndex%4);
            WORD currentY = (WORD)(2 - (currentIndex/4));
            
            UWORD takeoffX = 152 - (engine.renderer.xTurnOffset<<1);
            UWORD takeoffY = GAME_SHIP_POS - (engine.gameState.crossHairY/1600);


            DrawSprite4b(engine.takeoffBitmap, &engine.takeoffHeader, takeoffX, takeoffY,
                     currentX, currentY, 48, 48, 0);

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
            WORD currentY = (WORD)(2 - (currentIndex/3));
            
            UWORD explosionX = 152 - (engine.renderer.xTurnOffset<<1);
            UWORD explosionY = GAME_SHIP_POS - (engine.gameState.crossHairY/1600);


            DrawSprite4b(engine.explosionBitmap, &engine.explosionHeader, explosionX, explosionY,
                     currentX, currentY, 48, 48, 0);
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
            WORD currentY = (WORD)(3 - (currentIndex/4));
            
            UWORD landingX = 152 - (engine.renderer.xTurnOffset<<1);
            UWORD landingY = GAME_SHIP_POS - (engine.gameState.crossHairY/1600);


            DrawSprite4b(engine.landingBitmap, &engine.landingHeader, landingX, landingY,
                     currentX, currentY, 48, 48, 0);
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
    
    
    engine.gameState.hudBlink = !engine.gameState.hudBlink;

}