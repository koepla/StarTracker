#pragma once

#define STA_ASSERT(condition, msg) if(!(condition)){ fprintf(stderr, "%s", msg); __debugbreak(); } 