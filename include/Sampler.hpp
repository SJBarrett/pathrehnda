//
// Created by sjbar on 15/01/2023.
//

#pragma once

#include "Vec3.hpp"
#include "Ray.hpp"
#include "Hittable.hpp"
#include "Camera.hpp"
#include "ImageBuffer.hpp"

namespace PathRehnda {
    class Sampler {
    public:
        Sampler(uint32_t max_depth);
        ColorRgb sample_ray(const Ray& ray, const Hittable& world, uint32_t depth);
        void sample_pixels(const Camera& camera, const Hittable& world, ImageBuffer& out_image_buffer, uint32_t samples_per_pixel);

    public:
        uint32_t max_depth;
    };
}
