#ifndef LIBTRACKER_BEZIER_H
#define LIBTRACKER_BEZIER_H

#include "ui.hpp"
#include "utility/types.hpp"

struct BezierPoint {
    f32 X;
    f32 Y;
};

struct BezierCurve {
    BezierPoint First;
    BezierPoint Last;
    f32 Minimum;
    f32 Maximum;
    f32 Left;
    f32 Right;
};

bool BezierEditor(std::string_view label, BezierCurve& curve, f32 speed, f32 min = 0.0f, f32 max = 0.0f) noexcept;

#endif// LIBTRACKER_BEZIER_H
