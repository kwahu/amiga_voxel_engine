
typedef struct MemoryArena
{
    UBYTE tag[5];
    ULONG Size;
    ULONG Used;
    UBYTE *Memory;
    UBYTE *CurrentPointer;
} MemoryArena;

void NewArena(MemoryArena *Arena, ULONG ArenaSize)
{
    Arena->tag[0] = 'a';
    Arena->tag[1] = 'r';
    Arena->tag[2] = 'e';
    Arena->tag[3] = 'n';
    Arena->tag[4] = 'a';
    Arena->Memory = (UBYTE *)memAllocFast(ArenaSize);
    if(Arena->Memory != 0)
    {
        Arena->CurrentPointer = Arena->Memory;
        Arena->Size = ArenaSize;
        Arena->Used = 0;
    }
    else
    {
        Arena->Memory = (UBYTE *)memAllocChip(ArenaSize);
        if(Arena->Memory != 0)
        {
            Arena->CurrentPointer = Arena->Memory;
            Arena->Size = ArenaSize;
            Arena->Used = 0;
        }
    }
    
    
}


void NewChipArena(MemoryArena *Arena, ULONG ArenaSize)
{
    
    Arena->tag[0] = 'c';
    Arena->tag[1] = 'a';
    Arena->tag[2] = 'r';
    Arena->tag[3] = 'e';
    Arena->tag[4] = 'n';
    Arena->Memory = (UBYTE *)memAllocChip(ArenaSize);
    if(Arena->Memory != 0)
    {
        Arena->CurrentPointer = Arena->Memory;
        Arena->Size = ArenaSize;
        Arena->Used = 0;
    }
    
}

void DestroyArena(MemoryArena *Arena)
{
    memFree(Arena->Memory, Arena->Size);
}


void ClearArena(MemoryArena *Arena)
{
    Arena->CurrentPointer = Arena->Memory;
    Arena->Used = 0;
}

UBYTE *AllocateFromArena(MemoryArena *Arena, ULONG spaceSize)
{
    UBYTE *Result = 0;
    if((Arena->Used + spaceSize) <= Arena->Size)
    {
        Result = Arena->CurrentPointer;
        Arena->CurrentPointer += spaceSize;
        Arena->Used += spaceSize;
    }

    return Result;
}

void NewSubArena(MemoryArena *Arena, MemoryArena *SubArena, ULONG ArenaSize)
{
    
    SubArena->tag[0] = 's';
    SubArena->tag[1] = 'a';
    SubArena->tag[2] = 'r';
    SubArena->tag[3] = 'e';
    SubArena->tag[4] = 'n';
    SubArena->Memory = (UBYTE *)AllocateFromArena(Arena, ArenaSize);
    if(SubArena->Memory != 0)
    {
        SubArena->CurrentPointer = SubArena->Memory;
        SubArena->Size = ArenaSize;
        SubArena->Used = 0;
    }
    
}
