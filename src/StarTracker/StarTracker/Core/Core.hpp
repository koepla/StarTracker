#ifndef STARTRACKER_CORE_H
#define STARTRACKER_CORE_H

#include <StarTracker/Core/Allocator.hpp>

void* operator new(std::size_t size);
void operator delete(void* block);

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#else
#error "Only Windows is supported!"
#endif
#endif // STARTRACKER_CORE_H
