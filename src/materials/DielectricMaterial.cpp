//
// Created by sjbar on 13/01/2023.
//

#include "materials/DielectricMaterial.hpp"

namespace PathRehnda {

    std::optional<Scatter> DielectricMaterial::scatter(const Ray &ray_in, const HitResult &hit_result) const {
        auto attentuation = ColorRgb(1.0, 1.0, 1.0);
        // TODO this assumes the ray is always going into or out of air
        double refraction_ratio = hit_result.front_face ? (1.0 / refractive_index) : refractive_index;

        Vec3 unit_direction = unit_vector(ray_in.direction);
        Vec3 refracted_direction = refract(unit_direction, hit_result.normal, refraction_ratio);
        Scatter scatter{
                .scattered_ray = Ray(hit_result.hit_location, refracted_direction),
                .attenuation = attentuation,
        };
        return scatter;
    }
}