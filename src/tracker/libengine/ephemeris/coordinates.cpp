#include "coordinates.hpp"
#include "libengine/math.hpp"

namespace ephemeris {

    f64 Vector3::Length() const noexcept {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }

    Matrix3x3::Matrix3x3(f64 diagonal) noexcept : elements() {
        elements[0][0] = diagonal;
        elements[1][1] = diagonal;
        elements[2][2] = diagonal;
    }

    Matrix3x3 Matrix3x3::Transpose() const noexcept {
        Matrix3x3 result{};
        result[0] = { elements[0][0], elements[1][0], elements[2][0] };
        result[1] = { elements[0][1], elements[1][1], elements[2][1] };
        result[2] = { elements[0][2], elements[1][2], elements[2][2] };
        return result;
    }

    std::array<f64, 3>& Matrix3x3::operator[](usize index) noexcept {
        return elements[index];
    }

    const std::array<f64, 3>& Matrix3x3::operator[](usize index) const noexcept {
        return elements[index];
    }

    Vector3 Matrix3x3::operator*(const Vector3& right) noexcept {
        Vector3 result{};
        result.X = elements[0][0] * right.X + elements[0][1] * right.Y + elements[0][2] * right.Z;
        result.Y = elements[1][0] * right.X + elements[1][1] * right.Y + elements[1][2] * right.Z;
        result.Z = elements[2][0] * right.X + elements[2][1] * right.Y + elements[2][2] * right.Z;
        return result;
    }

    Matrix3x3 Matrix3x3::operator*(const Matrix3x3& right) noexcept {
        Matrix3x3 result{};
        result[0][0] = elements[0][0] * right[0][0] + elements[0][1] * right[1][0] + elements[0][2] * right[2][0];
        result[0][1] = elements[0][0] * right[0][1] + elements[0][1] * right[1][1] + elements[0][2] * right[2][1];
        result[0][2] = elements[0][0] * right[0][2] + elements[0][1] * right[1][2] + elements[0][2] * right[2][2];

        result[1][0] = elements[1][0] * right[0][0] + elements[1][1] * right[1][0] + elements[1][2] * right[2][0];
        result[1][1] = elements[1][0] * right[0][1] + elements[1][1] * right[1][1] + elements[1][2] * right[2][1];
        result[1][2] = elements[1][0] * right[0][2] + elements[1][1] * right[1][2] + elements[1][2] * right[2][2];

        result[2][0] = elements[2][0] * right[0][0] + elements[2][1] * right[1][0] + elements[2][2] * right[2][0];
        result[2][1] = elements[2][0] * right[0][1] + elements[2][1] * right[1][1] + elements[2][2] * right[2][1];
        result[2][2] = elements[2][0] * right[0][2] + elements[2][1] * right[1][2] + elements[2][2] * right[2][2];
        return result;
    }

    bool operator==(const Matrix3x3& left, const Matrix3x3& right) noexcept {
        return left[0] == right[0] && left[1] == right[1] && left[2] == right[2];
    }

    bool operator!=(const Matrix3x3& left, const Matrix3x3& right) noexcept {
        return !(left == right);
    }

    Matrix3x3 RotationMatrix(RotationAxis axis, f64 angle) noexcept {
        Matrix3x3 matrix{};
        const auto cosAngle = math::Cosine(angle);
        const auto sinAngle = math::Sine(angle);
        switch (axis) {
            case RotationAxis::X: {
                matrix[0] = { 1.0, 0.0, 0.0 };
                matrix[1] = { 0.0, cosAngle, -sinAngle };
                matrix[2] = { 0.0, sinAngle, cosAngle };
                return matrix;
            }
            case RotationAxis::Y: {
                matrix[0] = { cosAngle, 0.0, sinAngle };
                matrix[1] = { 0.0, 1.0, 0.0 };
                matrix[2] = { -sinAngle, 0.0, cosAngle };
                break;
            }
            case RotationAxis::Z: {
                matrix[0] = { cosAngle, -sinAngle, 0.0 };
                matrix[1] = { sinAngle, cosAngle, 0.0 };
                matrix[2] = { 0.0, 0.0, 1.0 };
                return matrix;
            }
        }
        return matrix;
    }

    Vector3 operator+(const Vector3& left, const Vector3& right) noexcept {
        return { left.X + right.X, left.Y + right.Y, left.Z + right.Z };
    }

    Vector3 operator-(const Vector3& left, const Vector3& right) noexcept {
        return { left.X - right.X, left.Y - right.Y, left.Z - right.Z };
    }

    f64 EclipticDrift(f64 julianCenturies) noexcept {
        // Correct for drifting ecliptic due to planets pull on the Earth
        return 23.43929111 -
               (46.8150 + (0.00059 - 0.001813 * julianCenturies) * julianCenturies) * julianCenturies / 3600.0;
    }

