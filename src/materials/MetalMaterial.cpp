//
// Created by sjbar on 12/01/2023.
//

#include "materials/MetalMaterial.hpp"

namespace PathRehnda {
    MetalMaterial::MetalMaterial(const ColorRgb &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {

    }

    std::optional<Scatter> MetalMaterial::scatter(const Ray &ray_in, const HitResult &hit_result) const {
        Vec3 reflect_dir = reflect(unit_vector(ray_in.direction), hit_result.normal);
        const Scatter scatter{
            .scattered_ray = Ray(hit_result.hit_location, reflect_dir + fuzz * Vec3::random_in_unit_sphere(), ray_in.time),
            .attenuation = albedo,
        };
        return scatter;
    }
} // PathRehnda