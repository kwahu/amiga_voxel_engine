
#include <exec/types.h>
#include <exec/memory.h>      // MEMF_CLEAR etc
#include <proto/exec.h>
#include <ace/managers/memory.h>


# define MemAlloc(ulSize, ulFlags) MemAllocRls(ulSize, ulFlags)
# define MemFree(pMem, ulSize) MemFreeRls(pMem, ulSize)

// Shorthands
#define MemAllocFast(ulSize) MemAlloc(ulSize, MEMF_ANY) 
#define MemAllocChip(ulSize) MemAlloc(ulSize, MEMF_CHIP)
#define MemAllocFastClear(ulSize) MemAlloc(ulSize, MEMF_ANY | MEMF_CLEAR)
#define MemAllocChipClear(ulSize) MemAlloc(ulSize, MEMF_CHIP | MEMF_CLEAR)
#define MemAllocChipFlags(ulSize, ulFlags) MemAlloc(ulSize, MEMF_CHIP | ulFlags)
#define MemAllocFastFlags(ulSize, ulFlags) MemAlloc(ulSize, MEMF_ANY |ulFlags)


void *MemAllocRls(ULONG size, ULONG flags) {
	SystemUse();
	void *result;
	result = AllocMem(size, flags);
	if(!(flags & MEMF_CHIP) && !result) {
		result = AllocMem(size, (flags & ~MEMF_FAST) | MEMF_ANY);
	}
	SystemUnuse();
	return result;
}

void MemFreeRls(void *mem, ULONG size) {
	SystemUse();
	FreeMem(mem, size);
	SystemUnuse();
}


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
    Arena->Memory = (UBYTE *)MemAllocFast(ArenaSize);
    if(Arena->Memory != 0)
    {
        Arena->CurrentPointer = Arena->Memory;
        Arena->Size = ArenaSize;
        Arena->Used = 0;
    }
    else
    {
        Arena->Memory = (UBYTE *)MemAllocChip(ArenaSize);
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
    Arena->Memory = (UBYTE *)MemAllocChip(ArenaSize);
    if(Arena->Memory != 0)
    {
        Arena->CurrentPointer = Arena->Memory;
        Arena->Size = ArenaSize;
        Arena->Used = 0;
    }
    
}

void DestroyArena(MemoryArena *Arena)
{
    MemFree(Arena->Memory, Arena->Size);
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
