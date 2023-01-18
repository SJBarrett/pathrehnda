//
// Created by sjbar on 15/01/2023.
//

#include "Sampler.hpp"
#include "materials/Material.hpp"


namespace PathRehnda {

    Sampler::Sampler(uint32_t max_depth) : max_depth(max_depth) {
    }

    ColorRgb Sampler::sample_ray(const Ray &ray, const Hittable &world, uint32_t depth) const {
        // use the max depth if the depth value has not been initialized
        if (depth == (uint32_t) -1) {
            depth = max_depth;
        }
        // If exceeded max number of light bounces, stop sampling
        if (depth <= 0)
            return ColorRgb::zero();

        // setting t_min to 0.001 avoids shadow acne, where floating point approximation allow reflecting things at t = +/- 0.000001
        if (auto hit_result = world.hit(ray, 0.001, infinity)) {
            if (auto scatter = hit_result->material->scatter(ray, hit_result.value())) {
                return scatter->attenuation * sample_ray(scatter->scattered_ray, world, depth - 1);
            }
            return ColorRgb::zero();
        }

        Vec3 unit_direction = unit_vector(ray.direction);
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * ColorRgb(1.0, 1.0, 1.0) + t * ColorRgb(0.5, 0.7, 1.0);
    }
}