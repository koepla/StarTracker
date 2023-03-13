#ifndef STARTRACKER_ARCH_OS_H
#define STARTRACKER_ARCH_OS_H

// clang-format off
#ifdef _WIN64
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinInet.h>
#endif

#ifdef __linux__

#endif
// clang-format on

namespace arch { }

#endif// STARTRACKER_ARCH_OS_H