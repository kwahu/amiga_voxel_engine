#include "engine.h"

void InitGameState()
{

    engine.currentState = State_Game;

    engine.gameState.shipParams.precX = 60 * 100;
    engine.gameState.shipParams.precZ = 0;
    engine.gameState.shipParams.precY = 20 * 100;
    engine.gameState.shipParams.pX = 0;
    engine.gameState.shipParams.pZ = 0;
    engine.gameState.shipParams.pY = 0;
    engine.gameState.shipParams.dPDenom = 128;
    engine.gameState.shipParams.dP = 0;
    engine.gameState.shipParams.ddP = 0;
    engine.gameState.shipParams.relHeight = 0;

    engine.gameState.crossHairX = 0;
    engine.gameState.crossHairY = 0;
	engine.gameState.points = 0;
    engine.gameState.runOver = 0;
    engine.renderer.lastOverwrittenLine = 0;

 

    CopyMapWord(engine.renderer.mapSource[0], engine.renderer.mapHigh);
    
}

void UpdatePlayerPosition()
{
    engine.gameState = ProcessInput(engine.gameState, engine.deltaTime);

    ULONG lowerDelta = (engine.deltaTime/10000);

    UWORD terrainHeight = getTerrainHeight(engine.gameState.shipParams, engine.renderer.mapHigh);

    engine.gameState = updateShipParams(engine.gameState, lowerDelta, terrainHeight);

    LONG addedpoints = (lowerDelta)*(128 - engine.gameState.shipParams.relHeight);
    if(addedpoints > 0)
    {
        engine.gameState.points += addedpoints;
    }

    
    OverwriteMap(); //this is how we go through many different maps, we just overwrite the main array with new content

}

BYTE CheckPlayerCollision()
{
    return (engine.gameState.shipParams.pY - 3) < (UBYTE)(engine.renderer.mapHigh[((UBYTE)(engine.gameState.shipParams.pX)) >>1][((UBYTE)(engine.gameState.shipParams.pZ + 15)) >>1]);
}

void RenderShipAndCrossHair()
{
    UWORD crossPX =  (160 + (engine.gameState.crossHairX / 400));
    UWORD crossPY = ( 130 + (engine.gameState.crossHairY / 400) );

    UWORD shipDirX = 160 + (engine.gameState.crossHairX/1600);
    UWORD shipDirY = 135 + (engine.gameState.crossHairY/1600);

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

    DrawPixel( crossPX, crossPY + 4, 0);
    DrawPixel( crossPX, crossPY - 4, 0);
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

    DrawTextBitmap(engine.pBitmapVelocity, 100, 225, 12);
    DrawTextBitmap(engine.pBitmapScore, 35, 225, 12);
    DrawTextBitmap(engine.pBitmapHeight, 195, 225, 12);
    DrawTextBitmap(engine.pBitmapTime, 250, 225, 12);
    DrawTextBitmap(engine.pBitmapVelocityLabel, 75, 225, 12);
    DrawTextBitmap(engine.pBitmapScoreLabel, 0, 225, 12);
    DrawTextBitmap(engine.pBitmapHeightLabel, 135, 225, 12);
    DrawTextBitmap(engine.pBitmapTimeLabel, 230, 225, 12);

}

void RunGameState()
{
    
    UpdatePlayerPosition();
    if(CheckPlayerCollision())
    {
        ShowCutscene(Cutscene_Death);
    }
    else if(engine.gameState.runOver)
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
    else
    {
    
        ProcessQualityInput();
        

        
        if(engine.gameState.shipParams.pZ > 11*256 && engine.gameState.shipParams.dPDenom < 3*255)
        {
            engine.gameState.shipParams.dPDenom = engine.gameState.shipParams.dPDenom + 4;
        }
        else if(engine.gameState.shipParams.dPDenom >= 3*255)
        {
            engine.gameState.runOver = 1;
        }
    


        engine.renderer.xOffsetOdd = engine.gameState.crossHairX / 600;
        engine.renderer.xOffsetEven = engine.gameState.crossHairX / 900;

        RenderQuality();

    //draw crosshair
        
        RenderShipAndCrossHair();
        
                //draw only even lines 
        
        DrawGameStats();
                            

        VSyncAndDraw();


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
    }
    


}