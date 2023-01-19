//
// Created by sjbar on 12/01/2023.
//

#include <optional>
#include <utility>
#include "materials/LambertianMaterial.hpp"
#include "texture/SolidColor.hpp"

namespace PathRehnda {
    LambertianMaterial::LambertianMaterial(const ColorRgb &color) : albedo_texture(std::make_shared<SolidColor>(color)) {
    }

    LambertianMaterial::LambertianMaterial(std::shared_ptr<Texture> albedo_texture) : albedo_texture(std::move(albedo_texture)) {
    }

    std::optional<Scatter> LambertianMaterial::scatter(const Ray &ray_in, const HitResult &hit_result) const {
        auto scatter_direction = hit_result.normal + random_unit_vector();

        if (scatter_direction.near_zero()) {
            scatter_direction = hit_result.normal;
        }

        const Scatter scatter{
            .scattered_ray = Ray{hit_result.hit_location, scatter_direction, ray_in.time},
            .attenuation = albedo_texture->sample(hit_result.uv, hit_result.hit_location),
        };
        return scatter;
    }


} // PathRehnda