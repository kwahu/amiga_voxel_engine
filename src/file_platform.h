#pragma once

void LoadBitmapToMemory(BYTE *fileName)    
{                                       
    free(engine.activeBitmap);         
    #ifdef AMIGA                   
    systemUse();                    
    engine.activeBitmap = LoadBitmapFile(fileName, &engine.activeBitmapHeader, engine.activePalette, 1, 0);      
    systemUnuse();        
    #else                   
    engine.activeBitmap = LoadBitmapFile(fileName, &engine.activeBitmapHeader, engine.activePalette, 1, 0);     
    #endif                  
}   