//
// Created by sjbar on 16/01/2023.
//

#include "tracy/Tracy.hpp"
#include "Aggregator.hpp"

namespace PathRehnda {
    void Aggregator::sample_pixels(const SamplingConfig &sampling_config, const Scene &scene, ImageBuffer &out_image_buffer,
                                   bool print_progress) const {
        for (uint32_t j = out_image_buffer.image_height - 1; j != (uint32_t) -1; --j) {
            int percentage_complete = (int) ((float) (out_image_buffer.image_height - j) / (float) out_image_buffer.image_height * 100);
            if (print_progress) {
                std::cerr << "\rCompletion: " << percentage_complete << '%' << std::flush;
            }
            for (uint32_t i = 0; i < out_image_buffer.image_width; ++i) {
                for (uint32_t s = 0; s < sampling_config.samples_per_pixel; s++) {
                    ZoneScopedN("SampleRay");
                    auto u = (i + random_double()) / (out_image_buffer.image_width - 1);
                    auto v = (j + random_double()) / (out_image_buffer.image_height - 1);
                    Ray ray = scene.camera.get_ray(u, v);
                    out_image_buffer.write_color_sample(i, j, sampling_config.sampler.sample_ray(ray, scene.world));
                }
                FrameMark;
            }
        }
    }
}