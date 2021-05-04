#pragma once
#include <cmath>

#define PI 3.14159265355897L
#define PI2 (2 * PI)

double inline frac(double x) {

	return x - floor(x);
}

double inline mod(double a, double b) {

	return b * frac(a / b);
}

double inline radians2deg(double angle) {
    return angle * (180.0 / PI);
}

double inline deg2radians(double angle) {
    return angle * (PI / 180.0);
}

double inline sin_deg(double angle) {
    return sin(deg2radians(angle));
}

double inline cos_deg(double angle) {
    return cos(deg2radians(angle));
}

double inline tan_deg(double angle) {
    return tan(deg2radians(angle));
}

double inline asin_deg(double angle) {
    return radians2deg(asin(angle));
}

double inline acos_deg(double angle) {
    return radians2deg(acos(angle));
}

double inline atan_deg(double angle) {
    return radians2deg(atan(angle));
}

double inline atan2_deg(double x, double y) {
    return radians2deg(atan2(x, y));
}

double inline real_degree(double degree, double arcmin, double arcsec) {

    return (degree + arcmin / 60.0L + arcsec / 3600.0L);
}

double inline hour_degree(double hour, double minute, double sec) {

    return 15.0L * (hour + minute / 60.0L + sec / 3600.0L);
}