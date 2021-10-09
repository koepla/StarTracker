#ifndef STARTRACKER_CORE_H
#define STARTRACKER_CORE_H

#include <cstdio>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else

#error "Only Windows is supported!"

#endif

#ifdef _DEBUG

#define STR_ASSERT(c, m) if(!(c)) { fprintf(stderr, "[STARTRACKER ASSERTION] %s\n" m); __debugbreak(); }

#else

#define STR_ASSERT(c, m) if(!(c)) { MessageBoxA(nullptr, m, "Startracker Assertion", NULL); }

#endif

#endif // STARTRACKER_CORE_H