    Matrix3x3 ReferencePlaneMatrix(ReferencePlane from, ReferencePlane to, f64 at) noexcept {
        if (from == to) {
            return Matrix3x3(1.0);
        }

        const auto rotation = RotationMatrix(RotationAxis::X, EclipticDrift(at));
        if (from == ReferencePlane::Equatorial && to == ReferencePlane::Ecliptic) {
            return rotation.Transpose();
        }
        return rotation;
    }

    Matrix3x3 PrecessionMatrix(ReferencePlane referencePlane, f64 t1, f64 t2) noexcept {
        const auto dt = t2 - t1;
        switch (referencePlane) {
            case ReferencePlane::Ecliptic: {
                const auto Pi = math::Radians(174.876383889) +
                                (((3289.4789 + 0.60622 * t1) * t1) + ((-869.8089 - 0.50491 * t1) + 0.03536 * dt) * dt) /
                                        math::ARCS;
                const auto pi = ((47.0029 - (0.06603 - 0.000598 * t1) * t1) +
                                 ((-0.03302 + 0.000598 * t1) + 0.00006 * dt) * dt) *
                                dt / math::ARCS;
                const auto pa = ((5029.0966 + (2.22226 - 0.000042 * t1) * t1) +
                                 ((1.11113 - 0.000042 * t1) - 0.000006 * dt) * dt) *
                                dt / math::ARCS;
                return RotationMatrix(RotationAxis::Z, math::Degrees(Pi + pa)) *
                       RotationMatrix(RotationAxis::X, math::Degrees(-pi)) *
                       RotationMatrix(RotationAxis::Z, math::Degrees(-Pi));
            }
            case ReferencePlane::Equatorial: {
                const auto zeta = ((2306.2181 + (1.39646 - 0.000139 * t1) * t1) +
                                   ((0.30188 - 0.000344 * t1) + 0.017998 * dt) * dt) *
                                  dt / math::ARCS;
                const auto z = zeta + ((0.7928 + 0.000411 * t1) + 0.000205 * dt) * dt * dt / math::ARCS;
                const auto theta = ((2004.3109 - (0.85330 + 0.000217 * t1) * t1) -
                                    ((0.42664 + 0.000217 * t1) + 0.041833 * dt) * dt) *
                                   dt / math::ARCS;
                return RotationMatrix(RotationAxis::Z, math::Degrees(z)) *
                       RotationMatrix(RotationAxis::Y, math::Degrees(-theta)) *
                       RotationMatrix(RotationAxis::Z, math::Degrees(zeta));
            }
        }
        return {};
    }

    Vector3 EquatorialToVector(const Equatorial& coords) noexcept {
        Vector3 rectCoords{};
        rectCoords.X = coords.Radius * math::Cosine(coords.RightAscension) * math::Cosine(coords.Declination);
        rectCoords.Y = coords.Radius * math::Sine(coords.RightAscension) * math::Cosine(coords.Declination);
        rectCoords.Z = coords.Radius * math::Sine(coords.Declination);
        return rectCoords;
    }

    Equatorial VectorToEquatorial(const Vector3& vector) noexcept {
        const auto x = vector.X;
        const auto y = vector.Y;
        const auto z = vector.Z;

        Equatorial sphericalCoords{};
        sphericalCoords.Radius = sqrt(x * x + y * y + z * z);
        sphericalCoords.RightAscension = math::ArcTangent2(y, x);
        sphericalCoords.Declination = math::ArcTangent2(z, sqrt(x * x + y * y));
        return sphericalCoords;
    }

    Horizontal LocalEquatorialToHorizontal(f64 declination, f64 hourAngle, f64 latitude) noexcept {
        Equatorial sphericalPosition{};
        sphericalPosition.RightAscension = hourAngle;
        sphericalPosition.Declination = declination;
        sphericalPosition.Radius = 1.0;

        const auto positionVector = EquatorialToVector(sphericalPosition);
        const auto rotatedVector = RotationMatrix(RotationAxis::Y, -(90 - latitude)) * positionVector;

        // add 180 to get the angle from north to east to south and so on
        Horizontal horizontalCoords{};
        horizontalCoords.Azimuth = math::ArcTangent2(rotatedVector.Y, rotatedVector.X) + 180.0;
        horizontalCoords.Altitude = math::ArcSine(rotatedVector.Z);
        return horizontalCoords;
    }

    Horizontal ObserveGeographic(const Equatorial& sphericalCoords,
                                 const Geographic& observer,
                                 const DateTime& date) noexcept {
        auto utcTime = DateTime::Utc(date);
        const auto localMeanSiderealTime = DateTime::GreenwichMeanSiderealTime(utcTime) + observer.Longitude;
        const auto hourAngle = localMeanSiderealTime - sphericalCoords.RightAscension;
        return LocalEquatorialToHorizontal(sphericalCoords.Declination, hourAngle, observer.Latitude);
    }
}// namespace ephemeris