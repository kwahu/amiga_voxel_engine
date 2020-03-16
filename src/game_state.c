#include "engine.h"

void InitGameState()
{
    ResetTime();

    engine.currentState = State_Game;

    engine.gameState.shipParams.precX = 50 * 100;
    engine.gameState.shipParams.precZ = 0;
    engine.gameState.shipParams.precY = 60 * 100;
    engine.gameState.shipParams.pX = 0;
    engine.gameState.shipParams.pZ = 0;
    engine.gameState.shipParams.pY = 0;
    engine.gameState.shipParams.dPDenom = 160;
    engine.gameState.shipParams.dP = 0;
    engine.gameState.shipParams.ddP = 0;
    engine.gameState.shipParams.relHeight = 0;

    engine.gameState.crossHairX = 0;
    engine.gameState.crossHairY = 0;
	engine.gameState.points = 0;
    engine.gameState.runOver = 0;
    engine.renderer.lastOverwrittenLine = 0;

    for(UBYTE i = 0; i < 40; ++i)
    {
        engine.renderer.depthBuffer[i] = 255;
    }
 

    engine.renderer.mapHigh = engine.renderer.mapSource;
	//CopyMapWord(engine.renderer.mapSource, engine.renderer.mapHigh);
    
}

void UpdatePlayerPosition()
{
    ProcessInput();


    UWORD terrainHeight = getTerrainHeight(engine.gameState.shipParams, engine.renderer.mapHigh);

    ULONG lowerDelta = (engine.deltaTime/10000);
    updateShipParams(lowerDelta, terrainHeight);

    LONG addedpoints = (lowerDelta)*(112 - engine.gameState.shipParams.relHeight);
    if(addedpoints > 0 && engine.gameState.shipParams.pZ < 2816)
    {
        engine.gameState.points += addedpoints;
    }


}

BYTE CheckPlayerCollision()
{
    UBYTE shipCollisionY = (engine.gameState.shipParams.pY - 2);
    UWORD shipCollisionX = engine.gameState.shipParams.pX;

    return shipCollisionY < (UBYTE)(engine.renderer.mapHigh[(((UBYTE)(shipCollisionX)) >> 1)*11*MAPSIZE + (((UBYTE)(engine.renderer.zStart + 8)) >> 1)]);
    
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
        spriteIndexY = 0;
    }
    else if(engine.gameState.crossHairY < -4000)
    {
        spriteIndexY = 2;
    }

                
    UBYTE bestFit = engine.renderer.depthBufferHeight + 1;
    WORD bestValue = 1024;

    for(BYTE i = 0; i < engine.renderer.depthBufferHeight/2; ++i)
    {
        UBYTE d = (engine.renderer.depthBuffer[i]);
        WORD depth = (WORD)d;
        WORD offset = (WORD)engine.renderer.zStart + (WORD)((WORD)(engine.renderer.depthBufferHeight/4) - i);
        WORD value = (depth - offset);
        if((value > 0) && (value < bestValue) && (depth > engine.renderer.zStart + 1))
        {
            bestValue = value;
            bestFit = i;
        }
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
                     spriteIndexX, spriteIndexY, 48, 48, 3);
    
     
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

    DrawTextBitmapOverwrite(engine.pBitmapVelocity, 100, PLANEHEIGHT-6, 12);
    DrawTextBitmapOverwrite(engine.pBitmapScore, 35, PLANEHEIGHT-6, 12);
    DrawTextBitmapOverwrite(engine.pBitmapHeight, 195, PLANEHEIGHT-6, 12);
    DrawTextBitmapOverwrite(engine.pBitmapTime, 250, PLANEHEIGHT-6, 12);
    DrawTextBitmap(engine.pBitmapVelocityLabel, 75, PLANEHEIGHT-6, 12);
    DrawTextBitmap(engine.pBitmapScoreLabel, 0, PLANEHEIGHT-6, 12);
    DrawTextBitmap(engine.pBitmapHeightLabel, 135, PLANEHEIGHT-6, 12);
    DrawTextBitmap(engine.pBitmapTimeLabel, 230, PLANEHEIGHT-6, 12);

}

void RunGameState()
{


    //ProcessQualityInput();
    

    
    if(engine.gameState.shipParams.pZ > 11*256 && engine.gameState.shipParams.dPDenom < 3*255)
    {
        engine.gameState.crossHairX = 0;

        if((UWORD)engine.gameState.shipParams.relHeight > 0)
        {
            UWORD offset = (((UWORD)engine.gameState.shipParams.relHeight)*200);
            if(offset > 0x7FFF)
            {
                offset = 0x7FFF;
            }
            engine.gameState.crossHairY = offset;
        }

        UWORD terrainHeight = getTerrainHeight(engine.gameState.shipParams, engine.renderer.mapHigh);

        ULONG lowerDelta = (engine.deltaTime/10000);
        updateShipParams(lowerDelta, terrainHeight);

        
        

        engine.gameState.shipParams.dPDenom = engine.gameState.shipParams.dPDenom + 6;
    }
    else if(engine.gameState.shipParams.dPDenom >= 3*255)
    {
        engine.gameState.runOver = 1;
    }
    else
    {
        
        UpdatePlayerPosition();
        if(CheckPlayerCollision())
        {
            ShowCutscene(Cutscene_Death);
        }
        
    }
    
    
    OverwriteMap(); //this is how we go through many different maps, we just overwrite the main array with new content



    engine.renderer.xTurnOffset = engine.gameState.crossHairX / engine.renderer.turnDenom;


    RenderQuality();

//draw crosshair
    
    RenderShipAndCrossHair();
    
            //draw only even lines 
    
                        

    VSyncAndDraw();
    DrawGameStats();


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
        if(engine.gameState.points < 1000000)
        {
            ShowCutscene(Cutscene_TooLate);
        }
        else
        {
            ShowCutscene(Cutscene_Win);

        }
    }
    


}