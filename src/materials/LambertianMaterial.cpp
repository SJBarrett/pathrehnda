//
// Created by sjbar on 12/01/2023.
//

#include <optional>
#include "materials/LambertianMaterial.hpp"

namespace PathRehnda {
    LambertianMaterial::LambertianMaterial(const ColorRgb &color) : albedo(color) {

    }

    std::optional<Scatter> LambertianMaterial::scatter(const Ray &ray_in, const HitResult &hit_result) const {
        auto scatter_direction = hit_result.normal + random_unit_vector();

        if (scatter_direction.near_zero()) {
            scatter_direction = hit_result.normal;
        }

        return Scatter(Ray(hit_result.hit_location, scatter_direction), albedo);
    }
} // PathRehnda