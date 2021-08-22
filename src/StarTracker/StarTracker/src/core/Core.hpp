#ifndef _STAR_CORE_H_
#define _STAR_CORE_H_

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

#define STR_ASSERT(c, ...) if(!(c)) { MessageBoxA(nullptr, c, "Startracker Assertion", NULL); }

#endif

#endif // _STAR_CORE_H_
