#ifndef LIBENGINE_EPHEMERIS_COORDINATES_H
#define LIBENGINE_EPHEMERIS_COORDINATES_H

#include "utility/types.hpp"

#include "coordinates.hpp"
#include "date-time.hpp"

/**
 * @brief Calculations are based on the book "Astronomie mit dem Personal Computer"
 * For further reference, see http://www.stjarnhimlen.se/comp/tutorial.html
 */
namespace ephemeris {

    struct Equatorial {
        f64 Radius;
        f64 RightAscension;
        f64 Declination;
    };

    struct Horizontal {
        f64 Azimuth;
        f64 Altitude;
    };

    /**
     * @brief Latitude positive in north, negative in south
     * Longitude positive in east, negative in west
     */
    struct Geographic {
        f64 Latitude;
        f64 Longitude;
    };


    class Vector3 {
    public:
        f64 X;
        f64 Y;
        f64 Z;

        f64 Length() const noexcept;
        friend Vector3 operator+(const Vector3& left, const Vector3& right) noexcept;
        friend Vector3 operator-(const Vector3& left, const Vector3& right) noexcept;
    };

    class Matrix3x3 {
    public:
        std::array<std::array<f64, 3>, 3> elements{};

        /**
         * Default constructor
         */
        Matrix3x3() noexcept = default;

        /**
         * Create diagonal matrix
         * @param diagonal
         */
        explicit Matrix3x3(f64 diagonal) noexcept;

        /**
         * Transposed version of the matrix
         * @return
         */
        Matrix3x3 Transpose() const noexcept;

        /**
         * Subscript operator for accessing matrix elements
         * @param index Index
         * @return array
         */
        std::array<f64, 3>& operator[](usize index) noexcept;

        /**
         * Subscript operator for accessing const matrix elements
         * @param index Index
         * @return const array
         */
        const std::array<f64, 3>& operator[](usize index) const noexcept;

        /**
         * Multiply with vector
         * @param right vector
         * @return multiplication result
         */
        Vector3 operator*(const Vector3& right) noexcept;

        /**
         * Multiply with matrix3x3
         * @param right matrix
         * @return multiplication result
         */
        Matrix3x3 operator*(const Matrix3x3& right) noexcept;

        friend bool operator==(const Matrix3x3& left, const Matrix3x3& right) noexcept;
        friend bool operator!=(const Matrix3x3& left, const Matrix3x3& right) noexcept;
    };

    enum class RotationAxis {
        X, Y, Z
    };

    /**
     * Create a new rotation matrix
     * @param axis Axis to rotate around
     * @param angle Angle
     * @return
     */
    Matrix3x3 RotationMatrix(RotationAxis axis, f64 angle) noexcept;

    /**
     * Computes the ecliptic drift since J2000
     * @param julianCenturies Julian Centuries since J2000
     * @return ecliptic drift in degrees
     */
    f64 EclipticDrift(f64 julianCenturies) noexcept;

    enum class ReferencePlane { Equatorial, Ecliptic };

    /**
     * Transforms from one reference plane to another
     * @param from Initial reference plane
     * @param to Reference frame that we want to transform to
     * @param at Time in julian centuries since J2000
     * @return transformation matrix
     */
    Matrix3x3 ReferencePlaneMatrix(ReferencePlane from, ReferencePlane to, f64 at) noexcept;

    /**
     * Matrix for the precession of coordinates
     * @param referencePlane Plane of reference
     * @param t1 Current epoch
     * @param t2 Epoch to that we want to precess
     * @note `t1` and `t2` are both in julian centuries since j2000
     * @return transformation matrix
     */
    Matrix3x3 PrecessionMatrix(ReferencePlane referencePlane, f64 t1, f64 t2) noexcept;

    /**
     * @brief Transform equatorial coordinates to a Vector3
     * @param coords The equatorial coordinates
     * @return The transformed rectangular coordinates
     */
    Vector3 EquatorialToVector(const Equatorial& coords) noexcept;

    /**
     * @brief Transform a Vector3 to equatorial coordinates
     * @param vector The rectangular coordinates
     * @return The transformed spherical coordinates
     */
    Equatorial VectorToEquatorial(const Vector3& vector) noexcept;

    /**
     * @brief Transforms Equatorial coordinates to Horizontal ones
     * @param declination Declination in degrees
     * @param hourAngle HourAngle (by GMST) in degrees
     * @param latitude Latitude in degrees
     * @return The transformed horizontal coordinates
     */
    Horizontal LocalEquatorialToHorizontal(f64 declination, f64 hourAngle, f64 latitude) noexcept;

    /**
     * @brief Computes the Horizontal position of an object with spherical coordinates
     * @param sphericalCoords The spherical coordinates of the object
     * @param observer The geographic coordinates of the observer
     * @param date The date and time for the computation
     * @return the Computed horizontal coordinates
     */
    Horizontal ObserveGeographic(const Equatorial& sphericalCoords,
                                 const Geographic& observer,
                                 const DateTime& date) noexcept;
}// namespace ephemeris

#endif// LIBENGINE_EPHEMERIS_COORDINATES_H