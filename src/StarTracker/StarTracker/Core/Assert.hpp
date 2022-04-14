#pragma once
#ifndef STARTRACKER_CORE_ASSERT_H
#define STARTRACKER_CORE_ASSERT_H

#include <cstdio>

#ifdef _DEBUG
#define ASSERT(expr) if (!(expr)) { fprintf(stderr, "[-] Assertion failed on expression %s in file %s on line %d!\n", #expr, __FILE__, __LINE__); __debugbreak(); } 
#else
#define ASSERT(expr) (void)(expr)
#endif

#endif // STARTRACKER_CORE_ASSERT_H
