#pragma once

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
};