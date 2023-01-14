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

        double cos_theta = fmin(dot(-unit_direction, hit_result.normal), 1.0);
        double sin_theta = sqrt(1 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3 scatter_direction;
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
            scatter_direction = reflect(unit_direction, hit_result.normal);
        } else {
            scatter_direction = refract(unit_direction, hit_result.normal, refraction_ratio);
        }

        Scatter scatter{
                .scattered_ray = Ray(hit_result.hit_location, scatter_direction),
                .attenuation = attentuation,
        };
        return scatter;
    }

    double DielectricMaterial::reflectance(double cosine, double ref_idx) {
        auto r_0 = (1 - ref_idx) / (1 + ref_idx);
        r_0 = r_0 * r_0;
        return r_0 + (1 - r_0) * pow((1 - cosine), 5);
    }
}