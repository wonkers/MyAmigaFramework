#pragma once
#include <proto/exec.h>

#define size_t unsigned long

void* operator new(size_t size);
void* operator new[](size_t size);

void operator delete(void* obj);
void operator delete(void* obj, size_t i);
void operator delete[](void* obj, size_t i);
void operator delete[](void* obj);