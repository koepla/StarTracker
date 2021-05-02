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
	*	Does not work yet
	*	The error is probably the gmst0
	*/
	
	terrestial observer = terrestial(48.268552, 14.448328);
	spherical venus = spherical(48.8958, 17.7656);

	for (;;) {

		horizontal coords = coordinates::transform::horizontal_position(venus, observer, date::now());

		std::cout << "azimuth: " << coords.azimuth << " ";
		std::cout << "altitude: " << coords.altitude << " ";
	}



	

	return 0;
}