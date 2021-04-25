#include <iostream>
#include <iomanip>
#include "ephemeris/elements.hpp"
#include "util.hpp"
#include "ephemeris/date.hpp"

/*
*	1. compute centuries since J2000
*	2. compute argument of perihelion and mean anomaly
*	3. 
*/

int main(int argc, char** argv) {
	
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
	}
	
	return 0;
}