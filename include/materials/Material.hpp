//
// Created by sjbar on 11/01/2023.
//

#pragma once

#include "Ray.hpp"
#include "Hittable.hpp"

namespace PathRehnda {

    class Material {
    public:
        virtual bool scatter(const Ray& ray_in, const HitResult& hit_result, ColorRgb& attenuation, Ray& scattered) const = 0;
    };

} // PathRehnda