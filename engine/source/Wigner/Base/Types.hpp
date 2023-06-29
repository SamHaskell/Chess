#pragma once

#include "Wigner/Base/Defines.hpp"

namespace Wigner {
    struct Point2D {
        f32 X;
        f32 Y;
    };

    struct Rect2D {
        f32 X;
        f32 Y;
        f32 Width;
        f32 Height;
    };

    inline Point2D rect_get_center(Rect2D rect) {
        return Point2D({(rect.Width + rect.X) / 2.0f, (rect.Height + rect.Y) / 2.0f});
    }

    inline bool point_in_rect(Point2D point, Rect2D rect) {
        return (point.X >= rect.X && point.Y >= rect.Y && point.X <= rect.X + rect.Width && point.Y <= rect.Y + rect.Height);
    }
}