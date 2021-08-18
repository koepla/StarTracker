#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include "../core.hpp"
#include <windows.h>
#include <string>

class Hardware
{
public:
	[[nodiscard]] static std::string GetUID() noexcept;
};

#endif // _HARDWARE_H_