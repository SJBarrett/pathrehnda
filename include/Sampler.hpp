//
// Created by sjbar on 15/01/2023.
//

#pragma once

#include "Vec3.hpp"
#include "Ray.hpp"
#include "hittable/Hittable.hpp"
#include "Camera.hpp"
#include "ImageBuffer.hpp"

namespace PathRehnda {
    class Sampler {
    public:
        explicit Sampler(uint32_t max_depth);

        [[nodiscard]]
        ColorRgb sample_ray(const Ray &ray, const Hittable &world, uint32_t depth = -1) const;

    public:
        uint32_t max_depth;
    };
}
