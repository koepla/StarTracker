#pragma once

#include <string>

template <typename T>
struct vec3
{
	T x;
	T y;
	T z;

	vec3() {

	}

	vec3(T x, T y, T z) {

		this->x = x;
		this->y = y;
		this->z = z;
	}

	T mag() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	vec3 norm() const {
		return vec3(x / mag(), y / mag(), z / mag());
	}

	T dot(const vec3& right) const {
		return this->x * right.x + this->y * right.y + this->z * right.z;
	}

	vec3 cross(const vec3& right) const {
		
		return vec3(this->y * right.z - this->z * right.y, this->z * right.x - this->x * right.z, this->x * right.y - this->y - right.x);
	}

	vec3 operator + (const vec3& right) const {
		return vec3(this->x + right.x, this->y + right.y, this->z + right.z);
	}

	vec3 operator - (const vec3& right) const {
		return vec3(this->x - right.x, this->y - right.y, this->z - right.z);
	}

	vec3 operator * (const T& right) const {
		return vec3(this->x * right, this->y * right, this->z * right);
	}

	vec3 operator * (const vec3& right) const {
		return vec3(this->x * right.x, this->y * right.y, this->z * right.z);
	}

	vec3 operator / (const T& right) const {
		return vec3(this->x / right, this->y / right, this->z / right);
	}

	vec3 operator / (const vec3& right) const {
		return vec3(this->x / right.x, this->y / right.y, this->z / right.z);
	}

	vec3& operator += (const vec3& right) {
		this->x += right.x;
		this->y += right.y;
		this->z += right.z;

		return *this;
	}

	vec3& operator -= (const vec3& right) {
		this->x -= right.x;
		this->y -= right.y;
		this->z -= right.z;

		return *this;
	}

	vec3& operator *= (const T& right) {
		this->x *= right;
		this->y *= right;
		this->z *= right;

		return *this;
	}

	vec3& operator *= (const vec3& right) {
		this->x *= right.x;
		this->y *= right.y;
		this->z *= right.z;

		return *this;
	}

	vec3& operator /= (const T& right) {
		this->x /= right;
		this->y /= right;
		this->z /= right;

		return *this;
	}

	vec3& operator /= (const vec3& right) {
		this->x /= right.x;
		this->y /= right.y;
		this->z /= right.z;

		return *this;
	}

	const std::string to_string() const {
		return std::string("(") + std::to_string(this->x) + "," + std::to_string(this->y) + "," + std::to_string(this->z) + ")";
	}

	friend std::ostream& operator << (std::ostream& os, const vec3& right) {
		os << right.str();
		return os;
	}

	bool operator == (const vec3& right) const {
		return (this->x == right.x && this->y == right.y && this->z == right.z);
	}

	bool operator != (const vec3& right) const {
		return !(*this == right);
	}
};

typedef vec3<float> vec3f;
typedef vec3<double> vec3d;
typedef vec3<int> vec3i;
typedef vec3<unsigned int> vec3u;
