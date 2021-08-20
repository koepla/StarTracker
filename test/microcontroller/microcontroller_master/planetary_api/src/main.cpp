#include <iostream>
#include <iomanip>
#include "Astro.hpp"

int main(int argc, char** argv) {

	auto observer = Astro::Coordinates::Terrestial(48.30694, -14.28583);
	auto mars = Astro::Coordinates::Spherical(Astro::Math::HourToDegrees(11, 1, 42), Astro::Math::RealDegrees(7, 17, 7.6));
	auto venus = Astro::Coordinates::Spherical(Astro::Math::HourToDegrees(12, 20, 11), Astro::Math::RealDegrees(-1, 39, 24.05));

	std::cout.precision(10);
	
	while(true) {

		auto marsCoords = Astro::Coordinates::Transform::TerrestialObserverToHorizontal(mars, observer, Astro::Date::Now());
		auto venusCoords = Astro::Coordinates::Transform::TerrestialObserverToHorizontal(venus, observer, Astro::Date::Now());

		/*
		*	Mars
		*/
		std::cout << "Mars " << marsCoords.ToString();

		/*
		*	Venus
		*/
		std::cout << " Venus " << venusCoords.ToString() << std::endl;
	}
	
	return 0;
}