//
// Created by sjbar on 15/01/2023.
//

#include "Sampler.hpp"
#include "materials/Material.hpp"


namespace PathRehnda {

    Sampler::Sampler(uint32_t max_depth) : max_depth(max_depth) {
    }

    ColorRgb Sampler::sample_ray(const Ray &ray, const Hittable &world, uint32_t depth) {
        // If exceeded max number of light bounces, stop sampling
        if (depth <= 0)
            return ColorRgb::zero();

        // setting t_min to 0.001 avoids shadow acne, where floating point approximation allow reflecting things at t = +/- 0.000001
        if (auto hit_result = world.hit(ray, 0.001, infinity)) {
            if (auto scatter = hit_result.material->scatter(ray, hit_result)) {
                return scatter->attenuation * sample_ray(scatter->scattered_ray, world, depth - 1);
            }
            return ColorRgb::zero();
        }

        Vec3 unit_direction = unit_vector(ray.direction);
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * ColorRgb(1.0, 1.0, 1.0) + t * ColorRgb(0.5, 0.7, 1.0);
    }


    void Sampler::sample_pixels(const Camera& camera, const Hittable& world, ImageBuffer& out_image_buffer, uint32_t samples_per_pixel) {
        for (uint32_t j = out_image_buffer.image_height - 1; j != (uint32_t) -1; --j) {
            int percentage_complete = (int)((float)(out_image_buffer.image_height - j) / (float)out_image_buffer.image_height * 100);
            std::cerr << "\rCompletion: " << percentage_complete << '%' << std::flush;
            for (uint32_t i = 0; i < out_image_buffer.image_width; ++i) {
                for (uint32_t s = 0; s < samples_per_pixel; s++) {
                    auto u = (i + random_double()) / (out_image_buffer.image_width - 1);
                    auto v = (j + random_double()) / (out_image_buffer.image_height - 1);
                    Ray ray = camera.get_ray(u, v);
                    out_image_buffer.write_color_sample(i, j, sample_ray(ray, world, max_depth));
                }
            }
        }
    }
}