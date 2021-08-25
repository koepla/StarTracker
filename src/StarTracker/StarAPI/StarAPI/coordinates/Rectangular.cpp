#include "Rectangular.hpp"

namespace Star::Coordinates {

	Rectangular::Rectangular() : X(0), Y(0), Z(0) {

	}

	Rectangular::Rectangular(double x, double y, double z) : X(x), Y(y), Z(z) {

	}

	std::string Rectangular::ToString() const {

		return "[x = " + std::to_string(X) + " | y = " + std::to_string(Y) + " | z = " + std::to_string(Z) + "]";
	}
}