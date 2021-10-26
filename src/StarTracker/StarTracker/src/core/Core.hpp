#ifndef STARTRACKER_CORE_H
#define STARTRACKER_CORE_H

#include <cstdio>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else

#error "Only Windows is supported!"

#endif

#endif // STARTRACKER_CORE_H
