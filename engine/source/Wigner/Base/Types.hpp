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
}