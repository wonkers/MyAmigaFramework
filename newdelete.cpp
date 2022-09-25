#pragma once
#include "newdelete.h"

void* operator new(size_t size)
{
	return AllocMem(size, MEMF_FAST|MEMF_CLEAR);
}
void* operator new[](size_t size)
{
	return AllocMem(size, MEMF_FAST|MEMF_CLEAR);
}

void operator delete(void* obj)
{
	FreeMem(obj, sizeof(obj));
}
void operator delete(void* obj, size_t i)
{
	FreeMem(obj, i);
}
void operator delete[](void* obj, size_t i)
{
	FreeMem(obj, i);
}
void operator delete[](void* obj)
{
	FreeMem(obj, sizeof(obj));
}