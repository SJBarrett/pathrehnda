//
// Created by sjbar on 11/01/2023.
//

#pragma once

#include <optional>

#include "Ray.hpp"
#include "Hittable.hpp"


namespace PathRehnda {

    struct Scatter {
        Ray scattered_ray;
        ColorRgb attenuation;
    };

    class Material {
    public:
        virtual std::optional<Scatter> scatter(const Ray& ray_in, const HitResult& hit_result) const = 0;
    };

} // PathRehnda