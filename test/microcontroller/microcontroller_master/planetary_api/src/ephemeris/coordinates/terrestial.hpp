#pragma once

struct terrestial {

	double latitude;
	double longitude;

	terrestial() {

		this->latitude = 0;
		this->longitude = 0;
	}

	terrestial(double latitude, double longitude) {

		this->latitude = latitude;
		this->longitude = longitude;
	}
};