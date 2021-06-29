#include <iostream>
#include <iomanip>
#include "util.hpp"
#include "ephemeris/elements.hpp"
#include "ephemeris/date.hpp"
#include "ephemeris/coordinates/coordinates.hpp"



int main(int argc, char** argv) {
	
	/*
	ephemeris::elements_table table;

	for (int i = 0; i < 10; i++) {

		table.load_table("res/ephemeris_table.csv", date(2021, 4, 25, 3 + i, 0, 0));

		try {
			ephemeris::elements venus = table["Venus"];

			std::cout << std::setfill('0') << std::setw(10) << venus.a << " ";
			std::cout << std::setfill('0') << std::setw(10) << venus.e << " ";
			std::cout << std::setfill('0') << std::setw(10) << venus.i << " ";
			std::cout << std::setfill('0') << std::setw(10) << venus.L << " ";
			std::cout << std::setfill('0') << std::setw(10) << venus.w << " ";
			std::cout << std::setfill('0') << std::setw(10) << venus.O << " " << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}*/

	/*
	*	From now on, we can calculate any stars position,
	*	if the right ascension and declination of the star
	*	is known.
	*/

	/*
	*	Venus testcase
	*	right ascension:	3h 25m 12s
	*	declination:		18° 26' 50"
	*/

	/*
	*	Mars testcase
	*	right ascension:	6h 27m 14s
	*	declination:		24° 43' 23"
	*/

	auto observer = coordinates::terrestial(48.2667, -14.45);
	auto venus = coordinates::spherical(hour_degree(3, 25, 12), real_degree(18, 26, 50));
	auto mars = coordinates::spherical(hour_degree(6, 27, 14), real_degree(24, 43, 23));

	std::cout.precision(10);
	
	while(true) {

		auto mars_coords = coordinates::transform::horizontal_position(mars, observer, date::now());
		auto venus_coords = coordinates::transform::horizontal_position(venus, observer, date::now());

		/*
		*	venus
		*/
		std::cout << "venus " << venus_coords.to_string();

		/*
		*	mars
		*/
		std::cout << " mars " << mars_coords.to_string() << std::endl;
	}
	
	return 0;
}