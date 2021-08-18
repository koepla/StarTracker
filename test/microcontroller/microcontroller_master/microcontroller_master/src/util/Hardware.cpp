#include "Hardware.hpp"

std::string Hardware::GetUID() noexcept
{
	HW_PROFILE_INFOA hwProfileInfo;
	if (!GetCurrentHwProfileA(&hwProfileInfo)) {

		STA_ASSERT(false, "Couldn't get current hardware profile!");
		return std::string();
	}

	return std::string(hwProfileInfo.szHwProfileGuid);
}
