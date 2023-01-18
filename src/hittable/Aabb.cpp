//
// Created by sjbar on 17/01/2023.
//

#include "hittable/Aabb.hpp"

namespace PathRehnda {
    AABB::AABB(const Point3 &a, const Point3 &b) : minimum(a), maximum(b) {

    }

    /**
     * Could use the "slab" method, but the below is copied from Andrew Kensler's (from
     * Pixar) optimised version that compiles well on many compilers.
     */
    bool AABB::does_hit(const Ray &ray, double t_min, double t_max) const {
        for (int a = 0; a < 3; a++) {
            auto inv_d = 1.0f / ray.direction[a];
            auto t_0 = (minimum[a] - ray.origin[a]) * inv_d;
            auto t_1 = (maximum[a] - ray.origin[a]) * inv_d;
            if (inv_d < 0.0f)
                std::swap(t_0, t_1);
            t_min = t_0 > t_min ? t_0 : t_min;
            t_max = t_1 < t_max ? t_1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

    AABB AABB::surrounding_box(AABB box_0, AABB box_1) {
        Point3 small{
                fmin(box_0.minimum.x(), box_1.minimum.x()),
                fmin(box_0.minimum.y(), box_1.minimum.y()),
                fmin(box_0.minimum.z(), box_1.minimum.z())
        };
        Point3 big{
                fmax(box_0.maximum.x(), box_1.maximum.x()),
                fmax(box_0.maximum.y(), box_1.maximum.y()),
                fmax(box_0.maximum.z(), box_1.maximum.z())
        };
        return {
            small,
            big,
        };
    }
} // PathRehnda