#pragma once

void LoadBitmapToMemory(BYTE *fileName)    
{                                      
    #ifdef AMIGA                   
    systemUse();                        
    free(engine.activeBitmap);      
    engine.activeBitmap = LoadBitmapFile(fileName, &engine.activeBitmapHeader, engine.activePalette, 1, 0);      
    systemUnuse();        
    #else                       
    free(engine.activeBitmap);      
    engine.activeBitmap = LoadBitmapFile(fileName, &engine.activeBitmapHeader, engine.activePalette, 1, 0);     
    #endif                  
}   