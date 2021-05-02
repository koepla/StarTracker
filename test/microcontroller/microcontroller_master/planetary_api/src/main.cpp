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
			ephemeris::elements venus = table["Mars"];

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
	*	Sun testcase
	*	right ascension:	26.658°
	*	declination:		11.0084°
	*/

	/*
	*	Venus testcase
	*	right ascension:	3h 18m 47.7s
	*	declination:		18° 2.8"
	*/

	/*
	*	Mars testcase
	*	right ascension:	6h 24m 25s
	*	declination:		24° 45' 46.2"
	*/

	terrestial observer = terrestial(48.2667, -14.45);

	spherical sun = spherical(26.658, 11.0084);
	spherical venus = spherical(49.6958, 18.0500);
	spherical mars = spherical(96.1042, 24.7628);

	auto sun_coords = coordinates::transform::horizontal_position(sun, observer, date::now());
	auto venus_coords = coordinates::transform::horizontal_position(venus, observer, date::now());
	auto mars_coords = coordinates::transform::horizontal_position(mars, observer, date::now());

	/*
	*	venus output
	*/

	/*
	*	Sun
	*/
	std::cout << sun_coords.to_string() << std::endl;

	/*
	*	venus
	*/
	std::cout << venus_coords.to_string() << std::endl;


	/*
	*	mars
	*/
	std::cout << mars_coords.to_string() << std::endl;
	
	return 0;
}