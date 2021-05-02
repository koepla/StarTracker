#pragma once

#include <string>

struct horizontal {

	double azimuth;
	double altitude;

	horizontal() {

		this->azimuth = 0;
		this->altitude = 0;
	}

	horizontal(double azimuth, double altitude) {

		this->azimuth = azimuth;
		this->altitude = altitude;
	}

	std::string to_string() const {

		return "[azimuth = " + std::to_string(this->azimuth) + " | altitude = " + std::to_string(this->altitude) + "]";
	}
};