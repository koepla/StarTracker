#ifndef _CORE_H_
#define _CORE_H_

#include <cstdio>

#define STR_ASSERT(condition, msg) if(!(condition)){ fprintf(stderr, "%s", msg); __debugbreak(); } 

#endif // _CORE_H_